section .text
    global  main
    extern  printf

main:
    push    rbp
    mov     rdi, format        
    mov     rsi, 10             
    mov     rax, 0             

    call    printf 
        
    pop	    rbp

exit:		
    mov	    rax,0	
    ret

section .data
    format: db "%d",0XA,0    