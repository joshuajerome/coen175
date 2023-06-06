assignment:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$assignment.size, %esp
	movl	$5, %eax
	movl	%eax, x
	movl	$5, %eax
	movl	%eax, y
	movl	x, %eax
	movl	%eax, y
	movl	y, %eax
	movl	%eax, z

assignment.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	assignment.size, 0
	.globl	assignment

add:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$add.size, %esp
	movl	y, %eax
	addl	z, %eax
	movl	%eax, x
	movl	x, %eax
	addl	z, %eax
	movl	%eax, y
	movl	x, %eax
	addl	y, %eax
	movl	%eax, z

add.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	add.size, 0
	.globl	add

sub:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$sub.size, %esp
	movl	y, %eax
	subl	z, %eax
	movl	%eax, x
	movl	x, %eax
	subl	z, %eax
	movl	%eax, y
	movl	x, %eax
	subl	y, %eax
	movl	%eax, z

sub.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	sub.size, 0
	.globl	sub

mul:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$mul.size, %esp
	movl	x, %eax
	imul	$2, %eax
	movl	%eax, x
	movl	y, %eax
	imul	$2, %eax
	movl	%eax, y
	movl	z, %eax
	imul	$2, %eax
	movl	%eax, z

mul.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	mul.size, 0
	.globl	mul

div:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$div.size, %esp
	movl	x, %eax
	movl	$2, %ecx
	cltd	
	idivl	%ecx
	movl	%eax, x
	movl	y, %eax
	movl	$2, %ecx
	cltd	
	idivl	%ecx
	movl	%eax, y
	movl	z, %eax
	movl	$2, %ecx
	cltd	
	idivl	%ecx
	movl	%eax, z

div.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	div.size, 0
	.globl	div

rem:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$rem.size, %esp
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

rem.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	rem.size, 0
	.globl	rem

relational:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$relational.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L0
	movl	12(%ebp), %eax
	cmpl	16(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)
.L0:
	jmp	.L1
	movl	8(%ebp), %eax
	cmpl	$1, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L2
	movl	12(%ebp), %eax
	cmpl	16(%ebp), %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)
.L2:
	jmp	.L3
	movl	8(%ebp), %eax
	cmpl	$2, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L4
	movl	12(%ebp), %eax
	cmpl	16(%ebp), %eax
	setle	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)
.L4:
	jmp	.L5
	movl	8(%ebp), %eax
	cmpl	$3, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L6
	movl	12(%ebp), %eax
	cmpl	16(%ebp), %eax
	setge	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)
.L6:
	jmp	.L7
	movl	8(%ebp), %eax
	cmpl	$4, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L8
	movl	12(%ebp), %eax
	cmpl	16(%ebp), %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)
.L8:
	jmp	.L9
	movl	8(%ebp), %eax
	cmpl	$5, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L10
	movl	12(%ebp), %eax
	cmpl	16(%ebp), %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)
.L10:
	jmp	.L11
	movl	$1, %eax
	negl	%eax
	movl	%eax, -4(%ebp)
.L11:
.L9:
.L7:
.L5:
.L3:
.L1:
	movl	-4(%ebp), %eax
	jmp	relational.exit

relational.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	relational.size, 4
	.globl	relational

not:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$not.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	jmp	not.exit

not.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	not.size, 0
	.globl	not

negate:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$negate.size, %esp
	movl	8(%ebp), %eax
	negl	%eax
	jmp	negate.exit

negate.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	negate.size, 0
	.globl	negate

main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	call	assignment
	pushl	z
	pushl	y
	pushl	x
	call	printAll
	addl	$12, %esp
	call	add
	pushl	z
	pushl	y
	pushl	x
	call	printAll
	addl	$12, %esp
	call	sub
	pushl	z
	pushl	y
	pushl	x
	call	printAll
	addl	$12, %esp
	call	mul
	pushl	z
	pushl	y
	pushl	x
	call	printAll
	addl	$12, %esp
	call	div
	pushl	z
	pushl	y
	pushl	x
	call	printAll
	addl	$12, %esp
	call	rem
	pushl	z
	pushl	y
	pushl	x
	call	printAll
	addl	$12, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
.L12:
	movl	-4(%ebp), %eax
	cmpl	$6, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L13
	pushl	y
	pushl	x
	pushl	-4(%ebp)
	call	relational
	addl	$12, %esp
	pushl	%eax
	call	printBool
	addl	$4, %esp
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L12
.L13:
	pushl	x
	call	not
	addl	$4, %esp
	pushl	%eax
	call	print
	addl	$4, %esp
	pushl	x
	call	not
	addl	$4, %esp
	pushl	%eax
	call	print
	addl	$4, %esp
	pushl	x
	call	negate
	addl	$4, %esp
	pushl	%eax
	call	print
	addl	$4, %esp
	pushl	x
	call	negate
	addl	$4, %esp
	pushl	%eax
	call	print
	addl	$4, %esp
	movl	$0, %eax
	jmp	main.exit

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 4
	.globl	main

	.comm	x, 4
	.comm	y, 4
	.comm	z, 4
	.data	
