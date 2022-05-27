%include "IO.inc"
   global    start
    section   .text
start:
; Syscalls are handled differently on OSX. For more details, see the links in README.md

    mov       rax, 0x02000004
    mov       rdi, 1
    mov       rsi, message
    mov       rdx, 13
    syscall
    mov       rax, 0x02000001
    xor       rdi, rdi
    syscall
    section   .data
message:
    db        "Hello, World", 10
