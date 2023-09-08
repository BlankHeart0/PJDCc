section .text
	global main
	extern printf

main:
	common	j  8:8
	mov	r8, 0
	mov	[j], r8
L1:
	mov	r8, [j]
	mov	r9, 5
	cmp	r8, r9
	setl	r8b
	and	r8, 255
	cmp	r8, 0
	je	L2
	common	k  8:8
	mov	r9, 0
	mov	[k], r9
L3:
	mov	r9, [k]
	mov	r10, 5
	cmp	r9, r10
	setl	r9b
	and	r9, 255
	cmp	r9, 0
	je	L4
	mov	r10, [j]
	push	rbp
	mov	rdi, format
	mov	rsi, r10
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r10, [k]
	push	rbp
	mov	rdi, format
	mov	rsi, r10
	mov	rax, 0
	call	printf
	pop	rbp
	mov	r10, [k]
	mov	r11, 1
	add	r10, r11
	mov	[k], r10
	jmp	L3
L4:
	mov	r10, [j]
	mov	r11, 1
	add	r10, r11
	mov	[j], r10
	jmp	L1
L2:

exit:
	mov	rax, 0
	ret

section .data
	format: db "%d",0XA,0
