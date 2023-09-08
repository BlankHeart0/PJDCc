section .text
	global main
	extern printf

main:
	common	x  8:8
	mov	r8, 7
	mov	r9, 9
	cmp	r8, r9
	setl	r8b
	and	r8, 255
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 7
	mov	r9, 9
	cmp	r8, r9
	setle	r8b
	and	r8, 255
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 7
	mov	r9, 9
	cmp	r8, r9
	setne	r8b
	and	r8, 255
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 7
	mov	r9, 7
	cmp	r8, r9
	sete	r8b
	and	r8, 255
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 7
	mov	r9, 7
	cmp	r8, r9
	setge	r8b
	and	r8, 255
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 7
	mov	r9, 7
	cmp	r8, r9
	setle	r8b
	and	r8, 255
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 9
	mov	r9, 7
	cmp	r8, r9
	setg	r8b
	and	r8, 255
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 9
	mov	r9, 7
	cmp	r8, r9
	setge	r8b
	and	r8, 255
	mov	[x], r8
	mov	r8, [x]
	push	rbp
	mov	rdi, format
	mov	rsi, r8
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r8, 9
	mov	r9, 7
	cmp	r8, r9
	setne	r8b
	and	r8, 255
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
