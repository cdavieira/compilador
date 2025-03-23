	.text
	.file	"recursive.c.ll"
	.globl	mdc                             # -- Begin function mdc
	.p2align	4, 0x90
	.type	mdc,@function
mdc:                                    # @mdc
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	pushq	%rax
	.cfi_offset %rbx, -24
	movl	%edi, -16(%rbp)
	movl	%esi, -12(%rbp)
	cmpl	%edi, %esi
	jg	.LBB0_1
# %bb.4:                                # %ifesc1
	movl	-16(%rbp), %eax
	cltd
	idivl	-12(%rbp)
	movq	%rsp, %rax
	leaq	-16(%rax), %rbx
	movq	%rbx, %rsp
	movl	%edx, -16(%rax)
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movl	$0, -16(%rax)
	testl	%edx, %edx
	je	.LBB0_5
# %bb.6:                                # %ifesc2
	movl	(%rbx), %esi
	movl	-12(%rbp), %edx
	leaq	.L.str2(%rip), %rdi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	(%rbx), %edi
	movl	-12(%rbp), %esi
	jmp	.LBB0_2
.LBB0_1:                                # %ifif1
	movl	-16(%rbp), %esi
	movl	-12(%rbp), %edx
	leaq	.L.str0(%rip), %rdi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	-12(%rbp), %edi
	movl	-16(%rbp), %esi
.LBB0_2:                                # %ifif1
	callq	mdc@PLT
	jmp	.LBB0_3
.LBB0_5:                                # %ifif2
	movl	-16(%rbp), %esi
	movl	-12(%rbp), %edx
	leaq	.L.str1(%rip), %rdi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	-12(%rbp), %eax
.LBB0_3:                                # %ifif1
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	mdc, .Lfunc_end0-mdc
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	$77, 20(%rsp)
	movl	$77, 16(%rsp)
	movl	$14, 12(%rsp)
	movl	$14, 8(%rsp)
	movl	$77, %edi
	movl	$14, %esi
	callq	mdc@PLT
	movl	%eax, 4(%rsp)
	leaq	.L.str3(%rip), %rdi
	movl	%eax, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$0, (%rsp)
	xorl	%eax, %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str0,@object                 # @.str0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str0:
	.asciz	"1: %d %d\n"
	.size	.L.str0, 10

	.type	.L.str1,@object                 # @.str1
.L.str1:
	.asciz	"2: %d %d\n"
	.size	.L.str1, 10

	.type	.L.str2,@object                 # @.str2
.L.str2:
	.asciz	"3: %d %d\n"
	.size	.L.str2, 10

	.type	.L.str3,@object                 # @.str3
.L.str3:
	.asciz	"%d\n"
	.size	.L.str3, 4

	.section	".note.GNU-stack","",@progbits
