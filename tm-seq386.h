/* Definitions for Sequent Intel 386.
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

/* Use the BSD assembler syntax.  */

#include "tm-bsd386.h"

/* By default, target has a 80387.  */

#define TARGET_DEFAULT 1

/* Specify predefined symbols in preprocessor.  */

#define CPP_PREDEFINES "-Dunix -Di386 -Dsequent"

/* We don't want to output SDB debugging information.  */

#undef SDB_DEBUGGING_INFO

/* We want to output DBX debugging information.  */

#define DBX_DEBUGGING_INFO

/* Floating-point return values come in the FP register.  */

#define VALUE_REGNO(MODE) \
  (((MODE)==SFmode || (MODE)==DFmode) ? FIRST_FLOAT_REG : 0)

/* 1 if N is a possible register number for a function value. */

#define FUNCTION_VALUE_REGNO_P(N) ((N) == 0 || (N)== FIRST_FLOAT_REG)
