@REM cl -DGCC_INCLUDE_DIR=\"/usr/local/lib/gcc-include\" ^
@REM    -DGPLUSPLUS_INCLUDE_DIR=\"/usr/local/lib/g++-include\" /c cccp.c
cl -DGCC_INCLUDE_DIR=\"/xenixnt/h\" ^
   -DGPLUSPLUS_INCLUDE_DIR=\"/xenixnt/h\" /DUSG /c cccp.c
\xdjgpp.v1\bin\bison -v cexp.y
ren cexp.tab.c cexp.c
cl /c cexp.c
cl /c /Foversion.obj version.c
cl /Fecpp cccp.obj cexp.obj version.obj
