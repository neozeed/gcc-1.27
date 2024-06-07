;;- Machine description for SPARC chip for GNU C compiler
;;   Copyright (C) 1988 Free Software Foundation, Inc.
;;   Contributed by Michael Tiemann (tiemann@mcc.com)

;; This file is part of GNU CC.

;; GNU CC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY.  No author or distributor
;; accepts responsibility to anyone for the consequences of using it
;; or for whether it serves any particular purpose or works at all,
;; unless he says so in writing.  Refer to the GNU CC General Public
;; License for full details.

;; Everyone is granted permission to copy, modify and redistribute
;; GNU CC, but only under the conditions described in the
;; GNU CC General Public License.   A copy of this license is
;; supposed to have been given to you along with GNU CC so you
;; can know your rights and responsibilities.  It should be in a
;; file named COPYING.  Among other things, the copyright notice
;; and this notice must be preserved on all copies.


;;- See file "rtl.def" for documentation on define_insn, match_*, et. al.

;;- cpp macro #define NOTICE_UPDATE_CC in file tm.h handles condition code
;;- updates for most instructions.

;;- Operand classes for the register allocator:

;; Compare instructions.
;; This controls RTL generation and register allocation.
(define_insn "cmpsi"
  [(set (cc0)
	(minus (match_operand:SI 0 "arith_operand" "rK")
	       (match_operand:SI 1 "arith_operand" "rK")))]
  ""
  "*
{
  if (! REG_P (operands[0]))
    {
      cc_status.flags |= CC_REVERSED;
      return \"cmp %1,%0\";
    }
  return \"cmp %0,%1\";
}")

(define_insn "cmpdf"
  [(set (cc0)
	(minus:DF (match_operand:DF 0 "nonmemory_operand" "fG")
		  (match_operand:DF 1 "nonmemory_operand" "fG")))
   (use (reg:DF 32))]
  ""
  "*
{
  if ((cc_status.flags & (CC_F0_IS_0|CC_F1_IS_0)) == 0
      && (GET_CODE (operands[0]) == CONST_DOUBLE
	  || GET_CODE (operands[1]) == CONST_DOUBLE))
    make_f0_contain_0 (2);

  cc_status.flags |= CC_IN_FCCR | CC_F0_IS_0 | CC_F1_IS_0;

  if (GET_CODE (operands[0]) == CONST_DOUBLE)
    return \"fcmped %%f0,%1\;nop\";
  if (GET_CODE (operands[1]) == CONST_DOUBLE)
    return \"fcmped %0,%%f0\;nop\";
  return \"fcmped %0,%1\;nop\";
}")

(define_insn "cmpsf"
  [(set (cc0)
	(minus:SF (match_operand:SF 0 "nonmemory_operand" "fG")
		  (match_operand:SF 1 "nonmemory_operand" "fG")))
   (use (reg:DF 32))]
  ""
  "*
{
  if ((cc_status.flags & (CC_F0_IS_0)) == 0
      && (GET_CODE (operands[0]) == CONST_DOUBLE
	  || GET_CODE (operands[1]) == CONST_DOUBLE))
    make_f0_contain_0 (1);

  cc_status.flags |= CC_IN_FCCR | CC_F0_IS_0;

  if (GET_CODE (operands[0]) == CONST_DOUBLE)
    return \"fcmpes %%f0,%1\;nop\";
  if (GET_CODE (operands[1]) == CONST_DOUBLE)
    return \"fcmpes %0,%%f0\;nop\";
  return \"fcmpes %0,%1\;nop\";
}")

;; We have to have these because cse can optimize the previous patterns
;; into this one.

(define_insn "tstsi"
  [(set (cc0)
	(match_operand:SI 0 "register_operand" "r"))]
  ""
  "tst %0")

;; Need this to take a general operand because cse can make
;; a CONST which won't be in a register.
(define_insn ""
  [(set (cc0)
	(match_operand:SI 0 "immediate_operand" "i"))]
  ""
  "set %0,%%g1\;tst %%g1")

;; By default, operations don't set the condition codes.
;; These patterns allow cc's to be set, while doing some work

(define_insn ""
  [(set (cc0)
	(zero_extend:SI (subreg:QI (match_operand:SI 0 "register_operand" "r"))))]
  ""
  "andcc %0,0xff,%g0")

(define_insn ""
  [(set (cc0)
	(plus:SI (match_operand:SI 0 "register_operand" "r%")
		 (match_operand:SI 1 "arith_operand" "rK")))]
  ""
  "addcc %0,%1,%%g0")

(define_insn ""
  [(set (cc0)
	(plus:SI (match_operand:SI 0 "register_operand" "r%")
		 (match_operand:SI 1 "arith_operand" "rK")))
   (set (match_operand:SI 2 "register_operand" "=r")
	(plus:SI (match_dup 0) (match_dup 1)))]
  ""
  "addcc %0,%1,%2")

(define_insn ""
  [(set (cc0)
	(minus:SI (match_operand:SI 0 "register_operand" "r")
		  (match_operand:SI 1 "arith_operand" "rK")))
   (set (match_operand:SI 2 "register_operand" "=r")
	(minus:SI (match_dup 0) (match_dup 1)))]
  ""
  "subcc %0,%1,%2")

(define_insn ""
  [(set (cc0)
	(and:SI (match_operand:SI 0 "register_operand" "r%")
		(match_operand:SI 1 "arith_operand" "rK")))]
  ""
  "andcc %0,%1,%%g0")

(define_insn ""
  [(set (cc0)
	(and:SI (match_operand:SI 0 "register_operand" "r%")
		(match_operand:SI 1 "arith_operand" "rK")))
   (set (match_operand:SI 2 "register_operand" "=r")
	(and:SI (match_dup 0) (match_dup 1)))]
  ""
  "andcc %0,%1,%2")

(define_insn ""
  [(set (cc0)
	(and:SI (match_operand:SI 0 "register_operand" "r%")
		(not:SI (match_operand:SI 1 "arith_operand" "rK"))))]
  ""
  "andncc %0,%1,%%g0")

(define_insn ""
  [(set (cc0)
	(and:SI (match_operand:SI 0 "register_operand" "r%")
		(not:SI (match_operand:SI 1 "arith_operand" "rK"))))
   (set (match_operand:SI 2 "register_operand" "=r")
	(and:SI (match_dup 0) (not:SI (match_dup 1))))]
  ""
  "andncc %0,%1,%2")

(define_insn ""
  [(set (cc0)
	(ior:SI (match_operand:SI 0 "register_operand" "r%")
		(match_operand:SI 1 "arith_operand" "rK")))]
  ""
  "orcc %0,%1,%%g0")

(define_insn ""
  [(set (cc0)
	(ior:SI (match_operand:SI 0 "register_operand" "r%")
		(match_operand:SI 1 "arith_operand" "rK")))
   (set (match_operand:SI 2 "register_operand" "=r")
	(ior:SI (match_dup 0) (match_dup 1)))]
  ""
  "orcc %0,%1,%2")

(define_insn ""
  [(set (cc0)
	(ior:SI (match_operand:SI 0 "register_operand" "r%")
		(not:SI (match_operand:SI 1 "arith_operand" "rK"))))]
  ""
  "orncc %0,%1,%%g0")

(define_insn ""
  [(set (cc0)
	(ior:SI (match_operand:SI 0 "register_operand" "r%")
		(not:SI (match_operand:SI 1 "arith_operand" "rK"))))
   (set (match_operand:SI 2 "register_operand" "=r")
	(ior:SI (match_dup 0) (not:SI (match_dup 1))))]
  ""
  "orncc %0,%1,%2")

(define_insn ""
  [(set (cc0)
	(xor:SI (match_operand:SI 0 "register_operand" "r%")
		(match_operand:SI 1 "arith_operand" "rK")))]
  ""
  "xorcc %0,%1,%%g0")

(define_insn ""
  [(set (cc0)
	(xor:SI (match_operand:SI 0 "register_operand" "r%")
		(match_operand:SI 1 "arith_operand" "rK")))
   (set (match_operand:SI 2 "register_operand" "=r")
	(xor:SI (match_dup 0) (match_dup 1)))]
  ""
  "xorcc %0,%1,%2")

(define_insn ""
  [(set (cc0)
	(xor:SI (match_operand:SI 0 "register_operand" "r%")
		(not:SI (match_operand:SI 1 "arith_operand" "rK"))))]
  ""
  "xnorcc %0,%1,%%g0")

(define_insn ""
  [(set (cc0)
	(xor:SI (match_operand:SI 0 "register_operand" "r%")
		(not:SI (match_operand:SI 1 "arith_operand" "rK"))))
   (set (match_operand:SI 2 "register_operand" "=r")
	(xor:SI (match_dup 0) (not:SI (match_dup 1))))]
  ""
  "xnorcc %0,%1,%2")

(define_insn "tstdf"
  [(set (cc0)
	(match_operand:DF 0 "register_operand" "f"))
   (use (reg:DF 32))]
  ""
  "*
{
  if ((cc_status.flags & (CC_F0_IS_0|CC_F1_IS_0)) == 0)
    make_f0_contain_0 (2);

  cc_status.flags |= CC_IN_FCCR | CC_F0_IS_0 | CC_F1_IS_0;

  return \"fcmped %0,%%f0\;nop\";
}")

(define_insn "tstsf"
  [(set (cc0)
	(match_operand:SF 0 "register_operand" "f"))
   (use (reg:DF 32))]
  ""
  "*
{
  if ((cc_status.flags & (CC_F0_IS_0)) == 0)
    make_f0_contain_0 (1);

  cc_status.flags |= CC_IN_FCCR | CC_F0_IS_0;

  return \"fcmpes %0,%%f0\;nop\";
}")

;; Peepholes for set-on-integer-condition-code insns
;; These are disabled until combiner is taught how to use them correctly
;; (note that better code is generated that should be with these
;;  insns because they fool expand_expr into passing more stuff in
;;  registers than it is supposed to.  Should understand when this
;;  can be taken advantage of officially.)
(define_insn "seq"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(eq (cc0) (const_int 0)))]
  "0"
  "* { return output_scc_insn (GET_CODE (SET_SRC (PATTERN (insn))), operands); }")

(define_insn "sne"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ne (cc0) (const_int 0)))]
  "0"
  "* { return output_scc_insn (GET_CODE (SET_SRC (PATTERN (insn))), operands); }")

(define_insn "sgt"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(gt (cc0) (const_int 0)))]
  "0"
  "* { return output_scc_insn (GET_CODE (SET_SRC (PATTERN (insn))), operands); }")

(define_insn "sgtu"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(gtu (cc0) (const_int 0)))]
  "0"
  "* { return output_scc_insn (GET_CODE (SET_SRC (PATTERN (insn))), operands); }")

(define_insn "slt"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(lt (cc0) (const_int 0)))]
  "0"
  "* { return output_scc_insn (GET_CODE (SET_SRC (PATTERN (insn))), operands); }")

(define_insn "sltu"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ltu (cc0) (const_int 0)))]
  "0"
  "* { return output_scc_insn (GET_CODE (SET_SRC (PATTERN (insn))), operands); }")

(define_insn "sge"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ge (cc0) (const_int 0)))]
  "0"
  "* { return output_scc_insn (GET_CODE (SET_SRC (PATTERN (insn))), operands); }")

(define_insn "sgeu"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(geu (cc0) (const_int 0)))]
  "0"
  "* { return output_scc_insn (GET_CODE (SET_SRC (PATTERN (insn))), operands); }")

(define_insn "sle"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(le (cc0) (const_int 0)))]
  "0"
  "* { return output_scc_insn (GET_CODE (SET_SRC (PATTERN (insn))), operands); }")

(define_insn "sleu"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(leu (cc0) (const_int 0)))]
  "0"
  "* { return output_scc_insn (GET_CODE (SET_SRC (PATTERN (insn))), operands); }")

;; These control RTL generation for conditional jump insns
;; and match them for register allocation.

(define_insn "beq"
  [(set (pc)
	(if_then_else (eq (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fbe %l0\;nop\";
  return \"be %l0\;nop\";
}")

(define_insn "bne"
  [(set (pc)
	(if_then_else (ne (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fbne %l0\;nop\";
  return \"bne %l0\;nop\";
}")

(define_insn "bgt"
  [(set (pc)
	(if_then_else (gt (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fbg %l0\;nop\";
  return \"bgt %l0\;nop\";
}")

(define_insn "bgtu"
  [(set (pc)
	(if_then_else (gtu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    abort ();
  return \"bgu %l0\;nop\";
}")

(define_insn "blt"
  [(set (pc)
	(if_then_else (lt (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fbl %l0\;nop\";
  return \"bl %l0\;nop\";
}")

(define_insn "bltu"
  [(set (pc)
	(if_then_else (ltu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    abort ();
  return \"blu %l0\;nop\";
}")

(define_insn "bge"
  [(set (pc)
	(if_then_else (ge (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fbge %l0\;nop\";
  return \"bge %l0\;nop\";
}")

(define_insn "bgeu"
  [(set (pc)
	(if_then_else (geu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    abort ();
  return \"bgeu %l0\;nop\";
}")

(define_insn "ble"
  [(set (pc)
	(if_then_else (le (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fble %l0\;nop\";
  return \"ble %l0\;nop\";
}")

(define_insn "bleu"
  [(set (pc)
	(if_then_else (leu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    abort ();
  return \"bleu %l0\;nop\";
}")

;; These match inverted jump insns for register allocation.

(define_insn ""
  [(set (pc)
	(if_then_else (eq (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fbne %l0\;nop\";
  return \"bne %l0\;nop\";
}")

(define_insn ""
  [(set (pc)
	(if_then_else (ne (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fbe %l0\;nop\";
  return \"be %l0\;nop\";
}")

(define_insn ""
  [(set (pc)
	(if_then_else (gt (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fble %l0\;nop\";
  return \"ble %l0\;nop\";
}")

(define_insn ""
  [(set (pc)
	(if_then_else (gtu (cc0)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    abort ();
  return \"bleu %l0\;nop\";
}")

(define_insn ""
  [(set (pc)
	(if_then_else (lt (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fbge %l0\;nop\";
  return \"bge %l0\;nop\";
}")

(define_insn ""
  [(set (pc)
	(if_then_else (ltu (cc0)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    abort ();
  return \"bgeu %l0\;nop\";
}")

(define_insn ""
  [(set (pc)
	(if_then_else (ge (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fbl %l0\;nop\";
  return \"bl %l0\;nop\";
}")

(define_insn ""
  [(set (pc)
	(if_then_else (geu (cc0)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    abort ();
  return \"blu %l0\;nop\";
}")

(define_insn ""
  [(set (pc)
	(if_then_else (le (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    return \"fbg %l0\;nop\";
  return \"bg %l0\;nop\";
}")

(define_insn ""
  [(set (pc)
	(if_then_else (leu (cc0)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "*
{
  if (cc_status.flags & CC_IN_FCCR)
    abort ();
  return \"bgu %l0\;nop\";
}")

;; Move instructions

(define_insn "swapsi"
  [(set (match_operand:SI 0 "general_operand" "r,rm")
	(match_operand:SI 1 "general_operand" "m,r"))
   (set (match_dup 1) (match_dup 0))]
  ""
  "*
{
  if (! REG_P (operands[1]))
    return \"swap %1,%0\";
  if (REG_P (operands[0]))
    {
      if (REGNO (operands[0]) == REGNO (operands[1]))
	return \"\";
      return \"xor %0,%1,%0\;xor %1,%0,%1\;xor %0,%1,%0\";
    }
  return \"swap %0,%1\";
}")

(define_insn "movsi"
  [(set (match_operand:SI 0 "general_operand" "=r,m")
	(match_operand:SI 1 "general_operand" "rmi,rJ"))]
  ""
  "*
{
  if (GET_CODE (operands[0]) == MEM)
    return \"st %r1,%0\";
  if (GET_CODE (operands[1]) == MEM)
    return \"ld %1,%0\";
  if (REG_P (operands[1])
      || (GET_CODE (operands[1]) == CONST_INT
	  && SMALL_INT (operands[1])))
    return \"mov %1,%0\";
  return \"set %1,%0\";
}")

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=r")
	(mem:SI (plus:SI (match_operand:SI 1 "register_operand" "r")
			 (match_operand:SI 2 "register_operand" "r"))))]
  ""
  "ld [%1+%2],%0")

(define_insn "movhi"
  [(set (match_operand:HI 0 "general_operand" "=r,m")
	(match_operand:HI 1 "general_operand" "rmi,rJ"))]
  ""
  "*
{
  if (GET_CODE (operands[0]) == MEM)
    return \"sth %r1,%0\";
  if (GET_CODE (operands[1]) == MEM)
    return \"ldsh %1,%0\";
  if (REG_P (operands[1])
      || (GET_CODE (operands[1]) == CONST_INT
	  && SMALL_INT (operands[1])))
    return \"mov %1,%0\";
  return \"set %1,%0\";
}")

(define_insn "movqi"
  [(set (match_operand:QI 0 "general_operand" "=r,m")
	(match_operand:QI 1 "general_operand" "rmi,rJ"))]
  ""
  "*
{
  if (GET_CODE (operands[0]) == MEM)
    return \"stb %r1,%0\";
  if (GET_CODE (operands[1]) == MEM)
    return \"ldsb %1,%0\";
  return \"mov %1,%0\";
}")

;; The definition of this insn does not really explain what it does,
;; but it should suffice
;; that anything generated as this insn will be recognized as one
;; and that it won't successfully combine with anything.
(define_insn "movstrsi"
  [(set (match_operand:BLK 0 "general_operand" "=g")
	(match_operand:BLK 1 "general_operand" "g"))
   (use (match_operand:SI 2 "arith32_operand" "rn"))
   (clobber (reg:SI 8))
   (clobber (reg:SI 9))
   (clobber (reg:SI 10))]
  ""
  "* return output_block_move (operands);")

;; Floating point move insns

(define_insn ""
  [(set (match_operand:DF 0 "general_operand" "=fm")
	(mem:DF (match_operand:SI 1 "immediate_operand" "i")))]
  ""
  "*
{
  if (FP_REG_P (operands[0]))
    return \"set %1,%%g1\;ldd [%%g1],%0\";
  output_asm_insn (\"set %1,%%g1\;ldd [%%g1],%%f0\", operands);
  operands[1] = gen_rtx (REG, DFmode, 32);
  return output_fp_move_double (operands);
}")

(define_insn ""
  [(set (match_operand:SF 0 "general_operand" "=fm")
	(mem:SF (match_operand:SI 1 "immediate_operand" "i")))]
  ""
  "*
{
  if (FP_REG_P (operands[0]))
    return \"set %1,%%g1\;ld [%%g1],%0\";
  return \"set %1,%%g1\;ld [%%g1],%%f0\;st %%f0,%0\";
}")

;; This pattern forces (set (reg:DF ...) (const_double ...))
;; to be reloaded by putting the constant into memory.
;; It must come before the more general movdf pattern.
(define_insn ""
  [(set (match_operand:DF 0 "general_operand" "=r,f,o")
	(match_operand:DF 1 "" "mG,m,G"))]
  "GET_CODE (operands[1]) == CONST_DOUBLE"
  "*
{
  if (FP_REG_P (operands[0]))
    return output_fp_move_double (operands);
  if (operands[1] == dconst0_rtx && GET_CODE (operands[0]) == REG)
    {
      operands[1] = gen_rtx (REG, SImode, REGNO (operands[0]) + 1);
      return \"mov %%g0,%0\;mov %%g0,%1\";
    }
  if (operands[1] == dconst0_rtx && GET_CODE (operands[0]) == MEM)
    {
      operands[1] = adj_offsetable_operand (operands[0], 4);
      return \"st %%g0,%0\;st %%g0,%1\";
    }
  return output_move_double (operands);
}")

(define_insn "movdf"
  [(set (match_operand:DF 0 "general_operand" "=r,m,?f,?rm")
	(match_operand:DF 1 "general_operand" "rm,r,rfm,f"))]
  ""
  "*
{
  if (FP_REG_P (operands[0]) || FP_REG_P (operands[1]))
    return output_fp_move_double (operands);
  return output_move_double (operands);
}")

(define_insn "movdi"
  [(set (match_operand:DI 0 "general_operand" "=r,m,?f,?rm")
	(match_operand:DI 1 "general_operand" "rm,r,rfm,f"))]
  ""
  "*
{
  if (FP_REG_P (operands[0]) || FP_REG_P (operands[1]))
    return output_fp_move_double (operands);
  return output_move_double (operands);
}")

(define_insn "movsf"
  [(set (match_operand:SF 0 "general_operand" "=rf,m")
	(match_operand:SF 1 "general_operand" "rfm,rf"))]
  ""
  "*
{
  if (FP_REG_P (operands[0]))
    {
      if (FP_REG_P (operands[1]))
	return \"fmovs %1,%0\";
      if (GET_CODE (operands[1]) == REG)
        return \"st %1,[%%fp-4]\;ld [%%fp-4],%0\";
      return \"ld %1,%0\";
    }
  if (FP_REG_P (operands[1]))
    {
      if (GET_CODE (operands[0]) == REG)
	return \"st %1,[%%fp-4]\;ld [%%fp-4],%0\";
      return \"st %1,%0\";
    }
  if (GET_CODE (operands[0]) == MEM)
    return \"st %r1,%0\";
  if (GET_CODE (operands[1]) == MEM)
    return \"ld %1,%0\";
  return \"mov %1,%0\";
}")

;;- truncation instructions
(define_insn "truncsiqi2"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(truncate:QI
	 (match_operand:SI 1 "register_operand" "r")))]
  ""
  "*
{
  if (GET_CODE (operands[0]) == MEM)
    return \"stb %1,%0\";
  return \"mov %1,%0\";
}")

(define_insn "trunchiqi2"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(truncate:QI
	 (match_operand:HI 1 "register_operand" "r")))]
  ""
  "*
{
  if (GET_CODE (operands[0]) == MEM)
    return \"stb %1,%0\";
  return \"mov %1,%0\";
}")

(define_insn "truncsihi2"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(truncate:HI
	 (match_operand:SI 1 "register_operand" "r")))]
  ""
  "*
{
  if (GET_CODE (operands[0]) == MEM)
    return \"sth %1,%0\";
  return \"mov %1,%0\";
}")

;;- zero extension instructions

;; Note that the one starting from HImode comes before those for QImode
;; so that a constant operand will match HImode, not QImode.

(define_insn "zero_extendhisi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(zero_extend:SI
	 (match_operand:HI 1 "general_operand" "g")))]
  ""
  "*
{
  if (REG_P (operands[1]))
    return \"sll %1,0x10,%0\;srl %0,0x10,%0\";
  if (GET_CODE (operands[1]) == CONST_INT)
    abort ();
  return \"lduh %1,%0\";
}")

(define_insn "zero_extendqihi2"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(zero_extend:HI
	 (match_operand:QI 1 "general_operand" "g")))]
  ""
  "*
{
  if (REG_P (operands[1]))
    return \"and %1,0xff,%0\";
  if (GET_CODE (operands[1]) == CONST_INT)
    abort ();
  return \"ldub %1,%0\";
}")

(define_insn "zero_extendqisi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(zero_extend:SI
	 (match_operand:QI 1 "general_operand" "g")))]
  ""
  "*
{
  if (REG_P (operands[1]))
    return \"and %1,0xff,%0\";
  if (GET_CODE (operands[1]) == CONST_INT)
    abort ();
  return \"ldub %1,%0\";
}")

;;- sign extension instructions
;; Note that the one starting from HImode comes before those for QImode
;; so that a constant operand will match HImode, not QImode.

(define_insn "extendhisi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(sign_extend:SI
	 (match_operand:HI 1 "general_operand" "g")))]
  ""
  "*
{
  if (REG_P (operands[1]))
    return \"sll %1,0x10,%0\;sra %0,0x10,%0\";
  if (GET_CODE (operands[1]) == CONST_INT)
    abort ();
  return \"ldsh %1,%0\";
}")

(define_insn "extendqihi2"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(sign_extend:HI
	 (match_operand:QI 1 "general_operand" "g")))]
  ""
  "*
{
  if (REG_P (operands[1]))
    return \"sll %1,0x18,%0\;sra %0,0x18,%0\";
  if (GET_CODE (operands[1]) == CONST_INT)
    abort ();
  return \"ldsb %1,%0\";
}")

(define_insn "extendqisi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(sign_extend:SI
	 (match_operand:QI 1 "general_operand" "g")))]
  ""
  "*
{
  if (REG_P (operands[1]))
    return \"sll %1,0x18,%0\;sra %0,0x18,%0\";
  if (GET_CODE (operands[1]) == CONST_INT)
    abort ();
  return \"ldsb %1,%0\";
}")

;; Signed bitfield extractions come out looking like
;;	(shiftrt (shift (sign_extend <Y>) <C1>) <C2>)
;; which we expand poorly as four shift insns.
;; These patters yeild two shifts:
;;	(shiftrt (shift <Y> <C3>) <C4>)
(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ashiftrt:SI
	 (sign_extend:SI
	  (match_operand:QI 1 "register_operand" "r"))
	 (match_operand:SI 2 "small_int" "n")))]
  ""
  "sll %1,0x18,%0\;sra %0,0x18+%2,%0")

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ashiftrt:SI
	 (sign_extend:SI
	  (subreg:QI (ashift:SI (match_operand:SI 1 "register_operand" "r")
				(match_operand:SI 2 "small_int" "n")) 0))
	 (match_operand:SI 3 "small_int" "n")))]
  ""
  "sll %1,0x18+%2,%0\;sra %0,0x18+%3,%0")

;; Special patterns for optimizing bit-field instructions.

;; First two patterns are for bitfields that came from memory
;; testing only the high bit.  They work with old combiner.
;; @@ Actually, the second pattern does not work if we
;; @@ need to set the N bit.
(define_insn ""
  [(set (cc0)
	(zero_extend:SI (subreg:QI (lshiftrt:SI (match_operand:SI 0 "register_operand" "r")
						(const_int 7)) 0)))]
  "0"
  "andcc %0,128,%%g0")

(define_insn ""
  [(set (cc0)
	(sign_extend:SI (subreg:QI (ashiftrt:SI (match_operand:SI 0 "register_operand" "r")
						(const_int 7)) 0)))]
  "0"
  "andcc %0,128,%%g0")

;; next two patterns are good for bitfields coming from memory
;; (via pseudo-register) or from a register, though this optimization
;; is only good for values contained wholly within the bottom 13 bits
(define_insn ""
  [(set (cc0)
	(and:SI (lshiftrt:SI (match_operand:SI 0 "register_operand" "r")
			     (match_operand:SI 1 "small_int" "n"))
		(match_operand:SI 2 "small_int" "n")))]
  ""
  "andcc %0,%2<<%1,%%g0")

(define_insn ""
  [(set (cc0)
	(and:SI (ashiftrt:SI (match_operand:SI 0 "register_operand" "r")
			     (match_operand:SI 1 "small_int" "n"))
		(match_operand:SI 2 "small_int" "n")))]
  ""
  "andcc %0,%2<<%1,%%g0")

;; Conversions between float and double.

(define_insn "extendsfdf2"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(float_extend:DF
	 (match_operand:SF 1 "register_operand" "f")))]
  ""
  "fstod %1,%0")

(define_insn "truncdfsf2"
  [(set (match_operand:SF 0 "register_operand" "=f")
	(float_truncate:SF
	 (match_operand:DF 1 "register_operand" "f")))]
  ""
  "fdtos %1,%0")

;; Conversion between fixed point and floating point.
;; Note that among the fix-to-float insns
;; the ones that start with SImode come first.
;; That is so that an operand that is a CONST_INT
;; (and therefore lacks a specific machine mode).
;; will be recognized as SImode (which is always valid)
;; rather than as QImode or HImode.
  
;; This pattern forces (set (reg:SF ...) (float:SF (const_int ...)))
;; to be reloaded by putting the constant into memory.
;; It must come before the more general floatsisf2 pattern.
(define_insn ""
  [(set (match_operand:SF 0 "general_operand" "=f")
	(float:SF (match_operand 1 "" "m")))]
  "GET_CODE (operands[1]) == CONST_INT"
  "ld %1,%0\;fitos %0,%0")

(define_insn "floatsisf2"
  [(set (match_operand:SF 0 "general_operand" "=f")
	(float:SF (match_operand:SI 1 "general_operand" "rfm")))]
  ""
  "*
{
  if (GET_CODE (operands[1]) == MEM)
    return \"ld %1,%0\;fitos %0,%0\";
  else if (FP_REG_P (operands[1]))
    return \"fitos %1,%0\";
  else
    return \"st %1,[%%fp-4]\;ld [%%fp-4],%0\;fitos %0,%0\";
}")

;; This pattern forces (set (reg:DF ...) (float:DF (const_int ...)))
;; to be reloaded by putting the constant into memory.
;; It must come before the more general floatsidf2 pattern.
(define_insn ""
  [(set (match_operand:DF 0 "general_operand" "=f")
	(float:DF (match_operand 1 "" "m")))]
  "GET_CODE (operands[1]) == CONST_INT"
  "ld %1,%0\;fitod %0,%0")

(define_insn "floatsidf2"
  [(set (match_operand:DF 0 "general_operand" "=f")
	(float:DF (match_operand:SI 1 "general_operand" "rfm")))]
  ""
  "*
{
  if (GET_CODE (operands[1]) == MEM)
    return \"ld %1,%0\;fitod %0,%0\";
  else if (FP_REG_P (operands[1]))
    return \"fitod %1,%0\";
  else
    return \"st %1,[%%fp-4]\;ld [%%fp-4],%0\;fitod %0,%0\";
}")

;; Convert a float to an actual integer.
;; Truncation is performed as part of the conversion.
(define_insn "fix_truncsfsi2"
  [(set (match_operand:SI 0 "general_operand" "=rm")
	(fix:SI (fix:SF (match_operand:SF 1 "general_operand" "fm"))))]
  ""
  "*
{
  if (FP_REG_P (operands[1]))
    output_asm_insn (\"fstoi %1,%%f0\", operands);
  else
    output_asm_insn (\"ld %1,%%f0\;fstoi %%f0,%%f0\", operands);
  if (GET_CODE (operands[0]) == MEM)
    return \"st %%f0,%0\";
  else
    return \"st %%f0,[%%fp-4]\;ld [%%fp-4],%0\";
}")

(define_insn "fix_truncdfsi2"
  [(set (match_operand:SI 0 "general_operand" "=rm")
	(fix:SI (fix:DF (match_operand:DF 1 "general_operand" "fm"))))]
  ""
  "*
{
  if (FP_REG_P (operands[1]))
    output_asm_insn (\"fdtoi %1,%%f0\", operands);
  else
    {
      rtx xoperands[2];
      xoperands[0] = gen_rtx (REG, DFmode, 32);
      xoperands[1] = operands[1];
      output_fp_move_double (xoperands);
      output_asm_insn (\"fdtoi %%f0,%%f0\", 0);
    }
  if (GET_CODE (operands[0]) == MEM)
    return \"st %%f0,%0\";
  else
    return \"st %%f0,[%%fp-4]\;ld [%%fp-4],%0\";
}")

;;- arithmetic instructions

(define_insn "addsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI (match_operand:SI 1 "arith32_operand" "%r")
		 (match_operand:SI 2 "arith32_operand" "rn")))]
  ""
  "*
{
  if (REG_P (operands[2]))
    return \"add %1,%2,%0\";
  if (SMALL_INT (operands[2]))
    return \"add %1,%2,%0\";
  return \"set %2,%%g1\;add %1,%%g1,%0\";
}")

(define_insn "subsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI (match_operand:SI 1 "register_operand" "r")
		  (match_operand:SI 2 "arith32_operand" "rn")))]
  ""
  "*
{
  if (REG_P (operands[2]))
    return \"sub %1,%2,%0\";
  if (SMALL_INT (operands[2]))
    return \"sub %1,%2,%0\";
  return \"set %2,%%g1\;sub %1,%%g1,%0\";
}")

(define_expand "mulsi3"
  [(set (match_operand:SI 0 "register_operand" "r")
	(mult:SI (match_operand:SI 1 "general_operand" "")
		 (match_operand:SI 2 "general_operand" "")))]
  ""
  "
{
  rtx src;

  if (GET_CODE (operands[1]) == CONST_INT)
    if (GET_CODE (operands[2]) == CONST_INT)
      {
	emit_move_insn (operands[0],
			gen_rtx (CONST_INT, VOIDmode,
				 INTVAL (operands[1]) * INTVAL (operands[2])));
	DONE;
      }
    else
      src = gen_rtx (MULT, SImode,
		     copy_to_mode_reg (SImode, operands[2]),
		     operands[1]);
  else if (GET_CODE (operands[2]) == CONST_INT)
    src = gen_rtx (MULT, SImode,
		   copy_to_mode_reg (SImode, operands[1]),
		   operands[2]);
  else src = 0;

  if (src)
    emit_insn (gen_rtx (SET, VOIDmode, operands[0], src));
  else
    emit_insn (gen_rtx (PARALLEL, VOIDmode, gen_rtvec (5,
	       gen_rtx (SET, VOIDmode, operands[0],
			gen_rtx (MULT, SImode, operands[1], operands[2])),
	       gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 8)),
	       gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 9)),
	       gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 12)),
	       gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 13)))));
  DONE;
}")

(define_expand "umulsi3"
  [(set (match_operand:SI 0 "register_operand" "r")
	(umult:SI (match_operand:SI 1 "general_operand" "")
		  (match_operand:SI 2 "general_operand" "")))]
  ""
  "
{
  rtx src;

  if (GET_CODE (operands[1]) == CONST_INT)
    if (GET_CODE (operands[2]) == CONST_INT)
      {
	emit_move_insn (operands[0],
			gen_rtx (CONST_INT, VOIDmode,
				 (unsigned)INTVAL (operands[1]) * (unsigned)INTVAL (operands[2])));
	DONE;
      }
    else
      src = gen_rtx (UMULT, SImode,
		     copy_to_mode_reg (SImode, operands[2]),
		     operands[1]);
  else if (GET_CODE (operands[2]) == CONST_INT)
    src = gen_rtx (UMULT, SImode,
		   copy_to_mode_reg (SImode, operands[1]),
		   operands[2]);
  else src = 0;

  if (src)
    emit_insn (gen_rtx (SET, VOIDmode, operands[0], src));
  else
    emit_insn (gen_rtx (PARALLEL, VOIDmode, gen_rtvec (5,
	       gen_rtx (SET, VOIDmode, operands[0],
			gen_rtx (UMULT, SImode, operands[1], operands[2])),
	       gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 8)),
	       gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 9)),
	       gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 12)),
	       gen_rtx (CLOBBER, VOIDmode, gen_rtx (REG, SImode, 13)))));
  DONE;
}")

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=r")
	(mult:SI (match_operand:SI 1 "register_operand" "r")
		 (match_operand:SI 2 "immediate_operand" "n")))]
  ""
  "* return output_mul_by_constant (insn, operands, 0);")

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=r")
	(umult:SI (match_operand:SI 1 "register_operand" "r")
		  (match_operand:SI 2 "immediate_operand" "n")))]
  ""
  "* return output_mul_by_constant (insn, operands, 1);")

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=r")
	(mult:SI (match_operand:SI 1 "general_operand" "%r")
		 (match_operand:SI 2 "general_operand" "r")))
   (clobber (reg:SI 8))
   (clobber (reg:SI 9))
   (clobber (reg:SI 12))
   (clobber (reg:SI 13))]
  ""
  "* return output_mul_insn (operands, 0);")

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=r")
	(umult:SI (match_operand:SI 1 "general_operand" "%r")
		  (match_operand:SI 2 "general_operand" "r")))
   (clobber (reg:SI 8))
   (clobber (reg:SI 9))
   (clobber (reg:SI 12))
   (clobber (reg:SI 13))]
  ""
  "* return output_mul_insn (operands, 1);")

;; this pattern is needed because CSE may eliminate the multiplication,
;; but leave the clobbers behind.

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=r")
	(match_operand:SI 1 "register_operand" "g"))
   (clobber (reg:SI 8))
   (clobber (reg:SI 9))
   (clobber (reg:SI 12))
   (clobber (reg:SI 13))]
  ""
  "*
  return (GET_CODE (operands[1]) == CONST_INT
	  ? \"set %1,%0\"
	  : \"mov %1,%0\";
")

;;- and instructions (with compliment also)			   
(define_insn "andsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(and:SI (match_operand:SI 1 "arith32_operand" "%r")
		(match_operand:SI 2 "arith32_operand" "rn")))]
  ""
  "*
{
  if (REG_P (operands[2]))
    return \"and %1,%2,%0\";
  if (SMALL_INT (operands[2]))
    return \"and %1,%2,%0\";
  return \"set %2,%%g1\;and %1,%%g1,%0\";
}")

(define_insn "andcbsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(and:SI (match_operand:SI 1 "register_operand" "%r")
		(not:SI (match_operand:SI 2 "register_operand" "r"))))]
  ""
  "andn %1,%2,%0")

(define_insn "iorsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (match_operand:SI 1 "arith32_operand" "%r")
		(match_operand:SI 2 "arith32_operand" "rn")))]
  ""
  "*
{
  if (REG_P (operands[2]))
    return \"or %1,%2,%0\";
  if (SMALL_INT (operands[2]))
    return \"or %1,%2,%0\";
  return \"set %2,%%g1\;or %1,%%g1,%0\";
}")

(define_insn "iorcbsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (match_operand:SI 1 "register_operand" "%r")
		(not:SI (match_operand:SI 2 "register_operand" "r"))))]
  ""
  "orn %1,%2,%0")

(define_insn "xorsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(xor:SI (match_operand:SI 1 "arith32_operand" "%r")
		(match_operand:SI 2 "arith32_operand" "rn")))]
  ""
  "*
{
  if (REG_P (operands[2]))
    return \"xor %1,%2,%0\";
  if (SMALL_INT (operands[2]))
    return \"xor %1,%2,%0\";
  return \"set %2,%%g1\;xor %1,%%g1,%0\";
}")

(define_insn "xorcbsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(xor:SI (match_operand:SI 1 "register_operand" "%r")
		(not:SI (match_operand:SI 2 "register_operand" "r"))))]
  ""
  "xnor %1,%2,%0")

;; We cannot use the "neg" pseudo insn because the Sun assembler
;; does not know how to make it work for constants.
(define_insn "negsi2"
  [(set (match_operand:SI 0 "general_operand" "=r")
	(neg:SI (match_operand:SI 1 "arith_operand" "rK")))]
  ""
  "sub %%g0,%1,%0")

;; We cannot use the "not" pseudo insn because the Sun assembler
;; does not know how to make it work for constants.
(define_insn "one_cmplsi2"
  [(set (match_operand:SI 0 "general_operand" "=r")
	(not:SI (match_operand:SI 1 "arith_operand" "rK")))]
  ""
  "xnor %1,%%g0,%0")

;; Floating point arithmetic instructions.

(define_insn "adddf3"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(plus:DF (match_operand:DF 1 "register_operand" "f")
		 (match_operand:DF 2 "register_operand" "f")))]
  ""
  "faddd %1,%2,%0")

(define_insn "addsf3"
  [(set (match_operand:SF 0 "register_operand" "=f")
	(plus:SF (match_operand:SF 1 "register_operand" "f")
		 (match_operand:SF 2 "register_operand" "f")))]
  ""
  "fadds %1,%2,%0")

(define_insn "subdf3"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(minus:DF (match_operand:DF 1 "register_operand" "f")
		  (match_operand:DF 2 "register_operand" "f")))]
  ""
  "fsubd %1,%2,%0")

(define_insn "subsf3"
  [(set (match_operand:SF 0 "register_operand" "=f")
	(minus:SF (match_operand:SF 1 "register_operand" "f")
		  (match_operand:SF 2 "register_operand" "f")))]
  ""
  "fsubs %1,%2,%0")

(define_insn "muldf3"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(mult:DF (match_operand:DF 1 "register_operand" "f")
		 (match_operand:DF 2 "register_operand" "f")))]
  ""
  "fmuld %1,%2,%0")

(define_insn "mulsf3"
  [(set (match_operand:SF 0 "register_operand" "=f")
	(mult:SF (match_operand:SF 1 "register_operand" "f")
		 (match_operand:SF 2 "register_operand" "f")))]
  ""
  "fmuls %1,%2,%0")

(define_insn "divdf3"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(div:DF (match_operand:DF 1 "register_operand" "f")
		(match_operand:DF 2 "register_operand" "f")))]
  ""
  "fdivd %1,%2,%0")

(define_insn "divsf3"
  [(set (match_operand:SF 0 "register_operand" "=f")
	(div:SF (match_operand:SF 1 "register_operand" "f")
		(match_operand:SF 2 "register_operand" "f")))]
  ""
  "fdivs %1,%2,%0")

(define_insn "negdf2"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(neg:DF (match_operand:DF 1 "register_operand" "f")))]
  ""
  "*
{
  output_asm_insn (\"fnegs %1,%0\", operands);
  if (REGNO (operands[0]) != REGNO (operands[1]))
    {
      operands[0] = gen_rtx (REG, VOIDmode, REGNO (operands[0]) + 1);
      operands[1] = gen_rtx (REG, VOIDmode, REGNO (operands[1]) + 1);
      output_asm_insn (\"fmovs %1,%0\", operands);
    }
  return \"\";
}")

(define_insn "negsf2"
  [(set (match_operand:SF 0 "register_operand" "=f")
	(neg:SF (match_operand:SF 1 "register_operand" "f")))]
  ""
  "fnegs %1,%0")

(define_insn "absdf2"
  [(set (match_operand:DF 0 "register_operand" "=f")
	(abs:DF (match_operand:DF 1 "register_operand" "f")))]
  ""
  "*
{
  output_asm_insn (\"fabss %1,%0\", operands);
  if (REGNO (operands[0]) != REGNO (operands[1]))
    {
      operands[0] = gen_rtx (REG, VOIDmode, REGNO (operands[0]) + 1);
      operands[1] = gen_rtx (REG, VOIDmode, REGNO (operands[1]) + 1);
      output_asm_insn (\"fmovs %1,%0\", operands);
    }
  return \"\";
}")

(define_insn "abssf2"
  [(set (match_operand:SF 0 "register_operand" "=f")
	(abs:SF (match_operand:SF 1 "register_operand" "f")))]
  ""
  "fabss %1,%0")

;; Shift instructions

;; Optimized special case of shifting.
;; Must precede the general case.

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ashiftrt:SI (match_operand:SI 1 "memory_operand" "m")
		     (const_int 24)))]
  ""
  "ldsb %1,%0")

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "=r")
	(lshiftrt:SI (match_operand:SI 1 "memory_operand" "m")
		     (const_int 24)))]
  ""
  "ldub %1,%0")

;;- arithmetic shift instructions
(define_insn "ashlsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ashift:SI (match_operand:SI 1 "register_operand" "r")
		   (match_operand:SI 2 "arith32_operand" "rn")))]
  ""
  "*
{
  if (GET_CODE (operands[2]) == CONST_INT
      && INTVAL (operands[2]) >= 32)
    return \"mov %%g0,%0\";
  return \"sll %1,%2,%0\";
}")

(define_insn "ashrsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "r")
		     (match_operand:SI 2 "arith32_operand" "rn")))]
  ""
  "*
{
  if (GET_CODE (operands[2]) == CONST_INT
      && INTVAL (operands[2]) >= 32)
    return \"sra %1,31,%0\";
  return \"sra %1,%2,%0\";
}")

(define_insn "lshrsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(lshiftrt:SI (match_operand:SI 1 "register_operand" "r")
		   (match_operand:SI 2 "arith32_operand" "rn")))]
  ""
  "*
{
  if (GET_CODE (operands[2]) == CONST_INT
      && INTVAL (operands[2]) >= 32)
    return \"mov %%g0,%0\";
  return \"srl %1,%2,%0\";
}")

;; Unconditional and other jump instructions
(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  "b %l0\;nop")

;; Peephole optimizers recognize a few simple cases when delay insns are safe.
(define_peephole
  [(set (match_operand:SI 0 "register_operand" "=r")
	(match_operand:SI 1 "single_insn_src_p" "p"))
   (set (pc) (label_ref (match_operand 2 "" "")))]
  ""
  "* return output_delay_insn (\"b %l2\", operands, insn);")

(define_peephole
  [(set (match_operand:SI 0 "memory_operand" "=m")
	(match_operand:SI 1 "register_operand" "r"))
   (set (pc) (label_ref (match_operand 2 "" "")))]
  ""
  "* return output_delay_insn (\"b %l2\", operands, insn);")

(define_insn "tablejump"
  [(set (pc) (match_operand:SI 0 "register_operand" "r"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  "jmp %0\;nop")

(define_peephole
  [(set (match_operand:SI 0 "register_operand" "=r")
	(match_operand:SI 1 "single_insn_src_p" "p"))
   (set (pc) (match_operand:SI 2 "register_operand" "r"))
   (use (label_ref (match_operand 3 "" "")))]
  "REGNO (operands[0]) != REGNO (operands[2])"
  "* return output_delay_insn (\"jmp %2\", operands, insn);")

(define_peephole
  [(set (match_operand:SI 0 "memory_operand" "=m")
	(match_operand:SI 1 "register_operand" "r"))
   (set (pc) (match_operand:SI 2 "register_operand" "r"))
   (use (label_ref (match_operand 3 "" "")))]
  ""
  "* return output_delay_insn (\"jmp %2\", operands, insn);")

;;- jump to subroutine
(define_expand "call"
  [(call (match_operand:SI 0 "memory_operand" "m")
	 (match_operand 1 "" "i"))]
  ;; operand[2] is next_arg_register
  ""
  "
{
  rtx fn_rtx, nregs_rtx;

  if (TARGET_SUN_ASM && GET_CODE (XEXP (operands[0], 0)) == REG)
    {
      rtx g1_rtx = gen_rtx (REG, SImode, 1);
      emit_move_insn (g1_rtx, XEXP (operands[0], 0));
      fn_rtx = gen_rtx (MEM, SImode, g1_rtx);
    }
  else
    fn_rtx = operands[0];

  /* Count the number of parameter registers being used by this call.
     if that argument is NULL, it means we are using them all, which
     means 6 on the sparc.  */
#if 0
  if (operands[2])
    nregs_rtx = gen_rtx (CONST_INT, VOIDmode, REGNO (operands[2]) - 8);
  else
    nregs_rtx = gen_rtx (CONST_INT, VOIDmode, 6);
#else
  nregs_rtx = const0_rtx;
#endif

  emit_call_insn (gen_rtx (PARALLEL, VOIDmode, gen_rtvec (2,
			   gen_rtx (CALL, VOIDmode, fn_rtx, nregs_rtx),
			   gen_rtx (USE, VOIDmode, gen_rtx (REG, SImode, 31)))));
  DONE;
}")

(define_insn ""
  [(call (match_operand:SI 0 "memory_operand" "m")
	 (match_operand 1 "" "i"))
   (use (reg:SI 31))]
  ;;- Don't use operand 1 for most machines.
  ""
  "*
{
  /* strip the MEM.  */
  operands[0] = XEXP (operands[0], 0);
  return \"call %a0,%1\;nop\";
}")

(define_peephole
  [(set (match_operand:SI 0 "register_operand" "=r")
	(match_operand:SI 1 "single_insn_src_p" "p"))
   (parallel [(call (match_operand:SI 2 "memory_operand" "m")
		    (match_operand 3 "" "i"))
	      (use (reg:SI 31))])]
  ;;- Don't use operand 1 for most machines.
  "! reg_mentioned_p (operands[0], operands[2])"
  "*
{
  /* strip the MEM.  */
  operands[2] = XEXP (operands[2], 0);
  return output_delay_insn (\"call %a2,%3\", operands, insn);
}")

(define_peephole
  [(set (match_operand:SI 0 "memory_operand" "=m")
	(match_operand:SI 1 "register_operand" "r"))
   (parallel [(call (match_operand:SI 2 "memory_operand" "m")
		    (match_operand 3 "" "i"))
	      (use (reg:SI 31))])]
  ;;- Don't use operand 1 for most machines.
  ""
  "*
{
  /* strip the MEM.  */
  operands[2] = XEXP (operands[2], 0);
  return output_delay_insn (\"call %a2,%3\", operands, insn);
}")

(define_expand "call_value"
  [(set (match_operand 0 "" "rf")
	(call (match_operand:SI 1 "memory_operand" "m")
	      (match_operand 2 "" "i")))]
  ;; operand 3 is next_arg_register
  ""
  "
{
  rtx fn_rtx, nregs_rtx;
  rtvec vec;

  if (TARGET_SUN_ASM && GET_CODE (XEXP (operands[1], 0)) == REG)
    {
      rtx g1_rtx = gen_rtx (REG, SImode, 1);
      emit_move_insn (g1_rtx, XEXP (operands[1], 0));
      fn_rtx = gen_rtx (MEM, SImode, g1_rtx);
    }
  else
    fn_rtx = operands[1];

#if 0
  if (operands[3])
    nregs_rtx = gen_rtx (CONST_INT, VOIDmode, REGNO (operands[3]) - 8);
  else
    nregs_rtx = gen_rtx (CONST_INT, VOIDmode, 6);
#else
  nregs_rtx = const0_rtx;
#endif

  vec = gen_rtvec (2,
		   gen_rtx (SET, VOIDmode, operands[0],
			    gen_rtx (CALL, VOIDmode, fn_rtx, nregs_rtx)),
		   gen_rtx (USE, VOIDmode, gen_rtx (REG, SImode, 31)));

  emit_call_insn (gen_rtx (PARALLEL, VOIDmode, vec));
  DONE;
}")

(define_insn ""
  [(set (match_operand 0 "" "rf")
	(call (match_operand:SI 1 "memory_operand" "m")
	      (match_operand 2 "" "i")))
   (use (reg:SI 31))]
  ;;- Don't use operand 1 for most machines.
  ""
  "*
{
  /* strip the MEM.  */
  operands[1] = XEXP (operands[1], 0);
  return \"call %a1,%2\;nop\";
}")

(define_peephole
  [(set (match_operand:SI 0 "register_operand" "=r")
	(match_operand:SI 1 "single_insn_src_p" "p"))
   (parallel [(set (match_operand 2 "" "rf")
		   (call (match_operand:SI 3 "memory_operand" "m")
			 (match_operand 4 "" "i")))
	      (use (reg:SI 31))])]
  ;;- Don't use operand 1 for most machines.
  "! reg_mentioned_p (operands[0], operands[3])"
  "*
{
  /* strip the MEM.  */
  operands[3] = XEXP (operands[3], 0);
  return output_delay_insn (\"call %a3,%4\", operands, insn);
}")

(define_peephole
  [(set (match_operand:SI 0 "memory_operand" "=m")
	(match_operand:SI 1 "register_operand" "r"))
   (parallel [(set (match_operand 2 "" "rf")
		   (call (match_operand:SI 3 "memory_operand" "m")
			 (match_operand 4 "" "i")))
	      (use (reg:SI 31))])]
  ;;- Don't use operand 1 for most machines.
  ""
  "*
{
  /* strip the MEM.  */
  operands[3] = XEXP (operands[3], 0);
  return output_delay_insn (\"call %a3,%4\", operands, insn);
}")

;; A memory ref with constant address is not normally valid.
;; But it is valid in a call insns.  This pattern allows the
;; loading of the address to combine with the call.
(define_insn ""
  [(call (mem:SI (match_operand:SI 0 "" "i"))
	 (match_operand 1 "" "i"))
   (use (reg:SI 31))]
  "GET_CODE (operands[0]) == SYMBOL_REF"
  "call %0,%1\;nop")

(define_peephole
  [(set (match_operand:SI 0 "register_operand" "=r")
	(match_operand:SI 1 "single_insn_src_p" "p"))
   (parallel [(call (mem:SI (match_operand:SI 2 "" "i"))
		    (match_operand 3 "" "i"))
	      (use (reg:SI 31))])]
  "GET_CODE (operands[2]) == SYMBOL_REF"
  "* return output_delay_insn (\"call %2,%3\", operands, insn);")

(define_peephole
  [(set (match_operand:SI 0 "memory_operand" "=m")
	(match_operand:SI 1 "register_operand" "r"))
   (parallel [(call (mem:SI (match_operand:SI 2 "" "i"))
		    (match_operand 3 "" "i"))
	      (use (reg:SI 31))])]
  "GET_CODE (operands[2]) == SYMBOL_REF"
  "* return output_delay_insn (\"call %2,%3\", operands, insn);")

(define_insn ""
  [(set (match_operand 0 "" "rf")
	(call (mem:SI (match_operand:SI 1 "" "i"))
	      (match_operand 2 "" "i")))
   (use (reg:SI 31))]
  "GET_CODE (operands[1]) == SYMBOL_REF"
  "call %1,%2\;nop")

(define_peephole
  [(set (match_operand:SI 0 "register_operand" "=r")
	(match_operand:SI 1 "single_insn_src_p" "p"))
   (parallel [(set (match_operand 2 "" "rf")
		   (call (mem:SI (match_operand:SI 3 "" "i"))
			 (match_operand 4 "" "i")))
	      (use (reg:SI 31))])]
  "GET_CODE (operands[3]) == SYMBOL_REF"
  "* return output_delay_insn (\"call %3,%4\", operands, insn);")

(define_peephole
  [(set (match_operand:SI 0 "memory_operand" "=m")
	(match_operand:SI 1 "register_operand" "r"))
   (parallel [(set (match_operand 2 "" "rf")
		   (call (mem:SI (match_operand:SI 3 "" "i"))
			 (match_operand 4 "" "i")))
	      (use (reg:SI 31))])]
  "GET_CODE (operands[3]) == SYMBOL_REF"
  "* return output_delay_insn (\"call %3,%4\", operands, insn);")

(define_insn "return"
  [(return)]
  "! TARGET_EPILOGUE"
  "ret\;restore")

(define_peephole
  [(set (reg:SI 24)
	(match_operand:SI 0 "register_operand" "r"))
   (return)]
  ""
  "ret\;restore %0,0x0,%%o0")

(define_peephole
  [(set (reg:SI 24)
	(plus:SI (match_operand:SI 0 "register_operand" "r%")
		 (match_operand:SI 1 "arith_operand" "rI")))
   (return)]
  ""
  "ret\;restore %0,%1,%%o0")

(define_peephole
  [(set (reg:SI 24)
	(minus:SI (match_operand:SI 0 "register_operand" "r")
		  (match_operand:SI 1 "small_int" "I")))
   (return)]
  ""
  "ret\;restore %0,-(%1),%%o0")

;;- Local variables:
;;- mode:emacs-lisp
;;- comment-start: ";;- "
;;- eval: (set-syntax-table (copy-sequence (syntax-table)))
;;- eval: (modify-syntax-entry ?[ "(]")
;;- eval: (modify-syntax-entry ?] ")[")
;;- eval: (modify-syntax-entry ?{ "(}")
;;- eval: (modify-syntax-entry ?} "){")
;;- End:
