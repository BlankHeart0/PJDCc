section .text
	global main
	extern printint

fred:
	push	rbp
	mov	rbp, rsp
	common	a  4:4
	common	b  1:1
	mov	r8, 10
	mov	[a], r8d
	xor	r8, r8
	mov	r8d, dword [a]
	mov	r8, 10
	mov	[b], r8b
	movzx	r8, byte [b]
	xor	r8, r8
	mov	r8d, dword [a]
	movzx	r9, byte [b]
	add	r8, r9
	mov	rax, r8
	jmp	L1
L1:
	pop	rbp
	ret

main:
	push	rbp
	mov	rbp, rsp
	common	result  4:4
	mov	r8, 10
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, 15
	mov	rdi, r8
	call	fred
	mov	r9, rax
	mov	[result], r9d
	xor	r8, r8
	mov	r8d, dword [result]
	xor	r8, r8
	mov	r8d, dword [result]
	mov	rdi, r8
	call	printint
	mov	r9, rax
	mov	r8, 15
	mov	rdi, r8
	call	fred
	mov	r9, rax
	mov	r8, 10
	add	r9, r8
	mov	rdi, r9
	call	printint
	mov	r8, rax
	mov	r8, 1
	mov	eax, r8d
	jmp	L2
L2:
	pop	rbp
	ret

