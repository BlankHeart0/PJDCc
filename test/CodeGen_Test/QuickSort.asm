	extern print_int
	extern print_long
	extern print_char
	extern print_string

section .data
global	array
	array:	times 10 dd	0


section	.text
global	Init
Init:
	push	rbp
	mov	rbp, rsp
	mov	r10, 9
	mov	r11, 0
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	mov	r10, 5
	mov	r11, 1
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	mov	r10, 2
	mov	r11, 2
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	mov	r10, 8
	mov	r11, 3
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	mov	r10, 1
	mov	r11, 4
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	mov	r10, 4
	mov	r11, 5
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	mov	r10, 3
	mov	r11, 6
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	mov	r10, 0
	mov	r11, 7
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	mov	r10, 7
	mov	r11, 8
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	mov	r10, 6
	mov	r11, 9
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
L1:
	pop	rbp
	ret

section	.text
global	Quick_Sort
Quick_Sort:
	push	rbp
	mov	rbp, rsp
	add	rsp, -32
	mov	dword [rbp+-4], edi
	mov	dword [rbp+-8], esi
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	xor	r11, r11
	mov	r11d, dword [rbp+-8]
	cmp	r10, r11
	setge	r10b
	and	r10, 255
	cmp	r10, 0
	je	L3
	jmp	L2
L3:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	dword [rbp+-12], r10d
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	mov	dword [rbp+-16], r10d
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	dword [rbp+-20], r10d
L4:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	xor	r11, r11
	mov	r11d, dword [rbp+-8]
	cmp	r10, r11
	setl	r10b
	and	r10, 255
	cmp	r10, 0
	je	L5
L6:
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	sal	r10, 2
	mov	r11, array
	add	r11, r10
	movzx	r11, word [r11]
	xor	r10, r10
	mov	r10d, dword [rbp+-20]
	sal	r10, 2
	mov	r12, array
	add	r12, r10
	movzx	r12, word [r12]
	cmp	r11, r12
	setge	r11b
	and	r11, 255
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	xor	r12, r12
	mov	r12d, dword [rbp+-8]
	cmp	r10, r12
	setl	r10b
	and	r10, 255
	and	r11, r10
	cmp	r11, 0
	je	L7
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	dec	dword [rbp+-8]
	jmp	L6
L7:
L8:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	sal	r10, 2
	mov	r11, array
	add	r11, r10
	movzx	r11, word [r11]
	xor	r10, r10
	mov	r10d, dword [rbp+-20]
	sal	r10, 2
	mov	r12, array
	add	r12, r10
	movzx	r12, word [r12]
	cmp	r11, r12
	setle	r11b
	and	r11, 255
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	xor	r12, r12
	mov	r12d, dword [rbp+-8]
	cmp	r10, r12
	setl	r10b
	and	r10, 255
	and	r11, r10
	cmp	r11, 0
	je	L9
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	inc	dword [rbp+-4]
	jmp	L8
L9:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	sal	r10, 2
	mov	r11, array
	add	r11, r10
	movzx	r11, word [r11]
	mov	dword [rbp+-24], r11d
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	sal	r10, 2
	mov	r11, array
	add	r11, r10
	movzx	r11, word [r11]
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	sal	r10, 2
	mov	r12, array
	add	r12, r10
	mov	[r12], r11d
	xor	r10, r10
	mov	r10d, dword [rbp+-24]
	xor	r11, r11
	mov	r11d, dword [rbp+-8]
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	jmp	L4
L5:
	xor	r10, r10
	mov	r10d, dword [rbp+-20]
	sal	r10, 2
	mov	r11, array
	add	r11, r10
	movzx	r11, word [r11]
	mov	dword [rbp+-24], r11d
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	sal	r10, 2
	mov	r11, array
	add	r11, r10
	movzx	r11, word [r11]
	xor	r10, r10
	mov	r10d, dword [rbp+-20]
	sal	r10, 2
	mov	r12, array
	add	r12, r10
	mov	[r12], r11d
	xor	r10, r10
	mov	r10d, dword [rbp+-24]
	xor	r11, r11
	mov	r11d, dword [rbp+-8]
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	mov	dword [rbp+-20], r10d
	xor	r10, r10
	mov	r10d, dword [rbp+-12]
	mov	rdi, r10
	xor	r10, r10
	mov	r10d, dword [rbp+-20]
	mov	r11, 1
	sub	r10, r11
	mov	rsi, r10
	call	Quick_Sort
	mov	r10, rax
	xor	r10, r10
	mov	r10d, dword [rbp+-20]
	mov	r11, 1
	add	r10, r11
	mov	rdi, r10
	xor	r10, r10
	mov	r10d, dword [rbp+-16]
	mov	rsi, r10
	call	Quick_Sort
	mov	r10, rax
L2:
	add	rsp, 32
	pop	rbp
	ret

section	.text
global	main
main:
	push	rbp
	mov	rbp, rsp
	add	rsp, -16
	call	Init
	mov	r10, rax
	mov	r10, 0
	mov	rdi, r10
	mov	r10, 9
	mov	rsi, r10
	call	Quick_Sort
	mov	r10, rax
	mov	r10, 0
	mov	dword [rbp+-4], r10d
L11:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	r11, 10
	cmp	r10, r11
	setl	r10b
	and	r10, 255
	cmp	r10, 0
	je	L12
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	sal	r10, 2
	mov	r11, array
	add	r11, r10
	movzx	r11, word [r11]
	mov	rdi, r11
	call	print_int
	mov	r10, rax
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	r11, 1
	add	r10, r11
	mov	dword [rbp+-4], r10d
	jmp	L11
L12:
	mov	r10, 0
	mov	eax, r10d
	jmp	L10
L10:
	add	rsp, 16
	pop	rbp
	ret

