	.file	"linux.c"
# GNU C (Ubuntu/Linaro 4.6.1-9ubuntu3) version 4.6.1 (x86_64-linux-gnu)
#	compiled by GNU C version 4.6.1, GMP version 5.0.1, MPFR version 3.0.1-p3, MPC version 0.9
# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  -imultilib . -imultiarch x86_64-linux-gnu linux.c
# -mtune=generic -march=x86-64 -O2 -Wall -fverbose-asm -fstack-protector
# options enabled:  -fasynchronous-unwind-tables -fauto-inc-dec
# -fbranch-count-reg -fcaller-saves -fcombine-stack-adjustments -fcommon
# -fcompare-elim -fcprop-registers -fcrossjumping -fcse-follow-jumps
# -fdefer-pop -fdelete-null-pointer-checks -fdevirtualize -fdwarf2-cfi-asm
# -fearly-inlining -feliminate-unused-debug-types -fexpensive-optimizations
# -fforward-propagate -ffunction-cse -fgcse -fgcse-lm
# -fguess-branch-probability -fident -fif-conversion -fif-conversion2
# -findirect-inlining -finline -finline-functions-called-once
# -finline-small-functions -fipa-cp -fipa-profile -fipa-pure-const
# -fipa-reference -fipa-sra -fira-share-save-slots -fira-share-spill-slots
# -fivopts -fkeep-static-consts -fleading-underscore -fmath-errno
# -fmerge-constants -fmerge-debug-strings -fmove-loop-invariants
# -fomit-frame-pointer -foptimize-register-move -foptimize-sibling-calls
# -fpartial-inlining -fpeephole -fpeephole2 -fprefetch-loop-arrays
# -freg-struct-return -fregmove -freorder-blocks -freorder-functions
# -frerun-cse-after-loop -fsched-critical-path-heuristic
# -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock
# -fsched-last-insn-heuristic -fsched-rank-heuristic -fsched-spec
# -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fschedule-insns2
# -fshow-column -fsigned-zeros -fsplit-ivs-in-unroller -fsplit-wide-types
# -fstack-protector -fstrict-aliasing -fstrict-overflow
# -fstrict-volatile-bitfields -fthread-jumps -ftoplevel-reorder
# -ftrapping-math -ftree-bit-ccp -ftree-builtin-call-dce -ftree-ccp
# -ftree-ch -ftree-copy-prop -ftree-copyrename -ftree-cselim -ftree-dce
# -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre
# -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
# -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop -ftree-pre
# -ftree-pta -ftree-reassoc -ftree-scev-cprop -ftree-sink
# -ftree-slp-vectorize -ftree-sra -ftree-switch-conversion -ftree-ter
# -ftree-vect-loop-version -ftree-vrp -funit-at-a-time -funwind-tables
# -fvect-cost-model -fverbose-asm -fzee -fzero-initialized-in-bss
# -m128bit-long-double -m64 -m80387 -maccumulate-outgoing-args
# -malign-stringops -mfancy-math-387 -mfp-ret-in-387 -mglibc -mieee-fp
# -mmmx -mno-sse4 -mpush-args -mred-zone -msse -msse2 -mtls-direct-seg-refs

# Compiler executable checksum: 5dede901e38d49932d3155c3e888bca1

	.text
	.p2align 4,,15
	.type	__rb_rotate_left, @function
__rb_rotate_left:
.LFB4:
	movq	8(%rdi), %rax	# node_1(D)->rb_right, right
	movq	(%rdi), %rdx	# node_1(D)->rb_parent_color, D.2824
	movq	16(%rax), %r8	# right_2->rb_left, D.2825
	andq	$-4, %rdx	#, D.2824
	testq	%r8, %r8	# D.2825
	movq	%r8, 8(%rdi)	# D.2825, node_1(D)->rb_right
	je	.L2	#,
	movq	(%r8), %rcx	# MEM[(long unsigned int *)D.2825_6], tmp87
	andl	$3, %ecx	#, tmp87
	orq	%rdi, %rcx	# node, tmp87
	movq	%rcx, (%r8)	# tmp87, MEM[(long unsigned int *)D.2825_6]
.L2:
	movq	(%rax), %rcx	# MEM[(long unsigned int *)right_2], tmp90
	movq	%rdi, 16(%rax)	# node, right_2->rb_left
	andl	$3, %ecx	#, tmp90
	orq	%rdx, %rcx	# D.2824, tmp90
	testq	%rdx, %rdx	# D.2824
	movq	%rcx, (%rax)	# tmp90, MEM[(long unsigned int *)right_2]
	je	.L3	#,
	cmpq	%rdi, 16(%rdx)	# node, parent_5->rb_left
	je	.L7	#,
	movq	%rax, 8(%rdx)	# right, parent_5->rb_right
	movq	(%rdi), %rdx	# MEM[(long unsigned int *)node_1(D)], tmp93
	andl	$3, %edx	#, tmp93
	orq	%rax, %rdx	# right, tmp93
	movq	%rdx, (%rdi)	# tmp93, MEM[(long unsigned int *)node_1(D)]
	ret
	.p2align 4,,10
	.p2align 3
.L3:
	movq	(%rdi), %rdx	# MEM[(long unsigned int *)node_1(D)], tmp93
	movq	%rax, (%rsi)	# right, root_10(D)->rb_node
	andl	$3, %edx	#, tmp93
	orq	%rax, %rdx	# right, tmp93
	movq	%rdx, (%rdi)	# tmp93, MEM[(long unsigned int *)node_1(D)]
	ret
	.p2align 4,,10
	.p2align 3
.L7:
	movq	%rax, 16(%rdx)	# right, parent_5->rb_left
	movq	(%rdi), %rdx	# MEM[(long unsigned int *)node_1(D)], tmp93
	andl	$3, %edx	#, tmp93
	orq	%rax, %rdx	# right, tmp93
	movq	%rdx, (%rdi)	# tmp93, MEM[(long unsigned int *)node_1(D)]
	ret
.LFE4:
	.size	__rb_rotate_left, .-__rb_rotate_left
	.p2align 4,,15
	.type	__rb_rotate_right, @function
__rb_rotate_right:
.LFB5:
	movq	16(%rdi), %rax	# node_1(D)->rb_left, left
	movq	(%rdi), %rcx	# node_1(D)->rb_parent_color, D.2807
	movq	8(%rax), %r8	# left_2->rb_right, D.2808
	andq	$-4, %rcx	#, D.2807
	testq	%r8, %r8	# D.2808
	movq	%r8, 16(%rdi)	# D.2808, node_1(D)->rb_left
	je	.L9	#,
	movq	(%r8), %rdx	# MEM[(long unsigned int *)D.2808_6], tmp87
	andl	$3, %edx	#, tmp87
	orq	%rdi, %rdx	# node, tmp87
	movq	%rdx, (%r8)	# tmp87, MEM[(long unsigned int *)D.2808_6]
.L9:
	movq	(%rax), %rdx	# MEM[(long unsigned int *)left_2], tmp90
	movq	%rdi, 8(%rax)	# node, left_2->rb_right
	andl	$3, %edx	#, tmp90
	orq	%rcx, %rdx	# D.2807, tmp90
	testq	%rcx, %rcx	# D.2807
	movq	%rdx, (%rax)	# tmp90, MEM[(long unsigned int *)left_2]
	je	.L10	#,
	movq	(%rdi), %rdx	# MEM[(long unsigned int *)node_1(D)], tmp93
	movq	%rax, 8(%rcx)	# left, parent_5->rb_right
	andl	$3, %edx	#, tmp93
	orq	%rax, %rdx	# left, tmp93
	movq	%rdx, (%rdi)	# tmp93, MEM[(long unsigned int *)node_1(D)]
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	movq	(%rdi), %rdx	# MEM[(long unsigned int *)node_1(D)], tmp93
	movq	%rax, (%rsi)	# left, root_10(D)->rb_node
	andl	$3, %edx	#, tmp93
	orq	%rax, %rdx	# left, tmp93
	movq	%rdx, (%rdi)	# tmp93, MEM[(long unsigned int *)node_1(D)]
	ret
.LFE5:
	.size	__rb_rotate_right, .-__rb_rotate_right
	.p2align 4,,15
	.globl	rb_insert_color
	.type	rb_insert_color, @function
rb_insert_color:
.LFB6:
	pushq	%r14	#
	movq	%rsi, %r14	# root, root
	pushq	%r13	#
	movq	%rdi, %r13	# node, node
	pushq	%r12	#
	pushq	%rbp	#
	pushq	%rbx	#
	movq	(%rdi), %rbp	#* node, prephitmp.19
	.p2align 4,,10
	.p2align 3
WHILE_TEST:
	andq	$-4, %rbp	#, parent != NULL
	je	DONE	#,
.L26:
	movq	0(%rbp), %rax	# parent_9->rb_parent_color, prephitmp.19
	testb	$1, %al	#, rb_is_red(parent)
	jne	DONE	#,
	movq	%rax, %rbx	# prephitmp.19, gparent
	andq	$-4, %rbx	#, gparent

        # test which child we are
        # this fetch would be unnecessary if we tracked which child we are.
        # these two instructions could be `testb $1,%bpl`
	movq	16(%rbx), %r12	# gparent_14->rb_left, node
	cmpq	%rbp, %r12	# parent, node
	je	.L25	#,

	# uncle && rb_is_red(uncle)
	testq	%r12, %r12	# node
	je	.L19	#,
	movq	(%r12), %rdx	# node_15->rb_parent_color, D.2791
	testb	$1, %dl	#, D.2791
	jne	.L19	#,

	# this is the meat of the "loop" where we go up the tree splitting
	# 4-nodes. want to make the loop be just this basically.

	# rb_set_black(uncle)
	orq	$1, %rdx	#, tmp120
	movq	%rbx, %r13	# gparent, node
	movq	%rdx, (%r12)	# tmp120, node_15->rb_parent_color
	# rb_set_black(parent)
	orq	$1, 0(%rbp)	#, parent_9->rb_parent_color
	# rb_set_red(gparent)
	movq	(%rbx), %rbp	# gparent_14->rb_parent_color, prephitmp.19
	andq	$-2, %rbp	#, prephitmp.19
	movq	%rbp, (%rbx)	# prephitmp.19, gparent_14->rb_parent_color

	# part of the while condition has been transplanted here
	andq	$-4, %rbp	#, parent
	jne	.L26	#,
	.p2align 4,,10
	.p2align 3
DONE:
	# rb_set_black(root->rb_node)
	movq	(%r14), %rax	# root_27(D)->rb_node, D.2803
	orq	$1, (%rax)	#, D.2803_54->rb_parent_color
	popq	%rbx	#
	popq	%rbp	#
	popq	%r12	#
	popq	%r13	#
	popq	%r14	#
	ret
	.p2align 4,,10
	.p2align 3
.L19:
	movq	16(%rbp), %r12	# parent_9->rb_left, parent
	cmpq	%r13, %r12	# node, parent
	je	.L27	#,
.L20:
	orq	$1, %rax	#, tmp122
	movq	%r14, %rsi	# root,
	movq	%rbx, %rdi	# gparent,
	movq	%rax, 0(%rbp)	# tmp122, parent_5->rb_parent_color
	andq	$-2, (%rbx)	#, gparent_14->rb_parent_color
	call	__rb_rotate_left	#
	movq	0(%r13), %rbp	#* node, prephitmp.19
	jmp	WHILE_TEST	#
	.p2align 4,,10
	.p2align 3
.L25:
	movq	8(%rbx), %rax	# gparent_14->rb_right, uncle
	testq	%rax, %rax	# uncle
	je	.L16	#,
	movq	(%rax), %rdx	# uncle_16->rb_parent_color, D.2777
	testb	$1, %dl	#, D.2777
	jne	.L16	#,
	orq	$1, %rdx	#, tmp117
	movq	%rbx, %r13	# gparent, node
	movq	%rdx, (%rax)	# tmp117, uncle_16->rb_parent_color
	orq	$1, (%r12)	#, node_15->rb_parent_color
	movq	(%rbx), %rbp	# gparent_14->rb_parent_color, prephitmp.19
	andq	$-2, %rbp	#, prephitmp.19
	movq	%rbp, (%rbx)	# prephitmp.19, gparent_14->rb_parent_color
	jmp	WHILE_TEST	#
	.p2align 4,,10
	.p2align 3
.L16:
	movq	8(%r12), %rbp	# node_15->rb_right, parent
	cmpq	%r13, %rbp	# node, parent
	je	.L28	#,
	movq	%r12, %rbp	# node, parent
.L18:
	orq	$1, 0(%rbp)	#, parent_4->rb_parent_color
	movq	%r14, %rsi	# root,
	movq	%rbx, %rdi	# gparent,
	andq	$-2, (%rbx)	#, gparent_14->rb_parent_color
	call	__rb_rotate_right	#
	movq	0(%r13), %rbp	#* node, prephitmp.19
	jmp	WHILE_TEST	#
	.p2align 4,,10
	.p2align 3
.L27:
	movq	%rbp, %rdi	# parent,
	movq	%r14, %rsi	# root,
	call	__rb_rotate_right	#
	movq	0(%r13), %rax	# parent_45->rb_parent_color, prephitmp.19
	movq	%rbp, %r13	# parent, node
	movq	%r12, %rbp	# parent, parent
	jmp	.L20	#
.L28:
	movq	%r14, %rsi	# root,
	movq	%r12, %rdi	# node,
	movq	%r12, %r13	# node, node
	call	__rb_rotate_left	#
	jmp	.L18	#
.LFE6:
	.size	rb_insert_color, .-rb_insert_color
	.ident	"GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1"
	.section	.note.GNU-stack,"",@progbits
