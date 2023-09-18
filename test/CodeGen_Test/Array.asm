	extern print_int
	extern print_char
	extern print_string

section	.data
global	array
	array:	times 10 dd	0

section	.text
global	Init
Init:
	push	rbp
	mov	rbp, rsp
	mov	r8, 9
	mov	r9, 0
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
	mov	r8, 5
	mov	r9, 1
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
	mov	r8, 2
	mov	r9, 2
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
	mov	r8, 8
	mov	r9, 3
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
	mov	r8, 1
	mov	r9, 4
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
	mov	r8, 4
	mov	r9, 5
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
	mov	r8, 3
	mov	r9, 6
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
	mov	r8, 0
	mov	r9, 7
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
	mov	r8, 7
	mov	r9, 8
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
	mov	r8, 6
	mov	r9, 9
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
L1:
	pop	rbp
	ret

section	.data
global	i
	i:	dd	0

section	.data
global	j
	j:	dd	0

section	.data
global	temp
	temp:	dd	0

section	.text
global	Bubble_Sort
Bubble_Sort:
	push	rbp
	mov	rbp, rsp
	mov	r8, 0
	mov	[i], r8d
L3:
	xor	r8, r8
	mov	r8d, dword [i]
	mov	r9, 10
	mov	r10, 1
	sub	r9, r10
	cmp	r8, r9
	setl	r8b
	and	r8, 255
	cmp	r8, 0
	je	L4
	mov	r8, 0
	mov	[j], r8d
L5:
	xor	r8, r8
	mov	r8d, dword [j]
	mov	r9, 10
	mov	r10, 1
	sub	r9, r10
	xor	r10, r10
	mov	r10d, dword [i]
	sub	r9, r10
	cmp	r8, r9
	setl	r8b
	and	r8, 255
	cmp	r8, 0
	je	L6
	xor	r8, r8
	mov	r8d, dword [j]
	sal	r8, 2
	mov	r9, array
	add	r9, r8
	movzx	r9, word [r9]
	xor	r8, r8
	mov	r8d, dword [j]
	mov	r10, 1
	add	r8, r10
	sal	r8, 2
	mov	r10, array
	add	r10, r8
	movzx	r10, word [r10]
	cmp	r9, r10
	setg	r9b
	and	r9, 255
	cmp	r9, 0
	je	L7
	xor	r8, r8
	mov	r8d, dword [j]
	sal	r8, 2
	mov	r9, array
	add	r9, r8
	movzx	r9, word [r9]
	mov	[temp], r9d
	xor	r8, r8
	mov	r8d, dword [j]
	mov	r9, 1
	add	r8, r9
	sal	r8, 2
	mov	r9, array
	add	r9, r8
	movzx	r9, word [r9]
	xor	r8, r8
	mov	r8d, dword [j]
	sal	r8, 2
	mov	r10, array
	add	r10, r8
	mov	[r10], r9d
	xor	r8, r8
	mov	r8d, dword [temp]
	xor	r9, r9
	mov	r9d, dword [j]
	mov	r10, 1
	add	r9, r10
	sal	r9, 2
	mov	r10, array
	add	r10, r9
	mov	[r10], r8d
L7:
	xor	r8, r8
	mov	r8d, dword [j]
	mov	r9, 1
	add	r8, r9
	mov	[j], r8d
	jmp	L5
L6:
	xor	r8, r8
	mov	r8d, dword [i]
	mov	r9, 1
	add	r8, r9
	mov	[i], r8d
	jmp	L3
L4:
	jmp	L2
L2:
	pop	rbp
	ret

section	.text
global	main
main:
	push	rbp
	mov	rbp, rsp
	mov	r8, 0
	mov	rdi, r8
	call	Init
	mov	r9, rax
	mov	r8, 0
	mov	rdi, r8
	call	Bubble_Sort
	mov	r9, rax
	mov	r8, 0
	mov	[i], r8d
L9:
	xor	r8, r8
	mov	r8d, dword [i]
	mov	r9, 10
	cmp	r8, r9
	setl	r8b
	and	r8, 255
	cmp	r8, 0
	je	L10
	xor	r8, r8
	mov	r8d, dword [i]
	sal	r8, 2
	mov	r9, array
	add	r9, r8
	movzx	r9, word [r9]
	mov	rdi, r9
	call	print_int
	mov	r8, rax
	xor	r8, r8
	mov	r8d, dword [i]
	mov	r9, 1
	add	r8, r9
	mov	[i], r8d
	jmp	L9
L10:
	mov	r8, 0
	mov	eax, r8d
	jmp	L8
L8:
	pop	rbp
	ret

section .data

