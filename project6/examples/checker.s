foo:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$foo.size, %esp
	movl	$5, %eax
	movl	%eax, x
	movl	$5, %eax
	movl	%eax, y
	movl	x, %eax
	movl	%eax, y
	movl	y, %eax
	movl	%eax, z
	movl	x, %eax
	addl	y, %eax
	movl	y, %eax
	addl	z, %eax
	movl	z, %eax
	addl	x, %eax
	movl	y, %eax
	addl	z, %eax
	movl	%eax, x
	movl	x, %eax
	addl	z, %eax
	movl	%eax, y
	movl	x, %eax
	addl	y, %eax
	movl	%eax, z
	movl	y, %eax
	subl	x, %eax
	movl	z, %eax
	subl	y, %eax
	movl	x, %eax
	subl	z, %eax
	movl	y, %eax
	subl	z, %eax
	movl	%eax, x
	movl	x, %eax
	subl	z, %eax
	movl	%eax, y
	movl	x, %eax
	subl	y, %eax
	movl	%eax, z
	movl	y, %eax
	imul	x, %eax
	movl	z, %eax
	imul	y, %eax
	movl	x, %eax
	imul	z, %eax
	movl	y, %eax
	imul	z, %eax
	movl	%eax, x
	movl	x, %eax
	imul	z, %eax
	movl	%eax, y
	movl	x, %eax
	imul	y, %eax
	movl	%eax, z
	movl	y, %eax
	movl	x, %ecx
	cltd	
	idivl	%ecx
	movl	z, %eax
	movl	y, %ecx
	cltd	
	idivl	%ecx
	movl	x, %eax
	movl	z, %ecx
	cltd	
	idivl	%ecx
	movl	y, %eax
	movl	z, %ecx
	cltd	
	idivl	%ecx
	movl	%eax, x
	movl	x, %eax
	movl	z, %ecx
	cltd	
	idivl	%ecx
	movl	%eax, y
	movl	x, %eax
	movl	y, %ecx
	cltd	
	idivl	%ecx
	movl	%eax, z
	movl	y, %eax
	movl	x, %ecx
	cltd	
	idivl	%ecx
	movl	z, %eax
	movl	y, %ecx
	cltd	
	idivl	%ecx
	movl	x, %eax
	movl	z, %ecx
	cltd	
	idivl	%ecx
	movl	y, %eax
	movl	z, %ecx
	cltd	
	idivl	%ecx
	movl	%edx, x
	movl	x, %eax
	movl	z, %ecx
	cltd	
	idivl	%ecx
	movl	%edx, y
	movl	x, %eax
	movl	y, %ecx
	cltd	
	idivl	%ecx
	movl	%edx, z
	movl	x, %eax
	cmpl	%eax, y
	setl	%al
	movbzl	%eax
	movl	x, %eax
	cmpl	%eax, y
	setg	%al
	movbzl	%eax
	movl	x, %eax
	cmpl	%eax, y
	setle	%al
	movbzl	%eax
	movl	x, %eax
	cmpl	%eax, y
	setge	%al
	movbzl	%eax
	movl	x, %eax
	cmpl	%eax, y
	sete	%al
	movbzl	%eax
	movl	x, %eax
	cmpl	%eax, y
	setne	%al
	movbzl	%eax
	movl	x, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	x, %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	x, %eax
	negl	%eax
	movl	x, %eax
	negl	%eax

foo.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	foo.size, 0
	.globl	foo

	.comm	x, 4
	.comm	y, 4
	.comm	z, 4
