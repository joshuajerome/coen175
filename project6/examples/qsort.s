readarray:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$readarray.size, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
