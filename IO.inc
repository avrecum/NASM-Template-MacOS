section .text
; Takes:
;	al: char
; Writes the specified value as an ASCII character to STDOUT.
WriteChar:
    push rcx
    push rdx
    push rbx

    push rax       	;Store parameter on the stack
    mov rcx, rsp	;Get address of the parameter
    mov rax, 0x4	;Specifying the write syscall
    mov rbx, 0x1	;STDOUT file descriptor
    mov rdx, 1		;Printing one byte
    int 0x80		;Invoke the syscall
    pop rax		;Clean up the stack

    pop rbx
    pop rdx
    pop rcx
	ret

; Returns:
;	al: value of the char
; Reads a character from STDIN, saves it in al.
ReadChar:
    sub rsp, 4		;allocate a dword on the stack
    mov rax, 3		;sys_read systemcall
    mov rbx, 0		;STDIN file descriptor
    mov rcx, rsp	;move the allocated dword address to rcx
    mov rdx, 2		;Reading one byte
    syscall	;Invoke the syscall

    mov al, BYTE [rsp] 	;Move the read value into al
    add rsp, 4		;Clean up the stack
    ret

; Takes:
;	rcx: String maximum length
;	rsi: address of where to store the string
; Returns:
;	rax: number of bytes read, i.e. string length
ReadString:
    push rsi
    push rcx

    mov rax, 3		;sys_read systemcall
    mov rbx, 0		;STDIN file descriptor
    mov rdx, rcx	;Reading n bytes
    mov rcx, rsi	;move the allocated dword address to rcx
    syscall		;Invoke the syscall
    pop rcx
    pop rsi

    mov rax, 0
    .count:
        inc rax
        inc rsi
    cmp rax, rcx
    je .equal

    cmp BYTE [rsi], 0xA
    je .done
    jmp .count
    .equal:
        mov rax, rcx
    .done:
    ret

; No parameters, prints a new line.
WriteLine:
    push rax

    mov al, 0xA
    call WriteChar

    pop rax
    ret

; Takes:
;	rsi: address of the string to be written
;	rcx: length of the string
WriteString:
    .print:
    mov rax, [rsi]
    call WriteChar
    inc rsi
    loop .print
    ret

; Takes:
;	rax: value
; Prints the value in decimal (base-10)
WriteDec:
    push 0xf1a9
    .next:
        mov rcx, 10
        mov rdx, 0
        div rcx
        add dl, 0x30
        mov rdi, rax
        xor rax, rax
        mov al, dl
        push rax
        mov rax, rdi
    cmp rax, 0
    jne .next
    .print:
        pop rax
        cmp rax, 0xf1a9
        je .done
        call WriteChar
        jmp .print
    .done:
    ret

; Returns:
;	al: value of the hex byte
; Reads a byte in hex, without the leading 0x
ReadHexByte:
    push rcx
    push rsi
    push rbx
    push rdx

    sub rsp, 4		;allocate a dword on the stack
    mov rax, 3		;sys_read systemcall
    mov rbx, 0		;STDIN file descriptor
    mov rcx, rsp	;move the allocated dword address to rcx
    mov rdx, 3		;Reading 2 bytes
    syscall		;Invoke the syscall
    mov rsi, rsp

    mov rcx, 2
    .nextbyte:
    cmp BYTE[rsi], '9'
    jle  .zeroto9
        sub BYTE[rsi], 0x27
    .zeroto9:
    sub BYTE[rsi], 0x30
    inc rsi
    loop .nextbyte

    movzx ax, BYTE [rsp] 	;Move the read value into ax
    shl	ax, 8
    mov	al, BYTE [rsp+1]
    shl al, 4
    shr ax, 4
    add rsp, 4			;Clean up the stack


    pop rdx
    pop rbx
    pop rsi
    pop rcx
    ret
; Takes:
;	rax: value
; Prints the value in hexa (base-16)
WriteHex:
    push rdx
    push rcx
    push rdi

    push rax
    mov al, '0'
    call WriteChar
    mov al, 'x'
    call WriteChar
    pop rax

    push 0xf1a9
    .next:
        mov rcx, 16
        mov rdx, 0
        div rcx
    cmp dl, 10
    jl  .zeroto9
        add dl, 0x7
    .zeroto9:
        add dl, 0x30
        mov rdi, rax
        xor rax, rax
        mov al, dl
        push rax
        mov rax, rdi
    cmp rax, 0
    jne .next
    .print:
        pop rax
        cmp rax, 0xf1a9
        je .done
        call WriteChar
        jmp .print
    .done:

    pop rdi
    pop rcx
    pop rdx
	ret

; Takes:
;	rsi: address of a byte array of length 16
; Prints the array as a 4x4 grid
Print4x4Matrix:
    mov bl, 0
    mov rcx, 16
    .print:
            xor rax, rax
            mov al, [rsi]
            call WriteHex
            inc rsi
            inc bl
        cmp bl, 4
        jne .next
        mov al, 0xA
        call WriteChar
        mov bl, 0
        jmp .done
        .next:
        mov al, ' '
        call WriteChar
        .done:
    loop .print
    ret
