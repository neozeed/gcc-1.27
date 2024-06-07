@REM cl /c -DSTANDARD_EXEC_PREFIX=\"/usr/local/lib/gcc-\" gcc.c
cl /c -DSTANDARD_EXEC_PREFIX=\"/xenixnt/bin/gcc-\" /DUSG gcc.c
cl /c /Foobstack.obj obstack.c
cl /Fexgcc gcc.obj version.obj obstack.obj
