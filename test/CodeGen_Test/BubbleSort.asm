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
global	Bubble_Sort
Bubble_Sort:
	push	rbp
	mov	rbp, rsp
	add	rsp, -16
	mov	r10, 0
	mov	dword [rbp+-4], r10d
	mov	r10, 0
	mov	dword [rbp+-8], r10d
L3:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	r11, 10
	mov	r12, 1
	sub	r11, r12
	cmp	r10, r11
	setl	r10b
	and	r10, 255
	cmp	r10, 0
	je	L4
	mov	r10, 0
	mov	dword [rbp+-8], r10d
L5:
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	mov	r11, 10
	mov	r12, 1
	sub	r11, r12
	xor	r12, r12
	mov	r12d, dword [rbp+-4]
	sub	r11, r12
	cmp	r10, r11
	setl	r10b
	and	r10, 255
	cmp	r10, 0
	je	L6
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	sal	r10, 2
	mov	r11, array
	add	r11, r10
	movzx	r11, word [r11]
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	mov	r12, 1
	add	r10, r12
	sal	r10, 2
	mov	r12, array
	add	r12, r10
	movzx	r12, word [r12]
	cmp	r11, r12
	setg	r11b
	and	r11, 255
	cmp	r11, 0
	je	L7
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	sal	r10, 2
	mov	r11, array
	add	r11, r10
	movzx	r11, word [r11]
	mov	dword [rbp+-12], r11d
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	mov	r11, 1
	add	r10, r11
	sal	r10, 2
	mov	r11, array
	add	r11, r10
	movzx	r11, word [r11]
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	sal	r10, 2
	mov	r12, array
	add	r12, r10
	mov	[r12], r11d
	xor	r10, r10
	mov	r10d, dword [rbp+-12]
	xor	r11, r11
	mov	r11d, dword [rbp+-8]
	mov	r12, 1
	add	r11, r12
	sal	r11, 2
	mov	r12, array
	add	r12, r11
	mov	[r12], r10d
L7:
	xor	r10, r10
	mov	r10d, dword [rbp+-8]
	mov	r11, 1
	add	r10, r11
	mov	dword [rbp+-8], r10d
	jmp	L5
L6:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	r11, 1
	add	r10, r11
	mov	dword [rbp+-4], r10d
	jmp	L3
L4:
	jmp	L2
L2:
	add	rsp, 16
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
	call	Bubble_Sort
	mov	r10, rax
	mov	r10, 0
	mov	dword [rbp+-4], r10d
L9:
	xor	r10, r10
	mov	r10d, dword [rbp+-4]
	mov	r11, 10
	cmp	r10, r11
	setl	r10b
	and	r10, 255
	cmp	r10, 0
	je	L10
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
	jmp	L9
L10:
	mov	r10, 0
	mov	eax, r10d
	jmp	L8
L8:
	add	rsp, 16
	pop	rbp
	ret

