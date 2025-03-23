	.text
	.file	"globalvar.c.ll"
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2, 0x0                          # -- Begin function main
.LCPI0_0:
	.long	0x40600000                      # float 3.5
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$1080033280, 4(%rsp)            # imm = 0x40600000
	movl	$1080033280, (%rsp)             # imm = 0x40600000
	movq	g2@GOTPCREL(%rip), %rax
	cvtsi2ssl	(%rax), %xmm0
	addss	.LCPI0_0(%rip), %xmm0
	cvtss2sd	%xmm0, %xmm0
	leaq	.L.str0(%rip), %rdi
	movb	$1, %al
	callq	printf@PLT
	movq	g1@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str0,@object                 # @.str0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str0:
	.asciz	"%f\n"
	.size	.L.str0, 4

	.type	g1,@object                      # @g1
	.bss
	.globl	g1
	.p2align	2, 0x0
g1:
	.long	0                               # 0x0
	.size	g1, 4

	.type	g2,@object                      # @g2
	.data
	.globl	g2
	.p2align	2, 0x0
g2:
	.long	1                               # 0x1
	.size	g2, 4

	.section	".note.GNU-stack","",@progbits
