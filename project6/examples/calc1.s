lexan:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$lexan.size, %esp
	movl	c, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L0
	call	getchar
	movl	%eax, c
.L0:
