#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"

void kernel_main() {
    enable_cursor(14, 15);
    clear_screen();
    print_string("System Booted Successfully!\n\n", YELLOW);
    print_string("OS    : LearnOS\n", YELLOW);
    print_string("Mode  : 32-bit Protected Mode\n", YELLOW);
    print_string("Author: Farhan Tariq\n\n", YELLOW);

    pic_remap();
    idt_load();
    keyboard_init();

    __asm__ volatile("sti");

    print_string("\n> ", LIGHT_GRAY);
    while (1) {
        __asm__("hlt");
    }
}
