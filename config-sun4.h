/* Config file for running GCC on Sunos version 4.
   This file is good for either a Sun 3 or a Sun 4 machine.  */

#ifdef sparc 
#include "config-sparc.h" 
#else 
#include "config-m68k.h"
#endif

/* Provide required defaults for linker -e and -d switches.  */

#define LINK_SPEC "%{!e*:-e start} -dc -dp"
