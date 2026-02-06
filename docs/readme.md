
# LearnOS

This is a basic OS written to learn the fundamentals of OS development.

---

# Big Picture First (Very Important)

Before touching code, fix this flow in your head:

```
Power ON
  ↓
BIOS firmware runs
  ↓
BIOS loads first 512 bytes (boot sector) to memory at 0x7C00
  ↓
BIOS jumps to 0x7C00 → the boot.asm starts
  ↓
Bootloader:
   - sets CPU state
   - prints message
   - loads kernel from disk into RAM
   - jumps to kernel
  ↓
kernel_entry.asm:
   - sets segments + stack
   - calls C code
  ↓
kernel.c:
   - prints message
   - halts forever
```

LearnOS is **not multitasking**, **not protected mode**, **not fancy** yet — and that’s PERFECT.

---

# PART 1: Bootloader (boot.asm)

## What is a bootloader?

A bootloader is:

* **raw machine code**
* exactly **512 bytes**
* loaded by BIOS at **0x7C00**
* runs in **16-bit Real Mode**
* has **no standard library**
* talks to BIOS via interrupts

---

## These two lines are NON-NEGOTIABLE

```asm
[BITS 16]
[ORG 0x7C00]
```

### `[BITS 16]`

* BIOS starts CPU in **16-bit real mode**
* All instructions must be 16-bit compatible

### `[ORG 0x7C00]`

* BIOS loads boot sector at **physical address 0x7C00**
* ORG tells NASM:

  > “Assume this code lives at 0x7C00”

Without this → jumps, labels, memory references break.

---

## Entry Point

```asm
start:
```

This is where BIOS jumps.

---

## Interrupt Control

```asm
cli
```

* **CLI** = Clear Interrupt Flag
* Temporarily disables interrupts
* Why?

  * We are setting up stack
  * Interrupt during stack setup = crash

---

## Save Boot Drive Number

```asm
mov [BOOT_DRIVE], dl
```

**Very important concept**

* BIOS passes boot drive number in `DL`

  * `0x00` → floppy
  * `0x80` → first hard disk
* BIOS disk reads **require correct DL**
* If we lose it → disk read fails

So we save it.

---

## Stack Setup

```asm
mov ax, 0x0000
mov ss, ax
mov sp, 0x7C00
```

### What is a stack?

* Used for:

  * `call`
  * `ret`
  * `push`
  * `pop`

### Why stack at 0x7C00?

* Bootloader code is already there
* Stack grows **downwards**
* Safe and simple for now

---

```asm
sti
```

* Re-enable interrupts
* Now system is safe again

---

## Printing Text (BIOS)

```asm
mov si, message
call print_string
```

### `SI`

* Source Index register
* Used for string operations

`message` → address of null-terminated string

---

### `print_string` function

```asm
mov ah, 0x0E
```

* BIOS video teletype function
* Prints character in `AL`

```asm
lodsb
```

* Load byte from `[DS:SI]` into `AL`
* Auto-increments `SI`

```asm
int 0x10
```

* BIOS video interrupt

This is **firmware printing**, not OS printing.

---

## Disk Read (Critical Concept)

```asm
mov bx, 0x1000
```

* Kernel will be loaded at **physical address 0x1000**
* BIOS uses `ES:BX`

```asm
mov ah, 0x02
```

* BIOS read sector function

```asm
mov al, 1
```

* Number of sectors to read

```asm
mov ch, 0   ; Cylinder
mov dh, 0   ; Head
mov cl, 2   ; Sector
```

* CHS addressing
* Sector 1 = bootloader
* Sector 2 = kernel

```asm
int 0x13
```

* BIOS disk service

```asm
jc disk_error
```

* Carry Flag set = disk error

---

## Jump to Kernel

```asm
jmp 0x0000:0x1000
```

### FAR jump:

* Changes CS:IP
* CPU now executes kernel code

At this moment:

* Bootloader is DONE
* Kernel owns the machine

---

## Boot Signature

```asm
times 510-($-$$) db 0
dw 0xAA55
```

### Mandatory BIOS requirement:

* Last two bytes must be `0x55AA`
* Otherwise BIOS ignores the disk

---

# PART 2: Kernel Entry (kernel_entry.asm)

This file is the **bridge between assembly and C**.

---

```asm
[BITS 16]
```

Still real mode.

---

```asm
global kernel_entry
extern kernel_main
```

### Meaning:

* `kernel_entry` → visible to linker
* `kernel_main` → defined in C

---

## CPU State Cleanup

```asm
mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax
```

Why?

* Bootloader left CPU in unknown state
* C expects clean segment registers

---

## Stack Setup for C

```asm
mov sp, stack_top
```

* C **REQUIRES** stack
* We allocated one manually

```asm
section .bss
resb 4096
```

* `.bss` = uninitialized memory
* Stack grows downward from `stack_top`

---

## Enter C World

```asm
call kernel_main
```

**C code starts running**

---

## Halt Loop

```asm
jmp .hang
```

Never return.

---

# PART 3: Kernel in C (kernel.c)

---

```c
__asm__(".code16gcc");
```

### EXTREMELY IMPORTANT

* Tells GCC:

  * Generate **16-bit compatible code**
* Without this → crash

---

## Printing a Character

```c
int $0x10
```

We are **still using BIOS**
LearnOS has:

* no drivers
* no framebuffer
* no console

Totally fine for now.

---

## `kernel_main`

```c
const char *message = "\r\nHello from C kernel of LearnOS!";
```

* `\r\n` = new line in BIOS text mode

```c
while (message[i] != '\0')
```

* Manual string loop
* No `printf`
* No libc

---

```c
while (1) {
    __asm__("hlt");
}
```

* HLT = stop CPU until interrupt
* Prevents 100% CPU usage

---

# PART 4: Makefile (Build Pipeline)

This is the **toolchain logic**.

---

## Cross Compiler

```makefile
i386-elf-gcc
```

Why?

* Host GCC assumes Linux ABI
* Cross compiler produces **freestanding code**

---

## Flags

```makefile
-ffreestanding
```

→ No standard library

```makefile
-m16
```

→ 16-bit code

```makefile
-fno-pie
```

→ No position-independent code

---

## Linker Flags

```makefile
-Ttext 0x1000
```

* Kernel linked assuming it runs at 0x1000

```makefile
--oformat binary
```

* Raw flat binary
* No ELF headers

---

## Disk Image Creation

```makefile
cat boot.bin kernel.bin > disk.img
```

Disk layout:

```
Sector 1 → bootloader
Sector 2 → kernel
```

Exactly what BIOS expects.

---

# What We Have Built (Reality Check)

We built:
- Real BIOS bootloader
- Manual disk loading
- Assembly → C handoff
- Freestanding C kernel
- Running in QEMU
