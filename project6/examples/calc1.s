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
	jmp	.L1
.L0:
.L1:
.L2:
	pushl	c
	call	isspace
	addl	$4, %esp
	cmpl	$0, %eax
	je	.L5
	movl	c, %eax
	cmpl	$10, %eax
	setne	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L5
	movl	$1, %eax
	jmp	.L5
.L4:
	movl	$0, %eax
.L5:
	cmpl	$0, %eax
	je	.L3
	call	getchar
	movl	%eax, c
	jmp	.L2
.L3:
	pushl	c
	call	isdigit
	addl	$4, %esp
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L6
	movl	c, %eax
	movl	%eax, -8(%ebp)
	movl	$0, %eax
	movl	%eax, c
	movl	-8(%ebp), %eax
	jmp	lexan.exit
	jmp	.L7
.L6:
.L7:
	movl	$0, %eax
	movl	%eax, -4(%ebp)
.L8:
	pushl	c
	call	isdigit
	addl	$4, %esp
	cmpl	$0, %eax
	je	.L9
	movl	-4(%ebp), %eax
	imull	$10, %eax
	addl	c, %eax
	subl	$48, %eax
	movl	%eax, -4(%ebp)
	pushl	c
	call	getchar
	addl	$4, %esp
	movl	%eax, c
	jmp	.L8
.L9:
	movl	-4(%ebp), %eax
	movl	%eax, lexval
	movl	NUM, %eax
	jmp	lexan.exit

lexan.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	lexan.size, 8
	.globl	lexan

match:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$match.size, %esp
	movl	lookahead, %eax
	cmpl	8(%ebp), %eax
	setne	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L10
	pushl	lookahead
	leal	.L12, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	$1
	call	exit
	addl	$4, %esp
	jmp	.L11
.L10:
.L11:
	call	lexan
	movl	%eax, lookahead

match.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	match.size, 0
	.globl	match

factor:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$factor.size, %esp
	movl	lookahead, %eax
	cmpl	$40, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L14
	pushl	$40
	call	match
	addl	$4, %esp
	call	expr
	movl	%eax, -4(%ebp)
	pushl	$41
	call	match
	addl	$4, %esp
	movl	-4(%ebp), %eax
	jmp	factor.exit
	jmp	.L15
.L14:
.L15:
	movl	lexval, %eax
	movl	%eax, -4(%ebp)
	pushl	NUM
	call	match
	addl	$4, %esp
	movl	-4(%ebp), %eax
	jmp	factor.exit

factor.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	factor.size, 4
	.globl	factor

term:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$term.size, %esp
	call	factor
	movl	%eax, -4(%ebp)
.L16:
	movl	lookahead, %eax
	cmpl	$42, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	jne	.L18
	movl	lookahead, %eax
	cmpl	$47, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L19
.L18:
	movl	$1, %eax
	jmp	.L20
.L19:
	movl	$0, %eax
.L20:
	cmpl	$0, %eax
	je	.L17
	movl	lookahead, %eax
	cmpl	$42, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L21
	pushl	$42
	call	match
	addl	$4, %esp
	call	factor
	movl	-4(%ebp), %ecx
	imull	%eax, %ecx
	movl	%ecx, -4(%ebp)
	jmp	.L22
.L21:
	pushl	$47
	call	match
	addl	$4, %esp
	call	factor
	movl	%eax, -8(%ebp)
	movl	-4(%ebp), %eax
	movl	-8(%ebp), %ecx
	cltd	
	idivl	%ecx
	movl	%eax, -4(%ebp)
.L22:
	jmp	.L16
.L17:
	movl	-4(%ebp), %eax
	jmp	term.exit

term.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	term.size, 8
	.globl	term

expr:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$expr.size, %esp
	call	term
	movl	%eax, -4(%ebp)
.L23:
	movl	lookahead, %eax
	cmpl	$43, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	jne	.L25
	movl	lookahead, %eax
	cmpl	$45, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L26
.L25:
	movl	$1, %eax
	jmp	.L27
.L26:
	movl	$0, %eax
.L27:
	cmpl	$0, %eax
	je	.L24
	movl	lookahead, %eax
	cmpl	$43, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L28
	pushl	$43
	call	match
	addl	$4, %esp
	call	term
	movl	-4(%ebp), %ecx
	addl	%eax, %ecx
	movl	%ecx, -4(%ebp)
	jmp	.L29
.L28:
	pushl	$45
	call	match
	addl	$4, %esp
	call	term
	movl	-4(%ebp), %ecx
	subl	%eax, %ecx
	movl	%ecx, -4(%ebp)
.L29:
	jmp	.L23
.L24:
	movl	-4(%ebp), %eax
	jmp	expr.exit

expr.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	expr.size, 4
	.globl	expr

main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	movl	$256, %eax
	movl	%eax, NUM
	call	lexan
	movl	%eax, lookahead
.L30:
	movl	$1, %eax
	negl	%eax
	movl	lookahead, %ecx
	cmpl	%eax, %ecx
	setne	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L31
	call	expr
	movl	%eax, -4(%ebp)
	pushl	-4(%ebp)
	leal	.L32, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
.L34:
	movl	lookahead, %eax
	cmpl	$10, %eax
	sete	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L35
	pushl	$10
	call	match
	addl	$4, %esp
	jmp	.L34
.L35:
	jmp	.L30
.L31:

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 4
	.globl	main

	.comm	NUM, 4
	.comm	lookahead, 4
	.comm	c, 4
	.comm	lexval, 4
	.data	
.L32:	.asciz	"%d\012"
.L12:	.asciz	"syntax error at %d\012"
