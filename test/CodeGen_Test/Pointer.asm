	extern printint

section	.data
global	a
	a:	dq	0

section	.data
global	b
	b:	dq	0

section	.data
global	a_ptr
	a_ptr:	dq	0

section	.data
global	b_ptr
	b_ptr:	dq	0

section	.text
global	main
main:
	push	rbp
	mov	rbp, rsp
	mov	r8, 12
	mov	[a], r8
	mov	r8, [a]
	mov	r8, 18
	mov	[b], r8
	mov	r8, [b]
	mov	r8, [a]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, [b]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, 111111111
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, a
	mov	[a_ptr], r8
	mov	r8, [a_ptr]
	mov	r8, [a_ptr]
	mov	r8, [r8]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, a
	mov	r9, 1
	sal	r9, 3
	add	r8, r9
	mov	[a_ptr], r8
	mov	r8, [a_ptr]
	mov	r8, [a_ptr]
	mov	r8, [r8]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, a
	mov	[a_ptr], r8
	mov	r8, [a_ptr]
	mov	r8, [a_ptr]
	mov	r9, 1
	sal	r9, 3
	add	r8, r9
	mov	r8, [r8]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, 111111111
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, b
	mov	[b_ptr], r8
	mov	r8, [b_ptr]
	mov	r8, [b_ptr]
	mov	r8, [r8]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, b
	mov	r9, 1
	sal	r9, 3
	sub	r8, r9
	mov	[b_ptr], r8
	mov	r8, [b_ptr]
	mov	r8, [b_ptr]
	mov	r8, [r8]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, b
	mov	[b_ptr], r8
	mov	r8, [b_ptr]
	mov	r8, [b_ptr]
	mov	r9, 1
	sal	r9, 3
	sub	r8, r9
	mov	r8, [r8]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, 0
	mov	eax, r8d
	jmp	L1
L1:
	pop	rbp
	ret

