	extern print_int
	extern print_long
	extern print_char
	extern print_string

section .data
global	a
	a:	dd	0

global	b
	b:	dd	0

global	c
	c:	dd	0


section	.text
global	main
main:
	push	rbp
	mov	rbp, rsp
	add	rsp, -16
	mov	r8, 10
	mov	dword [rbp+-4], r8d
	mov	r8, 20
	mov	dword [rbp+-4], r8d
	mov	r8, 30
	mov	byte [rbp+-4], r8b
	xor	r8, r8
	mov	r8d, dword [rbp+-4]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	xor	r8, r8
	mov	r8d, dword [rbp+-4]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	movzx	r8, byte [rbp+-4]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	mov	r8, 5
	mov	[a], r8d
	mov	r8, 15
	mov	[b], r8d
	mov	r8, 25
	mov	[c], r8d
	xor	r8, r8
	mov	r8d, dword [a]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	xor	r8, r8
	mov	r8d, dword [b]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	xor	r8, r8
	mov	r8d, dword [c]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	mov	r8, 0
	mov	eax, r8d
	jmp	L1
L1:
	add	rsp, 16	pop	rbp
	ret