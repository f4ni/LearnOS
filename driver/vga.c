#include <stdint.h>
#include "vga.h"

static volatile uint16_t* const VGA_MEMORY = (uint16_t*) VGA_ADDR;

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t curr_color = WHITE;

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
            vga_entry(' ', curr_color);

    cursor_y = VGA_HEIGHT - 1;
}

void putchar(char c, uint8_t color)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
        scroll();
        return;
    }

    VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] =
        vga_entry(c, color);

    cursor_x++;

    if (cursor_x >= VGA_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
        scroll();
    }
}

void print_string(const char* s, uint8_t color)
{
    while (*s)
        putchar(*s++, color);
}
