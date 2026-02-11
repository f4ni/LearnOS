[BITS 16]
[ORG 0x7C00]

start:
    cli                     ; Disable interrupts
    mov [BOOT_DRIVE], dl    ; Save boot drive number passed by BIOS

    mov ax, 0x0000
    mov ds, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Print bootloader message
    mov si, message
    call print_string

    ; Load kernel from disk
    mov ax, 0x0000
    mov es, ax
    mov bx, 0x1000          ; ES:BX points to load address

    mov dh, 0               ; Head
    mov dl, [BOOT_DRIVE]    ; Use saved boot drive
    mov ch, 0               ; Cylinder
    mov cl, 2               ; Sector 2

    call load_sector
    call enable_a20
    call sleep_3s
    call switch_to_pm

hang:
    hlt
    jmp hang


; =========================
; Sleep for ~2 seconds
; =========================
sleep_3s:
    mov ah, 0x86        ; BIOS wait function
    mov cx, 0x001E      ; High word of 2,000,000
    mov dx, 0x8480      ; Low word of 2,000,000
    int 0x15            ; BIOS delay
    ret


%include "boot/gdt.asm"


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


; =================================
; Load one sector via BIOS int 0x13
; =================================
load_sector:
    push ax
    push cx
    push dx
    push es

    mov ah, 0x02      ; BIOS read sector
    mov al, 10         ; Number of sectors to read (can adjust)
    int 0x13          ; BIOS disk interrupt
    jc disk_error     ; Jump if carry flag set (error)

    pop es
    pop dx
    pop cx
    pop ax
    ret


; =========================
; Enable A20 (FAST METHOD)
; =========================
enable_a20:
    in al, 0x92
    or al, 00000010b
    out 0x92, al
    ret


; =========================
; ENTER PROTECTED MODE
; =========================
switch_to_pm:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; FAR jump clears pipeline
    jmp CODE_SEG:protected_mode_start


; =========================
; 32-bit code starts here
; =========================

[BITS 32]

protected_mode_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, 0x90000   ; safe stack

    ; Jump to kernel (now must be 32-bit)
    jmp CODE_SEG:0x1000


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
