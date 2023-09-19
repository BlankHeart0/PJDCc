	extern print_int
	extern print_char
	extern print_string

section	.data
global	s
	s:	dq	0

section	.data
global	array
	array:	times 8 db	0

section	.text
global	main
main:
	push	rbp
	mov	rbp, rsp
	mov	r8, Str1
	mov	rdi, r8
	call	print_string
	mov	r9, rax
	mov	r8, Str2
	mov	[s], r8
	mov	r8, [s]
	mov	rdi, r8
	call	print_string
	mov	r9, rax
	mov	r8, 104
	mov	r9, 0
	sal	r9, 0
	mov	r10, array
	add	r10, r9
	mov	[r10], r8b
	mov	r8, 101
	mov	r9, 1
	sal	r9, 0
	mov	r10, array
	add	r10, r9
	mov	[r10], r8b
	mov	r8, 108
	mov	r9, 2
	sal	r9, 0
	mov	r10, array
	add	r10, r9
	mov	[r10], r8b
	mov	r8, 108
	mov	r9, 3
	sal	r9, 0
	mov	r10, array
	add	r10, r9
	mov	[r10], r8b
	mov	r8, 111
	mov	r9, 4
	sal	r9, 0
	mov	r10, array
	add	r10, r9
	mov	[r10], r8b
	mov	r8, 10
	mov	r9, 6
	sal	r9, 0
	mov	r10, array
	add	r10, r9
	mov	[r10], r8b
	mov	r8, 3
	sal	r8, 0
	mov	r9, array
	add	r9, r8
	movzx	r9, byte [r9]
	mov	rdi, r9
	call	print_char
	mov	r8, rax
	mov	r8, array
	mov	r9, 3
	sal	r9, 0
	add	r8, r9
	movzx	r8, byte [r8]
	mov	rdi, r8
	call	print_char
	mov	r9, rax
	mov	r8, array
	mov	rdi, r8
	call	print_string
	mov	r9, rax
	mov	r8, 0
	mov	eax, r8d
	jmp	L1
L1:
	pop	rbp
	ret

section .data
	Str1 db 72,101,108,108,111,32,119,111,114,108,100,10,0
	Str2 db 72,101,108,108,111,32,119,111,114,108,100,10,0

