section .text
	global main
	extern printf

main:
	mov	r8, 12
	mov	r9, 3
	imul	r8, r9
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 18
	mov	r9, 2
	mov	r10, 4
	imul	r9, r10
	sub	r8, r9
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 1
	mov	r9, 2
	add	r8, r9
	mov	r9, 9
	add	r8, r9
	mov	r9, 5
	mov	r10, 2
	mov	rax, r9
	cqo
	idiv	r10
	mov	r9, rax
	sub	r8, r9
	mov	r9, 3
	mov	r10, 5
	imul	r9, r10
	add	r8, r9
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	common	fred  8:8
	common	jim  8:8
	mov	r8, 5
	mov	[fred], r8
	mov	r8, 12
	mov	[jim], r8
	mov	r8, [fred]
	mov	r9, [jim]
	add	r8, r9
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	common	x  8:8
	mov	r8, 1
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, [x]
	mov	r9, 1
	add	r8, r9
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, [x]
	mov	r9, 1
	add	r8, r9
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, [x]
	mov	r9, 1
	add	r8, r9
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, [x]
	mov	r9, 1
	add	r8, r9
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp

exit:
	mov	rax, 0
	ret

section .data
	format: db "%d",0XA,0
