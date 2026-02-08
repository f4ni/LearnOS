; ============================
; Global Descriptor Table (GDT)
; ============================

gdt_start:

gdt_null:
    dq 0

gdt_code:
    dw 0xFFFF          ; limit low
    dw 0x0000          ; base low
    db 0x00            ; base middle
    db 10011010b       ; access
    db 11001111b       ; flags + limit high
    db 0x00            ; base high

gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
