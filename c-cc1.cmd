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
