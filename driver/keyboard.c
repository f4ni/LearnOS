#include <stdint.h>
#include "io.h"
#include "idt.h"
#include "vga.h"
#include "commands.h"

#define KEYBOARD_IRQ 1
#define KEYBOARD_PORT 0x60
#define KEYBOARD_INTERRUPT 0x21
#define MAX_CHARS 256

extern void keyboard_handler();
static char key_buffer[MAX_CHARS];
static uint8_t ind = 0;

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

void backspace()
{
    if (ind > 0) {
        key_buffer[--ind] = '\0';
    }
}

void keyboard_handler_main()
{
    uint8_t scancode = inb(KEYBOARD_PORT);

    // Send End Of Interrupt (EOI) to Master PIC
    outb(0x20, 0x20);

    // Ignore key release (Break code)
    if (scancode & 0x80)
        return;

    char c = scancode_table[scancode];

    if (c == '\b') {
        if (ind > 0) {
            backspace();
            print_backspace();
        }
    } else if (c == '\n') {
        putchar(c, LIGHT_GRAY);
        key_buffer[ind] = '\0';
        check_cmd(key_buffer);
        for (int i = 0; i < ind; i++)
            key_buffer[i] = '\0';
        ind = 0;
        print_string("> ", LIGHT_GRAY);
    } else {
        putchar(c, LIGHT_GRAY);
        key_buffer[ind++] = c;
    }
}

void keyboard_init()
{
    idt_set_gate(0x21, (uint32_t)keyboard_handler);
    print_string("Keyboard driver loaded successfully.\n", LIGHT_GRAY);
}