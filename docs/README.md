# LearnOS

LearnOS is a **minimal, educational operating system** written to teach the fundamentals of OS development.  
It is single-tasking, runs in **16-bit real mode**, and currently boots using BIOS before handing control to a simple C kernel.

This repository is designed for learners who want to understand:

- Bootloaders
- CPU initialization and stack setup
- Assembly → C handoff
- Simple I/O (printing to screen, keyboard input)
- Disk loading and memory mapping

---

## Project Structure

LearnOS/
├── boot/                  # Bootloader source (boot.asm)
├── build/                 # Compiled binaries and disk image
├── docs/                  # Step-by-step explanation for beginners
├── kernel/                # Kernel source (kernel_entry.asm + kernel.c)
└── Makefile               # Build pipeline

---

## Recommended Reading

### 1. **Function Reference** (`README_Functions.md`)

- Quick reference of all **main functions** in LearnOS  
- Good for **navigating the code quickly**  
- Covers:
  - Bootloader functions
  - Kernel entry functions
  - C kernel functions
  - Makefile targets  

> Read this if you want a **summary of the OS components and their purpose**.  
> [Open Function Reference](README_Functions.md)

---

### 2. **Step-by-Step Tutorial** (`README_Tutorial.md`)

- Detailed **line-by-line explanation** of the OS  
- Explains:
  - Bootloader internals
  - Kernel entry and stack setup
  - BIOS printing and later VGA driver
  - Keyboard input
  - Disk image creation and linking  
- Designed for **absolute beginners** in OS development

> Read this if you want a **deep understanding of how each part works**.  
> [Open Step-by-Step Tutorial](README_Tutorial.md)

---

## Quick Start

1. **Build LearnOS**

```bash
make all
```

2. **Run in QEMU**

```bash
make run
```

3. **Clean build artifacts**

```bash
make clean
```

### Next Steps

- Replace BIOS printing with VGA driver
- Add keyboard input
- Explore protected mode
- Implement basic drivers and multitasking