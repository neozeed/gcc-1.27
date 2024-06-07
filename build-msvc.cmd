del hello.o hello.exe
del insn-flags.h insn-config.h insn-codes.h insn-output.c insn-recog.c 
del insn-emit.c insn-extract.c insn-peep.c 
del c-parse.tab.c c-parse.output cexp.tab.c cexp.output cexp.c
del genemit.exe genoutput.exe genrecog.exe genextract.exe genflags.exe 
del gencodes.exe genconfig.exe genpeep.exe cc1.exe cpp.exe cccp.exe xgcc.exe
del *.co *.greg *.lreg *.combine *.flow *.cse *.jump *.rtl *.tree *.loop
del *.obj *.o
del libgcc.lib

cl /c /Fotoplev.obj toplev.c
cl /c /Foversion.obj version.c
\xdjgpp.v1\bin\bison -v c-parse.y
cl /c c-parse.tab.c
cl /c /Fotree.obj tree.c
cl /c /Foprint-tree.obj print-tree.c
cl /c /Foc-decl.obj c-decl.c
cl /c /Foc-typeck.obj c-typeck.c
cl /c /Fostor-layout.obj stor-layout.c
cl /c /Fofold-const.obj fold-const.c
cl /c /Fortl.obj rtl.c
cl /c genflags.c
cl /c /Foobstack.obj obstack.c
cl /Fegenflags genflags.obj rtl.obj obstack.obj 
genflags md > insn-flags.h
cl /c gencodes.c
cl /Fegencodes gencodes.obj rtl.obj obstack.obj 
gencodes md > insn-codes.h
cl /c genconfig.c
cl /Fegenconfig genconfig.obj rtl.obj obstack.obj 
genconfig md > insn-config.h
cl /c /Foexpr.obj expr.c
cl /c /Fostmt.obj stmt.c
cl /c /Foexpmed.obj expmed.c
cl /c /Foexplow.obj explow.c
cl /c /Fooptabs.obj optabs.c
cl /c /Fovarasm.obj varasm.c
cl /c /Fosymout.obj symout.c
cl /c /Fodbxout.obj dbxout.c
cl /c /Fosdbout.obj sdbout.c
cl /c /Foemit-rtl.obj emit-rtl.c
cl /c genemit.c
cl /Fegenemit genemit.obj rtl.obj obstack.obj 
genemit md > insn-emit.c
cl /c insn-emit.c
cl /c /Fointegrate.obj integrate.c
cl /c /Fojump.obj jump.c
cl /c /Focse.obj cse.c
cl /c /Foloop.obj loop.c
cl /c /Foflow.obj flow.c
cl /c /Fostupid.obj stupid.c
cl /c /Focombine.obj combine.c
cl /c /Foregclass.obj regclass.c
cl /c /Folocal-alloc.obj local-alloc.c
cl /c /Foglobal-alloc.obj global-alloc.c
cl /c /Foreload.obj reload.c
cl /c /Foreload1.obj reload1.c
cl /c genpeep.c
cl /Fegenpeep genpeep.obj rtl.obj obstack.obj 
genpeep md > insn-peep.c
cl /c insn-peep.c
cl /c /Fofinal.obj final.c
cl /c /Forecog.obj recog.c
cl /c genrecog.c
cl /Fegenrecog genrecog.obj rtl.obj obstack.obj 
genrecog md > insn-recog.c
cl /c insn-recog.c
cl /c genextract.c
cl /Fegenextract genextract.obj rtl.obj obstack.obj 
genextract md > insn-extract.c
cl /c insn-extract.c
cl /c genoutput.c
cl /Fegenoutput genoutput.obj rtl.obj obstack.obj 
genoutput md > insn-output.c
cl /c /D__STDC__ insn-output.c
cl /c c-convert.c
cl /Fecc1 toplev.obj version.obj c-parse.tab.obj tree.obj print-tree.obj c-decl.obj c-typeck.obj stor-layout.obj fold-const.obj rtl.obj expr.obj stmt.obj expmed.obj explow.obj optabs.obj varasm.obj symout.obj dbxout.obj sdbout.obj emit-rtl.obj insn-emit.obj integrate.obj jump.obj cse.obj loop.obj flow.obj stupid.obj combine.obj regclass.obj local-alloc.obj global-alloc.obj reload.obj reload1.obj insn-peep.obj final.obj recog.obj insn-recog.obj insn-extract.obj insn-output.obj obstack.obj c-convert.obj


@REM cl -DGCC_INCLUDE_DIR=\"/usr/local/lib/gcc-include\" ^
@REM    -DGPLUSPLUS_INCLUDE_DIR=\"/usr/local/lib/g++-include\" /c cccp.c
cl -DGCC_INCLUDE_DIR=\"/xenixnt/h\" ^
   -DGPLUSPLUS_INCLUDE_DIR=\"/xenixnt/h\" /DUSG /c cccp.c
\xdjgpp.v1\bin\bison -v cexp.y
ren cexp.tab.c cexp.c
cl /c cexp.c
cl /c /Foversion.obj version.c
cl /Fecpp cccp.obj cexp.obj version.obj


@REM cl /c -DSTANDARD_EXEC_PREFIX=\"/usr/local/lib/gcc-\" gcc.c
cl /c -DSTANDARD_EXEC_PREFIX=\"/xenixnt/bin/gcc-\" /DUSG gcc.c
cl /c /Foobstack.obj obstack.c
cl /Fexgcc gcc.obj version.obj obstack.obj

del insn-flags.h insn-config.h insn-codes.h insn-output.c insn-recog.c 
del insn-emit.c insn-extract.c insn-peep.c 
del c-parse.tab.c c-parse.output cexp.tab.c cexp.output cexp.c
del genemit.exe genoutput.exe genrecog.exe genextract.exe genflags.exe 
del gencodes.exe genconfig.exe genpeep.exe
del *.co *.greg *.lreg *.combine *.flow *.cse *.jump *.rtl *.tree *.loop
del *.obj *.o

cl /c /I. /Ox -DL_eprintf /Fo_eprintf.obj gnulib.c
cl /c /I. /Ox -DL_umulsi3 /Fo_umulsi3.obj gnulib.c
cl /c /I. /Ox -DL_mulsi3 /Fo_mulsi3.obj gnulib.c
cl /c /I. /Ox -DL_udivsi3 /Fo_udivsi3.obj gnulib.c
cl /c /I. /Ox -DL_divsi3 /Fo_divsi3.obj gnulib.c
cl /c /I. /Ox -DL_umodsi3 /Fo_umodsi3.obj gnulib.c
cl /c /I. /Ox -DL_modsi3 /Fo_modsi3.obj gnulib.c
cl /c /I. /Ox -DL_lshrsi3 /Fo_lshrsi3.obj gnulib.c
cl /c /I. /Ox -DL_lshlsi3 /Fo_lshlsi3.obj gnulib.c
cl /c /I. /Ox -DL_ashrsi3 /Fo_ashrsi3.obj gnulib.c
cl /c /I. /Ox -DL_ashlsi3 /Fo_ashlsi3.obj gnulib.c
cl /c /I. /Ox -DL_divdf3 /Fo_divdf3.obj gnulib.c
cl /c /I. /Ox -DL_muldf3 /Fo_muldf3.obj gnulib.c
cl /c /I. /Ox -DL_negdf2 /Fo_negdf2.obj gnulib.c
cl /c /I. /Ox -DL_adddf3 /Fo_adddf3.obj gnulib.c
cl /c /I. /Ox -DL_subdf3 /Fo_subdf3.obj gnulib.c
cl /c /I. /Ox -DL_cmpdf2 /Fo_cmpdf2.obj gnulib.c
cl /c /I. /Ox -DL_fixunsdfsi /Fo_fixunsdfsi.obj gnulib.c
cl /c /I. /Ox -DL_fixunsdfdi /Fo_fixunsdfdi.obj gnulib.c
cl /c /I. /Ox -DL_fixdfsi /Fo_fixdfsi.obj gnulib.c
cl /c /I. /Ox -DL_fixdfdi /Fo_fixdfdi.obj gnulib.c
cl /c /I. /Ox -DL_floatsidf /Fo_floatsidf.obj gnulib.c
cl /c /I. /Ox -DL_floatdidf /Fo_floatdidf.obj gnulib.c
cl /c /I. /Ox -DL_truncdfsf2 /Fo_truncdfsf2.obj gnulib.c
cl /c /I. /Ox -DL_extendsfdf2 /Fo_extendsfdf2.obj gnulib.c
cl /c /I. /Ox -DL_addsf3 /Fo_addsf3.obj gnulib.c
cl /c /I. /Ox -DL_negsf2 /Fo_negsf2.obj gnulib.c
cl /c /I. /Ox -DL_subsf3 /Fo_subsf3.obj gnulib.c
cl /c /I. /Ox -DL_cmpsf2 /Fo_cmpsf2.obj gnulib.c
cl /c /I. /Ox -DL_mulsf3 /Fo_mulsf3.obj gnulib.c
cl /c /I. /Ox -DL_divsf3 /Fo_divsf3.obj gnulib.c

del gnulib.lib
lib /OUT:gnulib.lib _adddf3.obj _addsf3.obj _ashlsi3.obj _ashrsi3.obj _cmpdf2.obj _cmpsf2.obj _divdf3.obj ^
_divsf3.obj _divsi3.obj _eprintf.obj _extendsfdf2.obj _fixdfdi.obj _fixdfsi.obj _fixunsdfdi.obj ^
_fixunsdfsi.obj _floatdidf.obj _floatsidf.obj _lshlsi3.obj _lshrsi3.obj _modsi3.obj _muldf3.obj ^
_mulsf3.obj _mulsi3.obj _negdf2.obj _negsf2.obj _subdf3.obj _subsf3.obj _truncdfsf2.obj ^
_udivsi3.obj _umodsi3.obj _umulsi3.obj

del *.obj


xgcc -c hello.c
link /NODEFAULTLIB:libc.lib /NODEFAULTLIB:OLDNAMES.LIB  -out:hello.exe hello.o -entry:mainCRTStartup gnulib.lib LIBC.LIB KERNEL32.LIB

