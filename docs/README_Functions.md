# LearnOS – Function Reference

This file lists all main functions in LearnOS and their purpose.

---

## Bootloader (boot.asm)

- **start:** Entry point of the bootloader.
- **print_string:** Prints a null-terminated string using BIOS interrupt 0x10.
- **load_sector:** Loads one sector from disk into memory using BIOS interrupt 0x13.
- **disk_error:** Prints disk error message and halts the CPU.

---

## Kernel Entry (kernel_entry.asm)

- **kernel_entry:** Entry point for the kernel. Sets up segment registers, stack, and calls `kernel_main`.
- **stack_top / stack_bottom:** Defines stack space for C code.

---

## Kernel (kernel.c)

- **kernel_main:** Main function of the kernel. Prints a message and halts CPU.
- **print_char:** Prints a single character using BIOS interrupt 0x10 (or later, custom VGA driver).
- **clear_screen (optional):** Clears VGA text memory (if implementing own driver).
- **keyboard_getchar (optional):** Reads a key from the keyboard controller (if implementing input driver).

---

## Makefile Targets

- **all:** Builds the disk image (bootloader + kernel).
- **run:** Launches LearnOS in QEMU.
- **clean:** Removes build artifacts.

See [Step-by-Step Tutorial](README_Tutorial.md) for detailed explanation.
