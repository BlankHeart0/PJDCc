	extern print_int
	extern print_long
	extern print_char
	extern print_string

section .data
global	d
	d:	db	0

global	e
	e:	dd	0

global	f
	f:	dq	0


section	.text
global	main
main:
	push	rbp
	mov	rbp, rsp
	add	rsp, -16
	mov	r8, 10
	mov	byte [rbp+-4], r8b
	mov	r8, 20
	mov	dword [rbp+-8], r8d
	mov	r8, 30
	mov	qword [rbp+-16], r8
	mov	r8, 40
	mov	[d], r8b
	mov	r8, 50
	mov	[e], r8d
	mov	r8, 60
	mov	[f], r8
	movzx	r8, byte [rbp+-4]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	xor	r8, r8
	mov	r8d, dword [rbp+-8]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	mov	r8, [rbp+-16]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	movzx	r8, byte [d]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	xor	r8, r8
	mov	r8d, dword [e]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	mov	r8, [f]
	mov	rdi, r8
	call	print_int
	mov	r9, rax
	mov	r8, 0
	mov	eax, r8d
	jmp	L1
L1:
	add	rsp, 16
	pop	rbp
	ret

