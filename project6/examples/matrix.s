allocate:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$allocate.size, %esp
	movl	8(%ebp), %eax
	imul	$4, %eax
	pushl	%eax
	call	malloc
	addl	$4, %esp
	movl	%eax, -8(%ebp)
	movl	$0, %eax
	movl	%eax, -4(%ebp)
.L0:
	movl	-4(%ebp), %eax
	cmpl	8(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L1
	movl	8(%ebp), %eax
	imul	$4, %eax
	pushl	%eax
	call	malloc
	addl	$4, %esp
	movl	-4(%ebp), %ecx
	imul	$4, %ecx
	movl	-8(%ebp), %edx
	addl	%ecx, %edx
	movl	%eax, (%edx)
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L0
.L1:
	movl	-8(%ebp), %eax
	jmp	allocate.exit

allocate.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	allocate.size, 8
	.globl	allocate

initialize:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$initialize.size, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
.L2:
	movl	-4(%ebp), %eax
	cmpl	12(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L3
	movl	$0, %eax
	movl	%eax, -8(%ebp)
.L4:
	movl	-8(%ebp), %eax
	cmpl	12(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L5
	movl	-4(%ebp), %eax
	addl	-8(%ebp), %eax
	movl	-4(%ebp), %ecx
	imul	$4, %ecx
	movl	8(%ebp), %edx
	addl	%ecx, %edx
	movl	(%edx), %edx
	movl	-8(%ebp), %ecx
	imul	$4, %ecx
	addl	%ecx, %edx
	movl	%eax, (%edx)
	movl	-8(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -8(%ebp)
	jmp	.L4
.L5:
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L2
.L3:

initialize.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	initialize.size, 8
	.globl	initialize

display:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$display.size, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
.L6:
	movl	-4(%ebp), %eax
	cmpl	12(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L7
	movl	$0, %eax
	movl	%eax, -8(%ebp)
.L8:
	movl	-8(%ebp), %eax
	cmpl	12(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L9
	movl	-4(%ebp), %eax
	imul	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	movl	%ecx, -12(%ebp)
	movl	-8(%ebp), %eax
	imul	$4, %eax
	movl	-12(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	pushl	%ecx
	leal	.L10, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	movl	-8(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -8(%ebp)
	jmp	.L8
.L9:
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	leal	.L12, %eax
	pushl	%eax
	call	printf
	addl	$4, %esp
	jmp	.L6
.L7:

display.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	display.size, 12
	.globl	display

deallocate:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$deallocate.size, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
.L14:
	movl	-4(%ebp), %eax
	cmpl	12(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L15
	movl	-4(%ebp), %eax
	imul	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	pushl	%ecx
	call	free
	addl	$4, %esp
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L14
.L15:
	pushl	8(%ebp)
	call	free
	addl	$4, %esp

deallocate.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	deallocate.size, 4
	.globl	deallocate

main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	leal	-8(%ebp), %eax
	pushl	%eax
	leal	.L16, %eax
	pushl	%eax
	call	scanf
	addl	$8, %esp
	pushl	-8(%ebp)
	call	allocate
	addl	$4, %esp
	movl	%eax, -4(%ebp)
	pushl	-8(%ebp)
	pushl	-4(%ebp)
	call	initialize
	addl	$8, %esp
	pushl	-8(%ebp)
	pushl	-4(%ebp)
	call	display
	addl	$8, %esp
	pushl	-8(%ebp)
	pushl	-4(%ebp)
	call	deallocate
	addl	$8, %esp

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 8
	.globl	main

	.data	
.L12:	.asciz	"\012"
.L16:	.asciz	"%d"
.L10:	.asciz	"%d "
