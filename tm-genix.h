#include "tm-encore.h"

/* Mention `(sb)' explicitly in indirect addresses.  */
#define SEQUENT_BASE_REGS

/*  A bug in the GNX 3.0 linker prevents symbol-table entries with a storage-
    class field of C_EFCN (-1) from being accepted. */
#ifdef PUT_SDB_EPILOGUE_END
#undef PUT_SDB_EPILOGUE_END
#endif
#define PUT_SDB_EPILOGUE_END(NAME)

#undef TARGET_VERSION
#define TARGET_VERSION printf (" (32000, National syntax)");

/* Output code needed at the start of `main',
   which really ought to be in crt0.o but isn't.  */

#define MAIN_FUNCTION_PROLOGUE						\
{ extern char *current_function_name;					\
  if (!strcmp ("main", current_function_name))				\
    { fprintf (FILE, "\taddr start,r0\n");				\
      fprintf (FILE, "\tsubd $32,r0\n");				\
      fprintf (FILE, "\tlprw mod,r0\n");				\
      fprintf (FILE, "\tlprd sb,$0\n"); }}

/* Same as the tm-encore definition except
   * Different syntax for double constants.
   * Don't output `?' before external regs.
   * Output `(sb)' in certain indirect refs.  */

#undef PRINT_OPERAND
#define PRINT_OPERAND(FILE, X, CODE)					\
{ if (CODE == '$') putc ('$', FILE);					\
  else if (CODE == '?');						\
  else if (GET_CODE (X) == REG)						\
    fprintf (FILE, "%s", reg_name [REGNO (X)]);				\
  else if (GET_CODE (X) == MEM)						\
    {									\
      rtx xfoo;								\
      xfoo = XEXP (X, 0);						\
      switch (GET_CODE (xfoo))						\
	{								\
	case MEM:							\
	  if (GET_CODE (XEXP (xfoo, 0)) == REG)				\
	    if (REGNO (XEXP (xfoo, 0)) == STACK_POINTER_REGNUM)		\
	      fprintf (FILE, "0(0(sp))");				\
	    else fprintf (FILE, "0(0(%s))",				\
			  reg_name [REGNO (XEXP (xfoo, 0))]);		\
	  else								\
	    {								\
	      fprintf (FILE, "0(");					\
	      paren_base_reg_printed = 0;				\
	      output_address (xfoo);					\
	      if (!paren_base_reg_printed)				\
		fprintf (file, "(sb)");					\
	      putc (')', FILE);						\
	    }								\
	  break;							\
	case REG:							\
	  fprintf (FILE, "0(%s)", reg_name [REGNO (xfoo)]);		\
	  break;							\
	case PRE_DEC:							\
	case POST_INC:							\
	  fprintf (FILE, "tos");					\
	  break;							\
	case CONST_INT:							\
	  fprintf (FILE, "@%d", INTVAL (xfoo));				\
	  break;							\
	default:							\
	  output_address (xfoo);					\
	  break;							\
	}								\
    }									\
  else if (GET_CODE (X) == CONST_DOUBLE)				\
    if (GET_MODE (X) == DFmode)						\
      { union { double d; int i[2]; } u;				\
	u.i[0] = XINT (X, 0); u.i[1] = XINT (X, 1);			\
	fprintf (FILE, "$0l%.20e", u.d); }				\
    else { union { double d; int i[2]; } u;				\
	   u.i[0] = XINT (X, 0); u.i[1] = XINT (X, 1);			\
	   fprintf (FILE, "$0f%.20e", u.d); }				\
  else if (GET_CODE (X) == CONST)					\
    output_addr_const (FILE, X);					\
  else { putc ('$', FILE); output_addr_const (FILE, X); }}
