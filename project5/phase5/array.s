main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	$10, -4(%ebp)
	pushl	-4(%ebp)
	call	init_array
	pushl	-4(%ebp)
	call	print_array
	movl	%ebp, %esp
	popl	%ebp
	retl	
	.globl	main
	.comm	a, 40
