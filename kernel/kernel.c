// kernel.c - freestanding
__asm__(".code16gcc");

void print_char(char c) {
    __asm__ volatile (
        "mov $0x0E, %%ah\n\t"
        "mov %0, %%al\n\t"
        "int $0x10"
        : : "r" (c) : "ax"
    );
}

void kernel_main() {
    const char *message = "\r\nHello from C kernel of LearnOS!";
    
    int i = 0;
    while (message[i] != '\0') {
        print_char(message[i]);
        i++;
    }

    // Infinite loop
    while (1) {
        __asm__("hlt");
    }
}
