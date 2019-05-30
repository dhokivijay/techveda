	.file	"inline.c"
	.text
.globl max
	.type	max, @function
max:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	cmpl	12(%ebp), %eax
	jle	.L2
	movl	8(%ebp), %eax
	jmp	.L3
.L2:
	movl	12(%ebp), %eax
.L3:
	popl	%ebp
	ret
	.size	max, .-max
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$10, -16(%ebp)
	movl	$20, -12(%ebp)
	movl	-16(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	-12(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.4 20100630 (Red Hat 4.4.4-10)"
	.section	.note.GNU-stack,"",@progbits
