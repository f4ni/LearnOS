#include <stdint.h>
#include "vga.h"
#include "io.h"

static volatile uint16_t* const VGA_MEMORY = (uint16_t*) VGA_ADDR;

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t default_color = WHITE;

static inline uint16_t vga_entry(char c, uint8_t color)
{
    return (uint16_t)c | (uint16_t)color << 8;
}

void clear_screen()
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        VGA_MEMORY[i] = vga_entry(' ', LIGHT_GRAY);

    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

void scroll()
{
    if (cursor_y < VGA_HEIGHT)
        return;

    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            int from = (y * VGA_WIDTH + x);
            int to   = ((y - 1) * VGA_WIDTH + x);

            VGA_MEMORY[to] = VGA_MEMORY[from];
        }
    }

    // clear last row
    for (int x = 0; x < VGA_WIDTH; x++)
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
            vga_entry(' ', default_color);

    cursor_y = VGA_HEIGHT - 1;
}

void print_backspace() {
    if (cursor_x > 0) {
        cursor_x--;
    } else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    } else {
        // nothing
    }

    VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(' ', default_color);
    update_cursor();
}

void update_cursor()
{
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, cursor_end);
    
    update_cursor();
}

void putchar(char c, uint8_t color)
{
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        scroll();
        update_cursor();
        return;
    }

    VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, color);
    cursor_x++;

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
        scroll();
    }
    update_cursor();
}

void print_string(const char* s, uint8_t color)
{
    while (*s)
        putchar(*s++, color);
}
