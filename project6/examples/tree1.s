insert:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$insert.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L0
	movl	$4, %eax
	imul	$3, %eax
	pushl	%eax
	call	malloc
	addl	$4, %esp
	movl	%eax, 8(%ebp)
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	12(%ebp), %ecx
	movl	%ecx, (%eax)
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	null, %ecx
	movl	%ecx, (%eax)
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	null, %ecx
	movl	%ecx, (%eax)
.L0:
	jmp	.L1
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	12(%ebp), %ecx
	cmpl	%eax, %ecx
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L2
	pushl	12(%ebp)
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	insert
	addl	$8, %esp
	movl	8(%ebp), %ecx
	addl	$4, %ecx
	movl	%eax, (%ecx)
.L2:
	jmp	.L3
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	12(%ebp), %ecx
	cmpl	%eax, %ecx
	setg	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L4
	pushl	12(%ebp)
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	insert
	addl	$8, %esp
	movl	8(%ebp), %ecx
	addl	$8, %ecx
	movl	%eax, (%ecx)
.L4:
.L3:
.L1:
	movl	8(%ebp), %eax
	jmp	insert.exit

insert.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	insert.size, 0
	.globl	insert

search:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$search.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L6
	movl	$0, %eax
	jmp	search.exit
.L6:
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	12(%ebp), %ecx
	cmpl	%eax, %ecx
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L8
	pushl	12(%ebp)
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	search
	addl	$8, %esp
	jmp	search.exit
.L8:
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	12(%ebp), %ecx
	cmpl	%eax, %ecx
	setg	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L10
	pushl	12(%ebp)
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	search
	addl	$8, %esp
	jmp	search.exit
.L10:
	movl	$1, %eax
	jmp	search.exit

search.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	search.size, 0
	.globl	search

preorder:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$preorder.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	je	.L12
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	pushl	%eax
	leal	.L14, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	preorder
	addl	$4, %esp
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	preorder
	addl	$4, %esp
.L12:

preorder.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	preorder.size, 0
	.globl	preorder

inorder:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$inorder.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	je	.L16
	movl	8(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	inorder
	addl	$4, %esp
	movl	8(%ebp), %eax
	addl	$0, %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	pushl	%eax
	leal	.L14, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	movl	8(%ebp), %eax
	addl	$8, %eax
	movl	(%eax), %eax
	pushl	%eax
	call	inorder
	addl	$4, %esp
.L16:

inorder.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	inorder.size, 0
	.globl	inorder

main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	movl	$0, %eax
	movl	%eax, -48(%ebp)
.L18:
	movl	-48(%ebp), %eax
	cmpl	$8, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L19
	leal	-44(%ebp), %eax
	movl	-48(%ebp), %ecx
	imul	$4, %ecx
	addl	%ecx, %eax
	movl	-48(%ebp), %ecx
	movl	%ecx, (%eax)
	movl	-48(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -48(%ebp)
	jmp	.L18
.L19:
	movl	null, %eax
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	addl	$28, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	addl	$16, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	addl	$4, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	addl	$0, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	addl	$20, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	addl	$8, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	addl	$12, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	addl	$24, %eax
	pushl	%eax
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -4(%ebp)
	leal	.L20, %eax
	pushl	%eax
	call	printf
	addl	$4, %esp
	pushl	-4(%ebp)
	call	preorder
	addl	$4, %esp
	leal	.L22, %eax
	pushl	%eax
	call	printf
	addl	$4, %esp
	pushl	-4(%ebp)
	call	inorder
	addl	$4, %esp

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 48
	.globl	main

	.comm	null, 4
	.data	
.L14:	.asciz	"%d\012"
.L22:	.asciz	"inorder traversal:\012"
.L20:	.asciz	"preorder traversal:\012"
