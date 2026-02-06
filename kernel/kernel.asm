[BITS 16]
[ORG 0x1000]

start:
    mov si, msg
    call print_string
hang:
    hlt
    jmp hang

print_string:
    mov ah, 0x0E
.next:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next
.done:
    ret

msg db "Hello from the LearnOS kernel!",0
times 512-($-$$) db 0
