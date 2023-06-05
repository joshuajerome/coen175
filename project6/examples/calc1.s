lexan:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$lexan.size, %esp
	movl	$0, %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	%eax, lexval

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
	movl	lexval, %eax
	movl	%eax, -4(%ebp)
	pushl	NUM
	call	match
	addl	$4, %esp

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

term.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	term.size, 4
	.globl	term

expr:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$expr.size, %esp
	call	term
	movl	%eax, -4(%ebp)

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
