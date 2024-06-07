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
