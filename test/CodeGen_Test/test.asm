section .text
	global main
	extern printf

main:
	common	x  8:8
	mov	r8, 6
	mov	[x], r8
	common	t  8:8
	mov	r8, 7
	mov	[t], r8
	mov	r8, [x]
	mov	r9, [t]
	cmp	r8, r9
	setge	r8b
	and	r8, 255
	mov	r9, 0
	cmp	r8, r9
	sete	r8b
	and	r8, 255
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
