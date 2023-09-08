section .text
	global main
	extern printf

main:
	common	i  8:8
	common	j  8:8
	mov	r8, 6
	mov	[i], r8
	mov	r8, 12
	mov	[j], r8
	mov	r8, [i]
	mov	r9, [j]
	cmp	r8, r9
	setl	r8b
	and	r8, 255
	cmp	r8, 0
	je	L1
	mov	r9, [i]
	push	rbp
	mov	rdi, format
	mov	rsi, r9
	mov	rax, 0
	call	printf
	pop	rbp
	jmp	L2
L1:
	mov	r9, [j]
	push	rbp
	mov	rdi, format
	mov	rsi, r9
	mov	rax, 0
	call	printf
	pop	rbp
L2:
	common	t  8:8
	mov	r9, 10
	mov	[t], r9
	mov	r9, [t]
	mov	r10, 10
	cmp	r9, r10
	setl	r9b
	and	r9, 255
	cmp	r9, 0
	je	L3
	mov	r10, 1
	push	rbp
	mov	rdi, format
	mov	rsi, r10
	mov	rax, 0
	call	printf
	pop	rbp
	jmp	L4
L3:
	mov	r10, [t]
	mov	r11, 10
	cmp	r10, r11
	sete	r10b
	and	r10, 255
	cmp	r10, 0
	je	L5
	mov	r11, 10
	push	rbp
	mov	rdi, format
	mov	rsi, r11
	mov	rax, 0
	call	printf
	pop	rbp
	jmp	L6
L5:
	mov	r11, [t]
	mov	r12, 20
	cmp	r11, r12
	sete	r11b
	and	r11, 255
	cmp	r11, 0
	je	L7
	mov	r12, 20
	push	rbp
	mov	rdi, format
	mov	rsi, r12
	mov	rax, 0
	call	printf
	pop	rbp
	jmp	L8
L7:
	mov	r12, 100
	push	rbp
	mov	rdi, format
	mov	rsi, r12
	mov	rax, 0
	call	printf
	pop	rbp
L8:
L6:
L4:

exit:
	mov	rax, 0
	ret

section .data
	format: db "%d",0XA,0
