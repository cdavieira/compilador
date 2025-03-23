	.text
	.file	"1.c.ll"
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	$2, -16(%rbp)
	movl	$2, -8(%rbp)
	movl	$3, -12(%rbp)
	movl	$3, -4(%rbp)
	movl	-8(%rbp), %eax
	.p2align	4, 0x90
.LBB0_1:                                # %whilecond1
                                        # =>This Inner Loop Header: Depth=1
	cmpl	-4(%rbp), %eax
	jl	.LBB0_1
# %bb.2:                                # %whileesc1
	movb	$1, %al
	.p2align	4, 0x90
.LBB0_3:                                # %whilecond2
                                        # =>This Inner Loop Header: Depth=1
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movl	$1, -16(%rcx)
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movl	$1, -16(%rcx)
	testb	%al, %al
	jne	.LBB0_5
# %bb.4:                                # %whilebody2
                                        #   in Loop: Header=BB0_3 Depth=1
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movl	$0, -16(%rcx)
	jmp	.LBB0_3
.LBB0_5:                                # %whileesc2
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movl	$0, -16(%rax)
	xorl	%eax, %eax
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
