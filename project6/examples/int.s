main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	movl	$100, %eax
	movl	%eax, -4(%ebp)
	movl	$30, %eax
	movl	%eax, -8(%ebp)
	movl	$2, %eax
	movl	%eax, -12(%ebp)
	movl	-4(%ebp), %eax
	addl	-8(%ebp), %eax
	addl	-12(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-4(%ebp), %eax
	subl	-8(%ebp), %eax
	subl	-12(%ebp), %eax
	movl	%eax, -20(%ebp)
	movl	-4(%ebp), %eax
	imul	-8(%ebp), %eax
	imul	-12(%ebp), %eax
	movl	%eax, -24(%ebp)
	movl	-4(%ebp), %eax
	movl	-8(%ebp), %ecx
	cltd	
	idivl	%ecx
	addl	-12(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	-4(%ebp), %eax
	movl	-8(%ebp), %ecx
	cltd	
	idivl	%ecx
	subl	-12(%ebp), %edx
	movl	%edx, -32(%ebp)
	pushl	-16(%ebp)
	leal	.L0, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-20(%ebp)
	leal	.L0, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-24(%ebp)
	leal	.L0, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-28(%ebp)
	leal	.L0, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-32(%ebp)
	leal	.L0, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 32
	.globl	main

	.data	
.L0:	.asciz	"%d\012"
