#include <stdint.h>
#include "io.h"
#include "idt.h"
#include "vga.h"

#define KEYBOARD_IRQ 1
#define KEYBOARD_PORT 0x60
#define KEYBOARD_INTERRUPT 0x21

extern void keyboard_handler();

// Basic US keyboard scancode set 1
static const char scancode_table[] =
{
    0, 27, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b',
    '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n',
    0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
    'l', ';', '\'', '`',
    0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/',
    0,
    '*',
    0,
    ' '
};

void keyboard_handler_main()
{
    // Send End Of Interrupt (EOI) to Master PIC
    outb(0x20, 0x20);

    uint8_t scancode = inb(KEYBOARD_PORT);

    // Ignore key release (Break code)
    if (scancode & 0x80)
        return;

    char c = scancode_table[scancode];

    if (c) {
        putchar(c);
    }
}

void keyboard_init()
{
    idt_set_gate(0x21, (uint32_t)keyboard_handler);
    print_string("Keyboard installed successfully.\n");
}