xgcc -O -I. -c -DL_eprintf -o _eprintf.o gnulib.c
xgcc -O -I. -c -DL_umulsi3 -o _umulsi3.o gnulib.c
xgcc -O -I. -c -DL_mulsi3 -o _mulsi3.o gnulib.c
xgcc -O -I. -c -DL_udivsi3 -o _udivsi3.o gnulib.c
xgcc -O -I. -c -DL_divsi3 -o _divsi3.o gnulib.c
xgcc -O -I. -c -DL_umodsi3 -o _umodsi3.o gnulib.c
xgcc -O -I. -c -DL_modsi3 -o _modsi3.o gnulib.c
xgcc -O -I. -c -DL_lshrsi3 -o _lshrsi3.o gnulib.c
xgcc -O -I. -c -DL_lshlsi3 -o _lshlsi3.o gnulib.c
xgcc -O -I. -c -DL_ashrsi3 -o _ashrsi3.o gnulib.c
xgcc -O -I. -c -DL_ashlsi3 -o _ashlsi3.o gnulib.c
xgcc -O -I. -c -DL_divdf3 -o _divdf3.o gnulib.c
xgcc -O -I. -c -DL_muldf3 -o _muldf3.o gnulib.c
xgcc -O -I. -c -DL_negdf2 -o _negdf2.o gnulib.c
xgcc -O -I. -c -DL_adddf3 -o _adddf3.o gnulib.c
xgcc -O -I. -c -DL_subdf3 -o _subdf3.o gnulib.c
xgcc -O -I. -c -DL_cmpdf2 -o _cmpdf2.o gnulib.c
xgcc -O -I. -c -DL_fixunsdfsi -o _fixunsdfsi.o gnulib.c
xgcc -O -I. -c -DL_fixunsdfdi -o _fixunsdfdi.o gnulib.c
xgcc -O -I. -c -DL_fixdfsi -o _fixdfsi.o gnulib.c
xgcc -O -I. -c -DL_fixdfdi -o _fixdfdi.o gnulib.c
xgcc -O -I. -c -DL_floatsidf -o _floatsidf.o gnulib.c
xgcc -O -I. -c -DL_floatdidf -o _floatdidf.o gnulib.c
xgcc -O -I. -c -DL_truncdfsf2 -o _truncdfsf2.o gnulib.c
xgcc -O -I. -c -DL_extendsfdf2 -o _extendsfdf2.o gnulib.c
xgcc -O -I. -c -DL_addsf3 -o _addsf3.o gnulib.c
xgcc -O -I. -c -DL_negsf2 -o _negsf2.o gnulib.c
xgcc -O -I. -c -DL_subsf3 -o _subsf3.o gnulib.c
xgcc -O -I. -c -DL_cmpsf2 -o _cmpsf2.o gnulib.c
xgcc -O -I. -c -DL_mulsf3 -o _mulsf3.o gnulib.c
xgcc -O -I. -c -DL_divsf3 -o _divsf3.o gnulib.c

del gnulib.lib
lib /OUT:gnulib.lib _adddf3.o _addsf3.o _ashlsi3.o _ashrsi3.o _cmpdf2.o _cmpsf2.o _divdf3.o ^
_divsf3.o _divsi3.o _eprintf.o _extendsfdf2.o _fixdfdi.o _fixdfsi.o _fixunsdfdi.o ^
_fixunsdfsi.o _floatdidf.o _floatsidf.o _lshlsi3.o _lshrsi3.o _modsi3.o _muldf3.o ^
_mulsf3.o _mulsi3.o _negdf2.o _negsf2.o _subdf3.o _subsf3.o _truncdfsf2.o ^
_udivsi3.o _umodsi3.o _umulsi3.o

del *.o
