/* Subroutines for insn-output.c for Vax.
   Copyright (C) 1987 Free Software Foundation, Inc.

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

#include <stdio.h>

print_operand_address (file, addr)
     FILE *file;
     register rtx addr;
{
  register rtx reg1, reg2, breg, ireg;
  rtx offset;
  static char *reg_name[] = REGISTER_NAMES;

 retry:
  switch (GET_CODE (addr))
    {
    case MEM:
      fprintf (file, "*");
      addr = XEXP (addr, 0);
      goto retry;

    case REG:
      fprintf (file, "(%s)", reg_name [REGNO (addr)]);
      break;

    case PRE_DEC:
      fprintf (file, "-(%s)", reg_name [REGNO (XEXP (addr, 0))]);
      break;

    case POST_INC:
      fprintf (file, "(%s)+", reg_name [REGNO (XEXP (addr, 0))]);
      break;

    case PLUS:
      reg1 = 0;	reg2 = 0;
      ireg = 0;	breg = 0;
      offset = 0;
      if (CONSTANT_ADDRESS_P (XEXP (addr, 0))
	  || GET_CODE (XEXP (addr, 0)) == MEM)
	{
	  offset = XEXP (addr, 0);
	  addr = XEXP (addr, 1);
	}
      else if (CONSTANT_ADDRESS_P (XEXP (addr, 1))
	       || GET_CODE (XEXP (addr, 1)) == MEM)
	{
	  offset = XEXP (addr, 1);
	  addr = XEXP (addr, 0);
	}
      if (GET_CODE (addr) != PLUS)
	;
      else if (GET_CODE (XEXP (addr, 0)) == MULT)
	{
	  reg1 = XEXP (addr, 0);
	  addr = XEXP (addr, 1);
	}
      else if (GET_CODE (XEXP (addr, 1)) == MULT)
	{
	  reg1 = XEXP (addr, 1);
	  addr = XEXP (addr, 0);
	}
      else if (GET_CODE (XEXP (addr, 0)) == REG)
	{
	  reg1 = XEXP (addr, 0);
	  addr = XEXP (addr, 1);
	}
      else if (GET_CODE (XEXP (addr, 1)) == REG)
	{
	  reg1 = XEXP (addr, 1);
	  addr = XEXP (addr, 0);
	}
      if (GET_CODE (addr) == REG || GET_CODE (addr) == MULT)
	{
	  if (reg1 == 0)
	    reg1 = addr;
	  else
	    reg2 = addr;
	  addr = 0;
	}
      if (offset != 0)
	{
	  if (addr != 0) abort ();
	  addr = offset;
	}
      if (reg1 != 0 && GET_CODE (reg1) == MULT)
	{
	  breg = reg2;
	  ireg = reg1;
	}
      else if (reg2 != 0 && GET_CODE (reg2) == MULT)
	{
	  breg = reg1;
	  ireg = reg2;
	}
      else if (reg2 != 0 || GET_CODE (addr) == MEM)
	{
	  breg = reg2;
	  ireg = reg1;
	}
      else
	{
	  breg = reg1;
	  ireg = reg2;
	}
      if (addr != 0)
	output_address (offset);
      if (breg != 0)
	{
	  if (GET_CODE (breg) != REG)
	    abort ();
	  fprintf (file, "(%s)", reg_name[REGNO (breg)]);
	}
      if (ireg != 0)
	{
	  if (GET_CODE (ireg) == MULT)
	    ireg = XEXP (ireg, 0);
	  if (GET_CODE (ireg) != REG)
	    abort ();
	  fprintf (file, "[%s]", reg_name[REGNO (ireg)]);
	}
      break;

    default:
      output_addr_const (file, addr);
    }
}
