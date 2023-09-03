section .text
	global main
	extern printf

main:
	mov	r8, 2
	mov	r9, 3
	mov	r10, 5
	imul	r9, r10
	add	r8, r9
	mov	r9, 8
	mov	r10, 3
	mov	rax, r9
	cqo
	idiv	r10
	mov	r9, rax
	sub	r8, r9
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 1
	mov	r9, 1
	add	r8, r9
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 12314
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 90
	mov	r9, 90
	sub	r8, r9
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 90
	mov	r9, 109
	sub	r8, r9
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
