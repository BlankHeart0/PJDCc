	extern print_int
	extern print_char
	extern print_string

section	.data
global	a
	a:	dd	0

section	.data
global	b
	b:	db	0

section	.text
global	fred
fred:
	push	rbp
	mov	rbp, rsp
	mov	r8, 10
	mov	[a], r8d
	mov	r8, 10
	mov	[b], r8b
	xor	r8, r8
	mov	r8d, dword [a]
	movzx	r9, byte [b]
	add	r8, r9
	mov	rax, r8
	jmp	L1
L1:
	pop	rbp
	ret

section	.data
global	result
	result:	dd	0

section	.text
global	main
main:
	push	rbp
	mov	rbp, rsp
	mov	r8, 10
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	mov	r8, 15
	mov	rdi, r8
	call	fred
	mov	r9, rax
	mov	[result], r9d
	xor	r8, r8
	mov	r8d, dword [result]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	mov	r8, 15
	mov	rdi, r8
	call	fred
	mov	r9, rax
	mov	r8, 10
	add	r9, r8
	mov	rdi, r9
	call	print_int
	mov	r8, rax
	mov	r8, 1
	mov	eax, r8d
	jmp	L2
L2:
	pop	rbp
	ret

section .data

