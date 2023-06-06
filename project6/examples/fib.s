fib:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$fib.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	jne	.L2
	movl	8(%ebp), %eax
	cmpl	$1, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L3
.L2:
