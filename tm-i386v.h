/* Definitions for Intel 386 running system V.
   Copyright (C) 1988 Free Software Foundation, Inc.

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


#include "tm-i386.h"

/* Use the ATT assembler syntax.  */

#include "tm-att386.h"

/* By default, target has a 80387.  */

#define TARGET_DEFAULT 1

/* Use crt1.o as a startup file and crtn.o as a closing file.  */

#define STARTFILE_SPEC  \
  "%{pg:gcrt1.o%s}%{!pg:%{p:mcrt1.o%s}%{!p:crt1.o%s}}"

#define LIB_SPEC "%{!p:%{!pg:-lc}}%{p:-lc_p}%{pg:-lc_p} crtn.o%s"

/* Specify predefined symbols in preprocessor.  */

#define CPP_PREDEFINES "-Dunix -Di386"

/* Allow #ident and #sccs in preprocessor.  */

#define IDENT_DIRECTIVE
#define SCCS_DIRECTIVE

/* We want to output SDB debugging information.  */

#define SDB_DEBUGGING_INFO

/* We don't want to output DBX debugging information.  */

#undef DBX_DEBUGGING_INFO

/* Implicit library calls should use memcpy, not bcopy, etc.  */

#define TARGET_MEM_FUNCTIONS

/* Don't write a `.optim' pseudo; this assembler doesn't handle them.  */

#undef ASM_FILE_START_1
#define ASM_FILE_START_1(FILE)

/* Machines that use the AT&T assembler syntax
   also return floating point values in an FP register.  */
/* Define how to find the value returned by a function.
   VALTYPE is the data type of the value (as a tree).
   If the precise function being called is known, FUNC is its FUNCTION_DECL;
   otherwise, FUNC is 0.  */

#define VALUE_REGNO(MODE) \
  (((MODE)==SFmode || (MODE)==DFmode) ? FIRST_FLOAT_REG : 0)

/* 1 if N is a possible register number for a function value. */

#define FUNCTION_VALUE_REGNO_P(N) ((N) == 0 || (N)== FIRST_FLOAT_REG)
