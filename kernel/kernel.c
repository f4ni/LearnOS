#define VGA_ADDR 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile char* vga = (volatile char*)VGA_ADDR;

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i * 2] = ' ';
        vga[i * 2 + 1] = 0x07;
    }
}

void print_string(const char* s) {
    static int offset = 0;
    int i = 0;
    while (s[i]) {
        vga[offset * 2] = s[i];
        offset++;
        i++;
    }
}

void kernel_main() {
    clear_screen();
    print_string("LearnOS Kernel: System Booted Successfully!");
    print_string(" (32-bit Protected Mode)");

    while (1) {
        __asm__("hlt");
    }
}
