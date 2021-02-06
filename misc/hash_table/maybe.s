	.file	"maybe.cpp"
	.text
	.section	.text._ZNSt10error_codeC2Ev,"axG",@progbits,_ZNSt10error_codeC5Ev,comdat
	.align 2
	.weak	_ZNSt10error_codeC2Ev
	.type	_ZNSt10error_codeC2Ev, @function
_ZNSt10error_codeC2Ev:
.LFB1315:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$0, (%rax)
	call	_ZNSt3_V215system_categoryEv@PLT
	movq	-8(%rbp), %rdx
	movq	%rax, 8(%rdx)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1315:
	.size	_ZNSt10error_codeC2Ev, .-_ZNSt10error_codeC2Ev
	.weak	_ZNSt10error_codeC1Ev
	.set	_ZNSt10error_codeC1Ev,_ZNSt10error_codeC2Ev
	.section	.text._ZNKSt10error_codecvbEv,"axG",@progbits,_ZNKSt10error_codecvbEv,comdat
	.align 2
	.weak	_ZNKSt10error_codecvbEv
	.type	_ZNKSt10error_codecvbEv, @function
_ZNKSt10error_codecvbEv:
.LFB1327:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	setne	%al
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1327:
	.size	_ZNKSt10error_codecvbEv, .-_ZNKSt10error_codecvbEv
	.section	.text._ZN7FailureC2ERSt10error_code,"axG",@progbits,_ZN7FailureC5ERSt10error_code,comdat
	.align 2
	.weak	_ZN7FailureC2ERSt10error_code
	.type	_ZN7FailureC2ERSt10error_code, @function
_ZN7FailureC2ERSt10error_code:
.LFB1392:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rcx
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, (%rcx)
	movq	%rdx, 8(%rcx)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1392:
	.size	_ZN7FailureC2ERSt10error_code, .-_ZN7FailureC2ERSt10error_code
	.weak	_ZN7FailureC1ERSt10error_code
	.set	_ZN7FailureC1ERSt10error_code,_ZN7FailureC2ERSt10error_code
	.text
	.align 2
	.globl	_ZN5MaybeIvE8HasErrorEv
	.type	_ZN5MaybeIvE8HasErrorEv, @function
_ZN5MaybeIvE8HasErrorEv:
.LFB1402:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNKSt10error_codecvbEv
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1402:
	.size	_ZN5MaybeIvE8HasErrorEv, .-_ZN5MaybeIvE8HasErrorEv
	.align 2
	.globl	_ZNK5MaybeIvE5errorEv
	.type	_ZNK5MaybeIvE5errorEv, @function
_ZNK5MaybeIvE5errorEv:
.LFB1403:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	.cfi_offset 3, -24
	movq	%rdi, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdx
	movq	(%rax), %rax
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movl	%ecx, %eax
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1403:
	.size	_ZNK5MaybeIvE5errorEv, .-_ZNK5MaybeIvE5errorEv
	.globl	_ZN11make_result4FailESt10error_code
	.type	_ZN11make_result4FailESt10error_code, @function
_ZN11make_result4FailESt10error_code:
.LFB1404:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rdx, %rcx
	movq	%rsi, %rax
	movq	%rdi, %rdx
	movq	%rcx, %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	leaq	-32(%rbp), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	_ZN7FailureC1ERSt10error_code
	movq	-8(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1404:
	.size	_ZN11make_result4FailESt10error_code, .-_ZN11make_result4FailESt10error_code
	.section	.text._ZN5MaybeIvEC2Ev,"axG",@progbits,_ZN5MaybeIvEC5Ev,comdat
	.align 2
	.weak	_ZN5MaybeIvEC2Ev
	.type	_ZN5MaybeIvEC2Ev, @function
_ZN5MaybeIvEC2Ev:
.LFB1407:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSt10error_codeC1Ev
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1407:
	.size	_ZN5MaybeIvEC2Ev, .-_ZN5MaybeIvEC2Ev
	.weak	_ZN5MaybeIvEC1Ev
	.set	_ZN5MaybeIvEC1Ev,_ZN5MaybeIvEC2Ev
	.text
	.globl	_ZN11make_result2OkEv
	.type	_ZN11make_result2OkEv, @function
_ZN11make_result2OkEv:
.LFB1405:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	movl	$0, -48(%rbp)
	movq	$0, -40(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN5MaybeIvEC1Ev
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	movq	%rax, %rcx
	movq	%rdx, %rbx
	movq	-24(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L14
	call	__stack_chk_fail@PLT
.L14:
	movl	%ecx, %eax
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1405:
	.size	_ZN11make_result2OkEv, .-_ZN11make_result2OkEv
	.ident	"GCC: (GNU) 9.2.0"
	.section	.note.GNU-stack,"",@progbits
