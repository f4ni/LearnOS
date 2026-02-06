[BITS 16]
[ORG 0x7C00]

start:
    cli                     ; Disable interrupts
    mov [BOOT_DRIVE], dl    ; Save boot drive number passed by BIOS
    mov ax, 0x0000
    mov ss, ax
    mov sp, 0x7C00
    sti                     ; Re-enable interrupts

    ; Print bootloader message
    mov si, message
    call print_string

    ; Load kernel from disk (sector 2, 1 sector, address 0x1000)
    mov es, 0
    mov bx, 0x1000          ; ES:BX points to load address
    mov dh, 0               ; Head
    mov dl, [BOOT_DRIVE]    ; Use saved boot drive
    mov ch, 0               ; Cylinder
    mov cl, 2               ; Sector 2
    call load_sector

    ; Jump to kernel
    jmp 0x0000:0x1000

hang:
    hlt
    jmp hang

; ----------------------------
; BIOS print string routine
; ----------------------------
print_string:
    mov ah, 0x0E
.print_char:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .print_char
.done:
    ret

; ----------------------------
; Load one sector via BIOS int 0x13
; ----------------------------
load_sector:
    push ax
    push cx
    push dx
    push es

    mov ah, 0x02      ; BIOS read sector
    mov al, 1         ; Number of sectors to read (1 for now)
    int 0x13          ; BIOS disk interrupt
    jc disk_error     ; Jump if carry flag set (error)

    pop es
    pop dx
    pop cx
    pop ax
    ret

disk_error:
    mov si, err_msg
    call print_string
    jmp hang

message db "Bootloader: loading kernel...",0
err_msg db "Disk read error!",0
BOOT_DRIVE db 0

; Boot signature
times 510-($-$$) db 0
dw 0xAA55
