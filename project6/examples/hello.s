main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	leal	.L0, %eax
	pushl	%eax
	call	printf
	addl	$4, %esp

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 0
	.globl	main

	.data	
.L0:	.asciz	"hello world\012"
