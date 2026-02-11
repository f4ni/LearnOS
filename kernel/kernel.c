#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"

void kernel_main() {
    clear_screen();
    print_string("System Booted Successfully!\n\n");
    print_string("OS    : LearnOS\n");
    print_string("Mode  : 32-bit Protected Mode\n");
    print_string("Author: Farhan Tariq\n\n");

    pic_remap();
    idt_load();
    keyboard_init();

    __asm__ volatile("sti");

    while (1) {
        __asm__("hlt");
    }
}
