/* Subroutines for insn-output.c for Sun SPARC.
   Copyright (C) 1987, 1988 Free Software Foundation, Inc.
   Contributed by Michael Tiemann (tiemann@mcc.com)

This file is part of GNU CC.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.  No author or distributor
accepts responsibility to anyone for the consequences of using it
or for whether it serves any particular purpose or works at all,
unless he says so in writing.  Refer to the GNU CC General Public
License for full details.

Everyone is granted permission to copy, modify and redistribute
GNU CC, but only under the conditions described in the
GNU CC General Public License.   A copy of this license is
supposed to have been given to you along with GNU CC so you
can know your rights and responsibilities.  It should be in a
file named COPYING.  Among other things, the copyright notice
and this notice must be preserved on all copies.  */

/* Global variables for machine-dependend things.  */

/* This should go away if we pass floats to regs via
   the stack instead of the frame, and if we learn how
   to renumber all the registers when we don't do a save (hard!).  */
extern int frame_pointer_needed;

static rtx find_addr_reg ();

/* Return non-zero if this pattern, as a source to a "SET",
   is known to yield an instruction of unit size.  */
int
single_insn_src_p (op, mode)
     rtx op;
     enum machine_mode mode;
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
      if (SMALL_INT (op))
	return 1;
      return 0;

    case REG:
    case MEM:
      return 1;

      /* We never need to negate or complement constants.  */
    case NOT:
    case NEG:
      return 1;

    case PLUS:
    case MINUS:
    case AND:
    case IOR:
    case XOR:
    case LSHIFT:
    case ASHIFT:
    case ASHIFTRT:
    case LSHIFTRT:
      if ((GET_CODE (XEXP (op, 0)) == CONST_INT && ! SMALL_INT (XEXP (op, 0)))
	  || (GET_CODE (XEXP (op, 1)) == CONST_INT && ! SMALL_INT (XEXP (op, 1))))
	return 0;
      return 1;

    case SUBREG:
      if (SUBREG_WORD (op) != 0)
	return 0;
      return single_insn_src_p (SUBREG_REG (op), mode);

    case SIGN_EXTEND:
    case ZERO_EXTEND:
      /* Lazy... could check for these.  */
      return 0;

      /* Not doing floating point, since they probably
	 take longer than the branch slot they might fill.  */
    case FLOAT_EXTEND:
    case FLOAT_TRUNCATE:
    case FLOAT:
    case FIX:
    case UNSIGNED_FLOAT:
    case UNSIGNED_FIX:
      return 0;

    default:
      return 0;
    }
}

/* ??? Comment needed here */

int
arith_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  return (register_operand (op, mode)
	  || (GET_CODE (op) == CONST_INT && SMALL_INT (op)));
}

/* ??? Comment needed here */

int
arith32_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  return (register_operand (op, mode) || GET_CODE (op) == CONST_INT);
}

int
small_int (op, mode)
     rtx op;
     enum machine_mode mode;
{
  return (GET_CODE (op) == CONST_INT && SMALL_INT (op));
}

/* Return the best assembler insn template
   for moving operands[1] into operands[0] as a fullword.  */

static char *
singlemove_string (operands)
     rtx *operands;
{
  if (GET_CODE (operands[0]) == MEM)
    return "st %r1,%0";
  if (GET_CODE (operands[1]) == MEM)
    return "ld %1,%0";
  return "mov %1,%0";
}

/* Output assembler code to perform a doubleword move insn
   with operands OPERANDS.  */

char *
output_move_double (operands)
     rtx *operands;
{
  enum { REGOP, OFFSOP, MEMOP, PUSHOP, POPOP, CNSTOP, RNDOP } optype0, optype1;
  rtx latehalf[2];
  rtx addreg0 = 0, addreg1 = 0;

  /* First classify both operands.  */

  if (REG_P (operands[0]))
    optype0 = REGOP;
  else if (offsetable_memref_p (operands[0]))
    optype0 = OFFSOP;
  else if (GET_CODE (operands[0]) == MEM)
    optype0 = MEMOP;
  else
    optype0 = RNDOP;

  if (REG_P (operands[1]))
    optype1 = REGOP;
  else if (CONSTANT_P (operands[1])
	   || GET_CODE (operands[1]) == CONST_DOUBLE)
    optype1 = CNSTOP;
  else if (offsetable_memref_p (operands[1]))
    optype1 = OFFSOP;
  else if (GET_CODE (operands[1]) == MEM)
    optype0 = MEMOP;
  else
    optype1 = RNDOP;

  /* Check for the cases that the operand constraints are not
     supposed to allow to happen.  Abort if we get one,
     because generating code for these cases is painful.  */

  if (optype0 == RNDOP || optype1 == RNDOP)
    abort ();

  /* If an operand is an unoffsettable memory ref, find a register
     we can increment temporarily to make it refer to the second word.  */

  if (optype0 == MEMOP)
    addreg0 = find_addr_reg (operands[0]);

  if (optype1 == MEMOP)
    addreg1 = find_addr_reg (operands[1]);

  /* Ok, we can do one word at a time.
     Normally we do the low-numbered word first,
     but if either operand is autodecrementing then we
     do the high-numbered word first.

     In either case, set up in LATEHALF the operands to use
     for the high-numbered word and in some cases alter the
     operands in OPERANDS to be suitable for the low-numbered word.  */

  if (optype0 == REGOP)
    latehalf[0] = gen_rtx (REG, SImode, REGNO (operands[0]) + 1);
  else if (optype0 == OFFSOP)
    latehalf[0] = adj_offsetable_operand (operands[0], 4);
  else
    latehalf[0] = operands[0];

  if (optype1 == REGOP)
    latehalf[1] = gen_rtx (REG, SImode, REGNO (operands[1]) + 1);
  else if (optype1 == OFFSOP)
    latehalf[1] = adj_offsetable_operand (operands[1], 4);
  else if (optype1 == CNSTOP)
    {
      if (CONSTANT_P (operands[1]))
	latehalf[1] = const0_rtx;
      else if (GET_CODE (operands[1]) == CONST_DOUBLE)
	{
	  latehalf[1] = gen_rtx (CONST_INT, VOIDmode, XINT (operands[1], 1));
	  operands[1] = gen_rtx (CONST_INT, VOIDmode, XINT (operands[1], 0));
	}
    }
  else
    latehalf[1] = operands[1];

  /* If the first move would clobber the source of the second one,
     do them in the other order.

     RMS says "This happens only for registers;
     such overlap can't happen in memory unless the user explicitly
     sets it up, and that is an undefined circumstance."

     but it happens on the sparc when loading parameter registers,
     so I am going to define that circumstance, and make it work
     as expected.  */

  /* Easy case: try moving both words at once.  */
  if ((optype0 == REGOP && optype1 != REGOP
       && (REGNO (operands[0]) & 1) == 0)
      || (optype0 != REGOP && optype1 == REGOP
	  && (REGNO (operands[1]) & 1) == 0))
    {
      rtx op1, op2;
      rtx base = 0, offset = const0_rtx;

      if (optype0 == REGOP)
	op1 = operands[0], op2 = XEXP (operands[1], 0);
      else
	op1 = operands[1], op2 = XEXP (operands[0], 0);

      /* Trust global variables.  */
      if (GET_CODE (op2) == SYMBOL_REF
	  || GET_CODE (op2) == CONST
	  || GET_CODE (op2) == REG)
	{
	  if (op1 == operands[0])
	    return "ldd %1,%0";
	  else
	    return "std %1,%0";
	}

      if (GET_CODE (op2) == PLUS)
	{
	  if (GET_CODE (XEXP (op2, 0)) == REG)
	    base = XEXP (op2, 0), offset = XEXP (op2, 1);
	  else if (GET_CODE (XEXP (op2, 1)) == REG)
	    base = XEXP (op2, 1), offset = XEXP (op2, 0);
	}

      /* Trust round enough offsets from the stack or frame pointer.  */
      if (base
	  && (REGNO (base) == FRAME_POINTER_REGNUM
	      || REGNO (base) == STACK_POINTER_REGNUM))
	{
	  if (GET_CODE (offset) == CONST_INT
	      && (INTVAL (offset) & 0x7) == 0)
	    {
	      if (op1 == operands[0])
		return "ldd %1,%0";
	      else
		return "std %1,%0";
	    }
	}
      else
	{
	  /* We know structs not on the stack are properly aligned.  */
	  if (operands[1]->in_struct)
	    return "ldd %1,%0";
	  else if (operands[0]->in_struct)
	    return "std %1,%0";
	}
    }

  if (optype0 == REGOP && optype1 == REGOP
      && REGNO (operands[0]) == REGNO (latehalf[1]))
    {
      /* Make any unoffsetable addresses point at high-numbered word.  */
      if (addreg0)
	output_asm_insn ("add %0,0x4,%0", &addreg0);
      if (addreg1)
	output_asm_insn ("add %0,0x4,%0", &addreg1);

      /* Do that word.  */
      output_asm_insn (singlemove_string (latehalf), latehalf);

      /* Undo the adds we just did.  */
      if (addreg0)
	output_asm_insn ("add %0,-0x4,%0", &addreg0);
      if (addreg1)
	output_asm_insn ("add %0,-0x4,%0", &addreg0);

      /* Do low-numbered word.  */
      return singlemove_string (operands);
    }
  else if (optype0 == REGOP && optype1 != REGOP
	   && reg_mentioned_p (operands[0], XEXP (operands[1], 0)))
    {
      /* Do the late half first.  */
      output_asm_insn (singlemove_string (latehalf), latehalf);
      /* Then clobber.  */
      return singlemove_string (operands);
    }

  /* Normal case: do the two words, low-numbered first.  */

  output_asm_insn (singlemove_string (operands), operands);

  /* Make any unoffsetable addresses point at high-numbered word.  */
  if (addreg0)
    output_asm_insn ("add %0,0x4,%0", &addreg0);
  if (addreg1)
    output_asm_insn ("add %0,0x4,%0", &addreg1);

  /* Do that word.  */
  output_asm_insn (singlemove_string (latehalf), latehalf);

  /* Undo the adds we just did.  */
  if (addreg0)
    output_asm_insn ("add %0,-0x4,%0", &addreg0);
  if (addreg1)
    output_asm_insn ("add %0,-0x4,%0", &addreg1);

  return "";
}

static char *
output_fp_move_double (operands)
     rtx *operands;
{
  if (FP_REG_P (operands[0]))
    {
      if (FP_REG_P (operands[1]))
	{
	  output_asm_insn ("fmovs %1,%0", operands);
	  operands[0] = gen_rtx (REG, VOIDmode, REGNO (operands[0]) + 1);
	  operands[1] = gen_rtx (REG, VOIDmode, REGNO (operands[1]) + 1);
	  return "fmovs %1,%0";
	}
      if (GET_CODE (operands[1]) == REG)
	{
	  if ((REGNO (operands[1]) & 1) == 0)
	    return "std %1,[%%fp-8]\n\tldd [%%fp-8],%0";
	  else
	    {
	      rtx xoperands[3];
	      xoperands[0] = operands[0];
	      xoperands[1] = operands[1];
	      xoperands[2] = gen_rtx (REG, SImode, REGNO (operands[1]) + 1);
	      output_asm_insn ("st %2,[%%fp-4]\n\tst %1,[%%fp-8]\n\tldd [%%fp-8],%0", xoperands);
	      return "";
	    }
	}
      if (GET_CODE (XEXP (operands[1], 0)) == PLUS
	  && (XEXP (XEXP (operands[1], 0), 0) == frame_pointer_rtx
	      || XEXP (XEXP (operands[1], 0), 0) == stack_pointer_rtx)
	  && GET_CODE (XEXP (XEXP (operands[1], 0), 1)) == CONST_INT
	  && (INTVAL (XEXP (XEXP (operands[1], 0), 1)) & 0x7) != 0)
	{
	  rtx xoperands[2];
	  output_asm_insn ("ld %1,%0", operands);
	  xoperands[0] = gen_rtx (REG, GET_MODE (operands[0]),
				  REGNO (operands[0]) + 1);
	  xoperands[1] = gen_rtx (MEM, GET_MODE (operands[1]),
				  plus_constant (XEXP (operands[1], 0), 4));
	  output_asm_insn ("ld %1,%0", xoperands);
	  return "";
	}
      return "ldd %1,%0";
    }
  else if (FP_REG_P (operands[1]))
    {
      if (GET_CODE (operands[0]) == REG)
	{
	  if ((REGNO (operands[0]) & 1) == 0)
	    return "std %1,[%%fp-8]\n\tldd [%%fp-8],%0";
	  else
	    {
	      rtx xoperands[3];
	      xoperands[2] = operands[1];
	      xoperands[1] = gen_rtx (REG, SImode, REGNO (operands[0]) + 1);
	      xoperands[0] = operands[0];
	      output_asm_insn ("std %2,[%%fp-8]\n\tld [%%fp-4],%1\n\tld [%%fp-8],%0", xoperands);
	      return "";
	    }
	}
      if (GET_CODE (XEXP (operands[0], 0)) == PLUS
	  && (XEXP (XEXP (operands[0], 0), 0) == frame_pointer_rtx
	      || XEXP (XEXP (operands[0], 0), 0) == stack_pointer_rtx)
	  && GET_CODE (XEXP (XEXP (operands[0], 0), 1)) == CONST_INT
	  && (INTVAL (XEXP (XEXP (operands[0], 0), 1)) & 0x7) != 0)
	{
	  rtx xoperands[2];
	  output_asm_insn ("st %1,%0", operands);
	  xoperands[1] = gen_rtx (REG, GET_MODE (operands[1]),
				  REGNO (operands[1]) + 1);
	  xoperands[0] = gen_rtx (MEM, GET_MODE (operands[0]),
				  plus_constant (XEXP (operands[0], 0), 4));
	  output_asm_insn ("st %1,%0", xoperands);
	  return "";
	}
      return "std %1,%0";
    }
  else abort ();
}

/* Return a REG that occurs in ADDR with coefficient 1.
   ADDR can be effectively incremented by incrementing REG.  */

static rtx
find_addr_reg (addr)
     rtx addr;
{
  while (GET_CODE (addr) == PLUS)
    {
      if (GET_CODE (XEXP (addr, 0)) == REG)
	addr = XEXP (addr, 0);
      if (GET_CODE (XEXP (addr, 1)) == REG)
	addr = XEXP (addr, 1);
      if (CONSTANT_P (XEXP (addr, 0)))
	addr = XEXP (addr, 1);
      if (CONSTANT_P (XEXP (addr, 1)))
	addr = XEXP (addr, 0);
    }
  if (GET_CODE (addr) == REG)
    return addr;
  return 0;
}

/* Load the address specified by OPERANDS[3] into the register
   specified by OPERANDS[0].

   OPERANDS[3] may be the result of a sum, hence it could either be:

   (1) CONST
   (2) REG
   (2) REG + CONST_INT
   (3) REG + REG + CONST_INT

   All cases are handled here.  */

void
output_load_address (operands)
     rtx *operands;
{
  rtx base, offset;

  if (CONSTANT_P (operands[3]))
    {
      output_asm_insn ("set %3,%0", operands);
      return;
    }

  if (REG_P (operands[3]))
    {
      if (REGNO (operands[0]) != REGNO (operands[3]))
	output_asm_insn ("mov %3,%0", operands);
      return;
    }

  base = XEXP (operands[3], 0);
  offset = XEXP (operands[3], 1);

  if (GET_CODE (base) == CONST_INT)
    {
      rtx tmp = base;
      base = offset;
      offset = tmp;
    }

  if (GET_CODE (offset) != CONST_INT)
    abort ();

  if (REG_P (base))
    {
      operands[6] = base;
      operands[7] = offset;
      if (SMALL_INT (offset))
	output_asm_insn ("add %6,%7,%0", operands);
      else
	output_asm_insn ("set %7,%0\n\tadd %0,%6,%0", operands);
    }
  else
    {
      operands[6] = XEXP (base, 0);
      operands[7] = XEXP (base, 1);
      operands[8] = offset;

      if (SMALL_INT (offset))
	output_asm_insn ("add %6,%7,%0\n\tadd %0,%8,%0", operands);
      else
	output_asm_insn ("set %8,%0\n\tadd %0,%6,%0\n\tadd %0,%7,%0", operands);
    }
}

char *
output_block_move (operands)
     rtx *operands;
{
  static int movstrsi_label = 0;
  int align = 4;

  rtx xoperands[9];
  int available[3];
  int i, j;

  /* Since we clobber untold things, nix the condition codes.  */
  CC_STATUS_INIT;

  /* Get past the MEMs.  */
  operands[0] = XEXP (operands[0], 0);
  operands[1] = XEXP (operands[1], 0);

  xoperands[0] = 0;
  xoperands[1] = 0;
  xoperands[2] = 0;

  available[0] = 1;
  available[1] = 1;
  available[2] = 1;
#if 1
  /* Prepare to juggle registers if necessary.  */
  if (REG_P (operands[0]) && (unsigned) (REGNO (operands[0]) - 8) < 3)
    {
      xoperands[0] = operands[0];
      available[REGNO (operands[0]) - 8] = 0;
    }
  if (REG_P (operands[1]) && (unsigned) (REGNO (operands[1]) - 8) < 3)
    {
      xoperands[1] = operands[1];
      available[REGNO (operands[1]) - 8] = 0;
    }
  if (REG_P (operands[2]) && (unsigned) (REGNO (operands[2]) - 8) < 3)
    {
      xoperands[2] = operands[2];
      available[REGNO (operands[2]) - 8] = 0;
    }
  for (i = 0; i < 3; i++)
    {
      if (xoperands[i])
	continue;
      if (available[0])
	{
	  xoperands[i] = gen_rtx (REG, SImode, 8);
	  available[0] = 0;
	  continue;
	}
      if (available[1])
	{
	  xoperands[i] = gen_rtx (REG, SImode, 9);
	  available[1] = 0;
	  continue;
	}
      xoperands[i] = gen_rtx (REG, SImode, 10);
      available[2] = 0;
    }
#endif

  /* First, figure out best alignment we may assume.  */
  if (REG_P (operands[2]))
    {
      xoperands[5] = operands[2];
      output_asm_insn ("sub %5,0x1,%2", xoperands);
      align = 1;
    }
  else
    {
      int i = INTVAL (operands[2]);

      if (i & 1)
	align = 1;
      else if (i & 3)
	align = 2;

      /* predecrement count.  */
      i -= align;
      if (i < 0) abort ();

      xoperands[5] = gen_rtx (CONST_INT, VOIDmode, i);

      output_asm_insn ("set %5,%2", xoperands);
    }

  /* Now, set up for pipelined operation: dest must contain
     a pre-incremented address, because its index is pre-decremented.  */

  xoperands[3] = plus_constant (operands[0], align);
  output_load_address (xoperands);

  xoperands[4] = operands[1];
  output_load_address (xoperands+1);

  xoperands[3] = gen_rtx (CONST_INT, VOIDmode, movstrsi_label++);
  xoperands[4] = gen_rtx (CONST_INT, VOIDmode, align);

  output_asm_insn ("\nLm%3:\n\tld [%1+%2],%%g1\n\tsubcc %2,%4,%2\n\tbge Lm%3\n\tst %%g1,[%0+%2]", xoperands);
  return "";
}

#define ABS(x) ((x) < 0 ? -(x) : x)

char *
output_mul_by_constant (insn, operands, unsignedp)
     rtx insn;
     rtx *operands;
     int unsignedp;
{
  int c;			/* Size of constant */
  int shifts[BITS_PER_WORD];	/* Table of shifts */
  unsigned int p, log;		/* A power of two, and its log */
  int d1, d2;			/* Differences of c and p */
  int first = 1;		/* True if dst has unknown data in it */
  int i;

  c = INTVAL (operands[2]);
  if (c == 0)
    {
      /* should not happen.  */
      abort ();
      if (GET_CODE (operands[0]) == MEM)
	return "st %%g0,%0";
      return "mov %%g0,%0";
    }

#if 0
  printf ("open coding insn:\n");
  debug_rtx (insn);
  printf ("done.\n");
#endif

  output_asm_insn ("! start open coded multiply");

  /* Clear out the table of shifts. */
  for (i = 0; i < BITS_PER_WORD; ++i)
    shifts[i] = 0;

  while (c)
    {
      /* Find the power of two nearest ABS(c) */
      p = 1, log = 0;
      do
	{
	  d1 = ABS(c) - p;
	  p *= 2;
	  ++log;
	}
      while (p < ABS(c));
      d2 = p - ABS(c);

      /* Make an appropriate entry in shifts for p. */
      if (d2 < d1)
	{
	  shifts[log] = c < 0 ? -1 : 1;
	  c = c < 0 ? d2 : -d2;
	}
      else
	{
	  shifts[log - 1] = c < 0 ? -1 : 1;
	  c = c < 0 ? -d1 : d1;
	}
    }

  /* For now, use a known clobberable register.  This could
     be improved by looking at insn, and seeing if we
     have a dying register contained therein.  */
  regs_ever_live[15] = 1;
  operands[3] = gen_rtx (REG, SImode, 15);

  /* Take care of the first insn in sequence.
     We know we have at least one. */

  /* A value of -1 in shifts says to subtract that power of two, and a value
     of 1 says to add that power of two. */
  for (i = 0; ; i++)
    if (shifts[i])
      {
	if (i)
	  {
	    operands[2] = gen_rtx (CONST_INT, VOIDmode, i);
	    output_asm_insn ("sll %1,%2,%3", operands);
	  }
	else if (REGNO (operands[3]) == 15)
	  output_asm_insn ("mov %1,%3", operands);

	log = i;
	if (shifts[i] < 0)
	  output_asm_insn ("sub %%g0,%3,%0", operands);
	else
	  output_asm_insn ("mov %3,%0", operands);
	break;
      }

  /* A value of -1 in shifts says to subtract that power of two, and a value
     of 1 says to add that power of two--continued.  */
  for (i += 1; i < BITS_PER_WORD; ++i)
    if (shifts[i])
      {
	if (i - log > 0)
	  {
	    operands[2] = gen_rtx (CONST_INT, VOIDmode, i - log);
	    output_asm_insn ("sll %3,%2,%3", operands);
	  }
	else
	  {
	    operands[2] = gen_rtx (CONST_INT, VOIDmode, log - i);
	    output_asm_insn ("sra %3,%2,%3", operands);
	  }
	log = i;
	if (shifts[i] < 0)
	  output_asm_insn ("sub %0,%3,%0", operands);
	else
	  output_asm_insn ("add %0,%3,%0", operands);
      }

  output_asm_insn ("! end open coded multiply");

  return "";
}

char *
output_mul_insn (operands, unsignedp)
     rtx *operands;
     int unsignedp;
{
  int lucky1 = ((unsigned)REGNO (operands[1]) - 8) <= 1;
  int lucky2 = ((unsigned)REGNO (operands[2]) - 8) <= 1;

  if (lucky1)
    if (lucky2)
      output_asm_insn ("call .mul,2\n\tnop", operands);
    else
      {
	rtx xoperands[2];
	xoperands[0] = gen_rtx (REG, SImode,
				8 ^ (REGNO (operands[1]) == 8));
	xoperands[1] = operands[2];
	output_asm_insn ("call .mul,2\n\tmov %1,%0", xoperands);
      }
  else if (lucky2)
    {
      rtx xoperands[2];
      xoperands[0] = gen_rtx (REG, SImode,
			      8 ^ (REGNO (operands[2]) == 8));
      xoperands[1] = operands[1];
      output_asm_insn ("call .mul,2\n\tmov %1,%0", xoperands);
    }
  else
    {
      output_asm_insn ("mov %1,%%o0\n\tcall .mul,2\n\tmov %2,%%o1",
		       operands);
    }

  if (REGNO (operands[0]) == 8)
    return "";
  return "mov %%o0,%0";
}

#if 0
/* This function does not properly protect its operands.  */
char *
output_arith_insn (operands, name)
     rtx *operands;
     char *name;
{
  extern struct _iobuf *asm_out_file;

  /* Does not commute.  */
  rtx op1 = 0;
  rtx op2 = 0;

  abort ();
  if (GET_CODE (operands[1]) == REG && REGNO (operands[1]) == 8)
    op1 = operands[1];
  if (GET_CODE (operands[2]) == REG && REGNO (operands[2]) == 8)
    op2 = operands[2];

  if (op1 && op2)
    {
      /* ?? should have been done earlier.  */
      abort ();
    }
  else
    {
      if (op2)
	{
	  op1 = operands[1];
	  if (REG_P (op1) && REGNO (op1) == 9)
	    {
	      output_asm_insn ("mov %%o1,%%o2\n\tmov %%o0,%%o1", operands);
	      fprintf (asm_out_file, "\tcall %s,2\n", name);
	      output_asm_insn ("mov %%o2,%%o0", operands);
	      goto done;
	    }
	  output_asm_insn ("mov %%o0,%%o1", operands);
	  if (GET_CODE (op1) == CONST_INT && ! SMALL_INT (op1))
	    {
	      output_asm_insn ("sethi %%hi(%1),%%o0", operands);
	      fprintf (asm_out_file, "\tcall %s,2\n", name);
	      output_asm_insn ("add %%o0,%%lo(%1),%%o0", operands);
	    }
	  else
	    {
	      fprintf (asm_out_file, "\tcall %s,2\n", name);
	      if (GET_CODE (op1) == MEM)
		output_asm_insn ("ld %1,%%o0", operands);
	      else
		output_asm_insn ("mov %1,%%o0", operands);
	    }
	}
      else if (op1)
	{
	  op2 = operands[2];
	  if (REG_P (op2) && REGNO (operands[1]) == 9)
	    {
	      fprintf (asm_out_file, "\tcall %s,2\n\tnop\n");
	      goto done;
	    }
	  if (GET_CODE (op2) == CONST_INT && ! SMALL_INT (op2))
	    {
	      output_asm_insn ("sethi %%hi(%2),%%o1", operands);
	      fprintf (asm_out_file, "\tcall %s,2\n", name);
	      output_asm_insn ("add %%o1,%%lo(%2),%%o1", operands);
	    }
	  else
	    {
	      fprintf (asm_out_file, "\tcall %s,2\n", name);
	      if (GET_CODE (op2) == MEM)
		output_asm_insn ("ld %2,%%o1", operands);
	      else
		output_asm_insn ("mov %2,%%o1", operands);
	    }
	}
      else
	{
	  op1 = operands[1];
	  op2 = operands[2];

	  if (! REG_P (op2) || REGNO (op2) != 9)
	    if (GET_CODE (op2) == MEM)
	      output_asm_insn ("ld %2,%%o1", operands);
	    else if (GET_CODE (op2) != CONST_INT || SMALL_INT (op2))
	      output_asm_insn ("mov %2,%%o1", operands);
	    else
	      output_asm_insn ("sethi %%hi(%2),%%o1\n\tadd %%o1,%%lo(%2),%%o1", operands);
	  if (GET_CODE (op1) == CONST_INT && ! SMALL_INT (op1))
	    {
	      output_asm_insn ("sethi %%hi(%1),%%o0", operands);
	      fprintf (asm_out_file, "\tcall %s,2\n", name);
	      output_asm_insn ("add %%o0,%%lo(%1),%%o0", operands);
	    }
	  else
	    {
	      fprintf (asm_out_file, "\tcall %s,2\n", name);
	      if (GET_CODE (op1) == MEM)
		output_asm_insn ("ld %1,%%o0", operands);
	      else
		output_asm_insn ("mov %1,%%o0", operands);
	    }
	}
    done:
      if (REG_P (operands[0]) && REGNO (operands[0]) == 8)
	return "";
      if (GET_CODE (operands[0]) == MEM)
	return "st %%o0,%0";
      return "mov %%o0,%0";
    }
}
#endif

/* Make floating point register f0 contain 0.
   SIZE is the number of registers (including f0)
   which should contain 0.  */

void
make_f0_contain_0 (size)
     int size;
{
  if (size == 1)
    output_asm_insn ("ld [%%fp-16],%%f0", 0);
  else if (size == 2)
    output_asm_insn ("ldd [%%fp-16],%%f0", 0);
}

/* Output reasonable peephole for set-on-condition-code insns.
   Note that these insns assume a particular way of defining
   labels.  Therefore, *both* tm-sparc.h and this function must
   be changed if a new syntax is needed.  */

char *
output_scc_insn (code, operands)
     enum rtx_code code;
     rtx *operands;
{
  rtx xoperands[2];
  rtx label = gen_label_rtx ();

  xoperands[0] = operands[0];
  xoperands[1] = label;

  switch (code)
    {
    case NE:
      if (cc_status.flags & CC_IN_FCCR)
	output_asm_insn ("fbne,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      else
	output_asm_insn ("bne,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      break;
    case EQ:
      if (cc_status.flags & CC_IN_FCCR)
	output_asm_insn ("fbe,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      else
	output_asm_insn ("be,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      break;
    case GE:
      if (cc_status.flags & CC_IN_FCCR)
	output_asm_insn ("fbge,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      else
	output_asm_insn ("bge,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      break;
    case GT:
      if (cc_status.flags & CC_IN_FCCR)
	output_asm_insn ("fbg,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      else
	output_asm_insn ("bg,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      break;
    case LE:
      if (cc_status.flags & CC_IN_FCCR)
	output_asm_insn ("fble,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      else
	output_asm_insn ("ble,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      break;
    case LT:
      if (cc_status.flags & CC_IN_FCCR)
	output_asm_insn ("fbl,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      else
	output_asm_insn ("bl,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      break;
    case GEU:
      if (cc_status.flags & CC_IN_FCCR)
	abort ();
      else
	output_asm_insn ("bgeu,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      break;
    case GTU:
      if (cc_status.flags & CC_IN_FCCR)
	abort ();
      else
	output_asm_insn ("bgu,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      break;
    case LEU:
      if (cc_status.flags & CC_IN_FCCR)
	abort ();
      else
	output_asm_insn ("bleu,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      break;
    case LTU:
      if (cc_status.flags & CC_IN_FCCR)
	abort ();
      else
	output_asm_insn ("blu,a %l1\n\tmov 1,%0\n\tmov 0,%0\n%l1:", xoperands);
      break;
    default:
      abort ();
    }
  return "";
}

/* ??? Comment needed here */

char *
output_delay_insn (template, operands, insn)
     char *template;
     rtx *operands;
     rtx insn;
{
  extern char *insn_template[];
  extern char *(*insn_outfun[])();
  rtx pat = gen_rtx (SET, VOIDmode,
		     XVECEXP (PATTERN (insn), 0, 0),
		     XVECEXP (PATTERN (insn), 0, 1));
  rtx delay_insn = gen_rtx (INSN, VOIDmode, 0, 0, 0, pat, -1, 0, 0);
  int insn_code_number;

  /* Output the branch instruction first.  */
  output_asm_insn (template, operands);

  /* Now recognize the insn which we put in its delay slot.
     We must do this after outputing the branch insn,
     since operands may just be a pointer to `recog_operands'.  */
  insn_code_number = recog (pat, delay_insn);
  if (insn_code_number == -1)
    abort ();

  /* Now get the template for what this insn would
     have been, without the branch.  Its operands are
     exactly the same as they would be, so we don't
     need to do an insn_extract.  */
  template = insn_template[insn_code_number];
  if (template == 0)
    template = (*insn_outfun[insn_code_number]) (operands, delay_insn);
  output_asm_insn (template, operands);
  return "";
}
