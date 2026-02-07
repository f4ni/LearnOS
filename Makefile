# ===== Toolchain =====
AS      = nasm
CC      = i386-elf-gcc
LD      = i386-elf-ld

# ===== Flags =====
CFLAGS  = -ffreestanding -m16 -fno-pie -fno-stack-protector
# LDFLAGS = -Ttext 0x1000 -e kernel_entry --oformat binary
LDFLAGS = -T kernel/linker.ld --oformat binary

# ===== Files =====
BOOT_SRC    = boot/boot.asm
BOOT_BIN    = build/boot.bin
KERNEL_BIN  = build/kernel.bin
DISK_IMG    = build/disk.img

# This automatically finds all .o files needed for the kernel
OBJ = build/kernel_entry.o build/kernel.o

# ===== Targets =====
all: $(DISK_IMG)

# 1. Build the Bootloader (Direct to Binary)
$(BOOT_BIN): $(BOOT_SRC)
	$(AS) -f bin $< -o $@

# 2. Compile C files to Object files
build/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 3. Assemble Kernel Entry to Object file
# Note: We use elf32 here so the linker can bridge it with C code
build/%.o: kernel/%.asm
	$(AS) -f elf32 $< -o $@

# 4. Link the Kernel objects into one binary
$(KERNEL_BIN): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# 5. Combine everything into the final Disk Image
$(DISK_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	cat $^ > $@
	truncate -s 10k $@

# ===== Commands =====
run: $(DISK_IMG)
	qemu-system-i386 -drive format=raw,file=$(DISK_IMG)
# 	qemu-system-i386 -drive format=raw,file=$(DISK_IMG) -d int,cpu_reset

clean:
	rm -f build/*