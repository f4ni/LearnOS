# $@ = target file
# $< = first dependency
# $^ = all dependencies

AS      = nasm
CC      = i386-elf-gcc
LD      = i386-elf-ld

CFLAGS  = -Icpu -Idriver -Iutils -ffreestanding -m32 -fno-pie -fno-stack-protector
# LDFLAGS = -Ttext 0x1000 -e kernel_entry --oformat binary
LDFLAGS = -T kernel/linker.ld --oformat binary

BOOT_SRC    = boot/boot.asm
BOOT_BIN    = build/boot.bin
KERNEL_BIN  = build/kernel.bin
DISK_IMG    = build/learnos.img

OBJ = build/kernel_entry.o build/kernel.o build/vga.o build/idt.o build/pic.o build/keyboard.o build/keyboard_handler.o build/string.o
# OBJ := $(wildcard build/*.o)

all: $(DISK_IMG)

# 1. Build the Bootloader (Direct to Binary)
$(BOOT_BIN): $(BOOT_SRC)
	$(AS) -f bin $< -o $@

# 2. Compile C files from kernel/, cpu/, and driver/
build/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -Idriver -o $@

build/%.o: cpu/%.c
	$(CC) $(CFLAGS) -c $< -Idriver -o $@

build/%.o: driver/%.c
	$(CC) $(CFLAGS) -c $< -Idriver -o $@

build/%.o: utils/%.c
	$(CC) $(CFLAGS) -c $< -Idriver -o $@

# 3. Assemble Assembly files from kernel/ and cpu/
build/%.o: kernel/%.asm
	$(AS) -f elf32 $< -o $@

build/%.o: cpu/%.asm
	$(AS) -f elf32 $< -o $@

# 4. Link the Kernel objects into one binary
$(KERNEL_BIN): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# 5. Combine everything into the final Disk Image
$(DISK_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	cat $^ > $@
	truncate -s 10k $@

run: $(DISK_IMG)
	qemu-system-i386 -drive format=raw,file=$(DISK_IMG)

clean:
	rm -f build/*