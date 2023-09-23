	extern print_int
	extern print_long
	extern print_char
	extern print_string

section	.text
global	fib
fib:
	push	rbp
	mov	rbp, rsp
	add	rsp, -16
	mov	dword [rbp+-4], edi
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	r11, 1
	cmp	r10, r11
	sete	r10b
	and	r10, 255
	xor	r11, r11
	mov	r11d, dword [rbp+-4]
	mov	r12, 2
	cmp	r11, r12
	sete	r11b
	and	r11, 255
	or	r10, r11
	cmp	r10, 0
	je	L2
	mov	r10, 1
	mov	eax, r10d
	jmp	L1
L2:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	r11, 1
	sub	r10, r11
	mov	rdi, r10
	call	fib
	mov	r10, rax
	xor	r11, r11
	mov	r11d, dword [rbp+-4]
	mov	r12, 2
	sub	r11, r12
	mov	rdi, r11
	push	r10
	call	fib
	mov	r11, rax
	pop	r10
	add	r10, r11
	mov	eax, r10d
	jmp	L1
L1:
	add	rsp, 16
	pop	rbp
	ret

section	.text
global	factorial
factorial:
	push	rbp
	mov	rbp, rsp
	add	rsp, -16
	mov	dword [rbp+-4], edi
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	test	r10, r10
	sete	r10b
	movzx	r10, r10b
	cmp	r10, 0
	je	L4
	mov	r10, 1
	mov	eax, r10d
	jmp	L3
L4:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	xor	r11, r11
	mov	r11d, dword [rbp+-4]
	mov	r12, 1
	sub	r11, r12
	mov	rdi, r11
	push	r10
	call	factorial
	mov	r11, rax
	pop	r10
	imul	r10, r11
	mov	eax, r10d
	jmp	L3
L3:
	add	rsp, 16
	pop	rbp
	ret

section	.text
global	main
main:
	push	rbp
	mov	rbp, rsp
	add	rsp, -16
	mov	r10, 1
	mov	dword [rbp+-4], r10d
L6:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	r11, 10
	cmp	r10, r11
	setle	r10b
	and	r10, 255
	cmp	r10, 0
	je	L7
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	rdi, r10
	call	factorial
	mov	r10, rax
	mov	rdi, r10
	call	print_int
	mov	r10, rax
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	r11, 1
	add	r10, r11
	mov	dword [rbp+-4], r10d
	jmp	L6
L7:
	mov	r10, 0
	mov	eax, r10d
	jmp	L5
L5:
	add	rsp, 16
	pop	rbp
	ret

