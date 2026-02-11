#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_ADDR 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define LIGHT_GRAY 0x07
#define LIGHT_RED 0x0C
#define YELLOW 0x0E
#define WHITE 0x0F

void clear_screen();
void scroll();
void print_backspace();
void putchar(char c, uint8_t color);
void print_string(const char* s, uint8_t color);
#endif
