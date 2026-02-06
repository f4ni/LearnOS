; Tell assembler this is 16-bit code
[BITS 16]

; BIOS loads boot sector at 0x7C00
[ORG 0x7C00]

start:
    ; Disable interrupts while we set things up
    cli

    ; Setup stack
    mov ax, 0x0000
    mov ss, ax
    mov sp, 0x7C00

    ; Enable interrupts again
    sti

    ; Print message
    mov si, message
    call print_string

hang:
    hlt
    jmp hang

; ----------------------------
; Print string using BIOS
; ----------------------------
print_string:
    mov ah, 0x0E        ; BIOS teletype function

.print_char:
    lodsb               ; AL = [SI], SI++
    cmp al, 0
    je .done
    int 0x10            ; Print character in AL
    jmp .print_char

.done:
    ret

message db "Hello from my basic bootloader for LearnOS!", 0

; ----------------------------
; Boot signature (REQUIRED)
; ----------------------------
times 510-($-$$) db 0
dw 0xAA55
