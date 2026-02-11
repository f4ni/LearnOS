# LearnOS: A Beginner's Guide to Our Operating System

Welcome to the world of OS development! This guide will walk you through how **LearnOS** works, from the moment the power button is pressed to the kernel printing its first message.

---

## 1. The Big Picture (The Flow)

When our computer starts, it doesn't know what an "Operating System" is. It follows a strict set of rules:
1. **BIOS** (Basic Input/Output System) starts up.
2. It looks for a **bootable drive** (like a Hard Drive or USB).
3. It loads the first **512 bytes** of that drive into memory at address `0x7C00`.
4. It starts executing those 512 bytes. This is our **Bootloader**.

---

## 2. The Bootloader [boot/boot.asm]

Our bootloader is like a "starter motor." Its job is to load the actual engine (the Kernel).

### Key Concepts:
- **16-bit Real Mode**: This is the "ancient" mode CPUs start in for backwards compatibility. You only have access to 1MB of RAM and simple instructions.
- **`[ORG 0x7C00]`**: Tells the assembler that this code will be loaded at memory address `0x7C00`.
- **`load_sector`**: Uses **BIOS Interrupt `int 0x13`** to read the Kernel from the disk and put it into memory at address `0x1000`.
- **`enable_a20`**: A historical quirk! To access memory beyond 1MB, we must "unlock" the 20th address line.
- **`switch_to_pm`**: Switches the CPU from 16-bit "Real Mode" to 32-bit **"Protected Mode"**. This gives us access to 4GB of RAM and modern features.

### The Magic Signature:
```nasm
times 510-($-$$) db 0  ; Fill the rest of the 512 bytes with zeros
dw 0xAA55              ; The BIOS looks for this exact number at the end
```

---

## 3. The Global Descriptor Table [boot/gdt.asm]

To enter Protected Mode, we need a **GDT**. Think of this as a "Permission Slips" table. It tells the CPU:
- Where the "Code" segment starts and ends.
- Where the "Data" segment starts and ends.
- What permissions they have (Read/Write/Execute).

---

## 4. The Bridge [kernel/kernel_entry.asm]

The Bootloader is written in Assembly, but the Kernel is written in C. We need a bridge!
- **`kernel_entry`**: This is the first code that runs in the Kernel.
- It sets up a **Stack** (necessary for C functions to work).
- It calls **`kernel_main`**.

---

## 5. The Kernel [kernel/kernel.c]

This is where the logic happens! In LearnOS, the kernel is very simple:
- **VGA Memory (`0xB8000`)**: This is a special memory area. Whatever you write here appears on the screen.
- **`clear_screen()`**.
- **`print_string()`**.

---

## 6. The Linker [kernel/linker.ld]

How do we make sure different files (Assembly and C) fit together? The Linker is like a jigsaw puzzle solver.
- It ensures that `kernel_entry` is at the very beginning of the kernel binary.
- It ensures the binary is organized so it can be loaded at address `0x1000`.

---

## 7. The Build System [Makefile]

The [Makefile]:
1. **Assembling** [.asm].
2. **Compiling** [.c].
3. **Linking** them together into a single `kernel.bin`.
4. **Stitching** the Bootloader and Kernel together into a `disk.img`.
5. **Running** the OS in an emulator called `qemu`.

### Try it out!
Just type:
```bash
make run
```
This will build everything and launch the OS in a window!