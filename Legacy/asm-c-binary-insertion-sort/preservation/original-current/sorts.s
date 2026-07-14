	.file	"sorts.c"
	.text
	.globl	binarySearch
	.type	binarySearch, @function
binarySearch:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	%edx, -32(%rbp)
	movl	-32(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jg	.L2
	movl	16(%rbp), %edx
	movl	-28(%rbp), %eax
	cltq
	salq	$5, %rax
	movq	%rax, %rcx
	movq	-24(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jle	.L3
	movl	-28(%rbp), %eax
	addl	$1, %eax
	jmp	.L5
.L3:
	movl	-28(%rbp), %eax
	jmp	.L5
.L2:
	movl	-28(%rbp), %edx
	movl	-32(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	movl	%eax, -4(%rbp)
	movl	16(%rbp), %edx
	movl	-4(%rbp), %eax
	cltq
	salq	$5, %rax
	movq	%rax, %rcx
	movq	-24(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jne	.L6
	movl	-4(%rbp), %eax
	addl	$1, %eax
	jmp	.L5
.L6:
	movl	16(%rbp), %edx
	movl	-4(%rbp), %eax
	cltq
	salq	$5, %rax
	movq	%rax, %rcx
	movq	-24(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jle	.L7
	movl	-4(%rbp), %eax
	leal	1(%rax), %ecx
	movl	-32(%rbp), %edx
	movq	-24(%rbp), %rax
	pushq	40(%rbp)
	pushq	32(%rbp)
	pushq	24(%rbp)
	pushq	16(%rbp)
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	binarySearch
	addq	$32, %rsp
	jmp	.L5
.L7:
	movl	-4(%rbp), %eax
	leal	-1(%rax), %edx
	movl	-28(%rbp), %ecx
	movq	-24(%rbp), %rax
	pushq	40(%rbp)
	pushq	32(%rbp)
	pushq	24(%rbp)
	pushq	16(%rbp)
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	binarySearch
	addq	$32, %rsp
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	binarySearch, .-binarySearch
	.globl	insertionSort
	.type	insertionSort, @function
insertionSort:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movl	%esi, -60(%rbp)
	movl	$1, -44(%rbp)
	jmp	.L9
.L12:
	movl	-44(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -40(%rbp)
	movl	-44(%rbp), %eax
	cltq
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movq	(%rcx), %rax
	movq	8(%rcx), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	16(%rcx), %rax
	movq	24(%rcx), %rdx
	movq	%rax, -16(%rbp)
	movq	%rdx, -8(%rbp)
	movl	-40(%rbp), %edx
	movq	-56(%rbp), %rax
	pushq	-8(%rbp)
	pushq	-16(%rbp)
	pushq	-24(%rbp)
	pushq	-32(%rbp)
	movl	$0, %esi
	movq	%rax, %rdi
	call	binarySearch
	addq	$32, %rsp
	movl	%eax, -36(%rbp)
	jmp	.L10
.L11:
	movl	-40(%rbp), %eax
	cltq
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	leaq	(%rdx,%rax), %rsi
	movl	-40(%rbp), %eax
	cltq
	addq	$1, %rax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movq	(%rsi), %rax
	movq	8(%rsi), %rdx
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	movq	16(%rsi), %rax
	movq	24(%rsi), %rdx
	movq	%rax, 16(%rcx)
	movq	%rdx, 24(%rcx)
	subl	$1, -40(%rbp)
.L10:
	movl	-40(%rbp), %eax
	cmpl	-36(%rbp), %eax
	jge	.L11
	movl	-40(%rbp), %eax
	cltq
	addq	$1, %rax
	salq	$5, %rax
	movq	%rax, %rdx
	movq	-56(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	movq	-16(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rax, 16(%rcx)
	movq	%rdx, 24(%rcx)
	addl	$1, -44(%rbp)
.L9:
	movl	-44(%rbp), %eax
	cmpl	-60(%rbp), %eax
	jl	.L12
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	insertionSort, .-insertionSort
	.globl	TaskCars
	.type	TaskCars, @function
TaskCars:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$10, -12(%rbp)
	movl	-12(%rbp), %edx
	movq	-8(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	insertionSort
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	TaskCars, .-TaskCars
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	call	TaskCars
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
