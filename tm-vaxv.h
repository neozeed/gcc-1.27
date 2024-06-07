/* Definitions of target machine for GNU compiler.  Vax sysV version.
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

#include "tm-vax.h"

/* Cope with these under SysV */

#define IDENT_DIRECTIVE
#define SCCS_DIRECTIVE

#undef DBX_DEBUGGING_INFO
#define SDB_DEBUGGING_INFO

/* The .file command should always begin the output.  */
#undef ASM_FILE_START
#define ASM_FILE_START(FILE) sdbout_filename ((FILE), main_input_filename)

#undef ASM_OUTPUT_ALIGN
#define ASM_OUTPUT_ALIGN(FILE,LOG) \
  fprintf(FILE, "\t.align %d\n", 1 << (LOG))

#undef ASM_OUTPUT_LOCAL
#define ASM_OUTPUT_LOCAL(FILE,NAME,SIZE,ROUNDED)	\
( fputs (".data\n", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ":\n\t.space %d\n", (ROUNDED)))

#define ASM_OUTPUT_ASCII(FILE,PTR,LEN)  \
{							\
  char *s;						\
  int i;						\
  for (i = 0, s = (PTR); i < (LEN); s++, i++)		\
    {							\
      if ((i % 8) == 0)					\
	fputs ("\n\t.byte\t", (FILE));			\
      fprintf ((FILE), "%s0x%x", (i%8?",":""), (unsigned)*s); \
    }							\
  fputs ("\n", (FILE));					\
}

#undef ASM_OUTPUT_DOUBLE
#define ASM_OUTPUT_DOUBLE(FILE,VALUE)  \
  fprintf (FILE, "\t.double 0d%.20e\n", (VALUE))
