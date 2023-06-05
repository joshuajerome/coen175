main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	movl	$10, %eax
	movl	%eax, -4(%ebp)
	movl	$10, %eax
	movl	%eax, -8(%ebp)
	movl	-4(%ebp), %eax
	addl	-8(%ebp), %eax
	pushl	-4(%ebp)
	call	f
	addl	$4, %esp
	pushl	-8(%ebp)
	call	f
	addl	$4, %esp
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	setle	%al
	movzbl	%al, %eax
	pushl	%eax
	call	f
	addl	$4, %esp

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 12
	.globl	main

