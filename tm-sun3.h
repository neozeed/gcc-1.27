#include "tm-m68k.h"

/* See tm-m68k.h.  7 means 68020 with 68881.  */

#define TARGET_DEFAULT 7

/* Define __HAVE_FPU__ in preprocessor, unless -msoft-float is specified.
   This will control the use of inline 68881 insns in certain macros.  */

#define CPP_SPEC "%{!msoft-float:-D__HAVE_FPU__} %{m68000:-Dmc68010} %{!m68000:-Dmc68020}"

/* -m68000 requires special flags to the assembler.  */

#define ASM_SPEC "%{m68000:-mc68010}%{!m68000:-mc68020}"

/* Names to predefine in the preprocessor for this target machine.  */

#define CPP_PREDEFINES "-Dmc68000 -Dsun -Dunix"

/* STARTFILE_SPEC to include sun floating point initialization
   This is necessary (tr: Sun does it) for both the m68881 and the fpa
   routines.
   Note that includes knowledge of the default specs for gcc, ie. no
   args translates to the same effect as -m68881
   I'm not sure what would happen below if people gave contradictory
   arguments (eg. -msoft-float -mfpa) */
  
#define STARTFILE_SPEC  \
  "%{pg:gcrt0.o%s}%{!pg:%{p:mcrt0.o%s}%{!p:crt0.o%s}}\
   %{mfpa:Wcrt1.o%s} %{!mfpa:%{m68881:Mcrt1.o%s}} \
   %{!mfpa:%{msoft-float:Fcrt1.o%s}} \
   %{!mfpa:%{!m68881:%{!msoft-float:Mcrt1.o%s}}}"

/* Every structure or union's size must be a multiple of 2 bytes.  */

#define STRUCTURE_SIZE_BOUNDARY 16

/* This is BSD, so it wants DBX format.  */

#define DBX_DEBUGGING_INFO
