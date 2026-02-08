[BITS 32]

global kernel_entry
extern kernel_main

kernel_entry:
    ; Stack already valid, but we can reset it if we want
    mov esp, stack_top

    call kernel_main

.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 4096
stack_top:
