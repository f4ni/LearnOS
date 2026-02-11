#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// Segment selectors
#define KERNEL_CS 0x08
#define IDT_ENTRIES 256

#define low_16(address) (uint16_t)(address & 0xFFFF)
#define high_16(address) (uint16_t)((address >> 16) & 0xFFFF)

// Interrupt gate entry (handler)
typedef struct {
    uint16_t low_offset; // Lower 16 bits of handler function address
    uint16_t sel;        // Kernel segment selector
    uint8_t  always0;
    uint8_t  flags; // Gate type, dpl, and p fields
    uint16_t high_offset; // Higher 16 bits of handler function address
} __attribute__((packed)) idt_gate_t;

// IDTR - pointer to the array of interrupt handlers (or to theIDT).
// This is what 'lidt' uses.
typedef struct {
    uint16_t limit; // Size of the IDT in bytes
    uint32_t base; // Base (address) of IDT
} __attribute__((packed)) idt_register_t;

void idt_set_gate(int n, uint32_t handler);
void idt_load();

#endif
