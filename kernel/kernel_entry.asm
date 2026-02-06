[BITS 16]

global kernel_entry
extern kernel_main

kernel_entry:
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, stack_top
    call kernel_main

.hang:
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 4096
stack_top:
