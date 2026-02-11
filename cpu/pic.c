#include "pic.h"

void pic_remap()
{
    // Start initialization of both PICs (ICW1)
    // 0x20 = command port for Master PIC
    // 0xA0 = command port for Slave PIC
    // 0x11 = initialize + expect ICW4 + cascade mode
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2: Set interrupt vector offsets
    // Master PIC will map IRQs 0-7 to interrupts starting at 0x20 (32)
    // Slave PIC will map IRQs 8-15 to interrupts starting at 0x28 (40)
    // This avoids conflicts with CPU exceptions (0–31)
    outb(0x21, 0x20);  
    outb(0xA1, 0x28);

    // ICW3: Tell the PICs how they are wired together
    // Master PIC: slave is connected on IRQ2 (00000100 = bit 2)
    // Slave PIC: cascade identity is 2 (connected to master's IRQ2)
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4: Set operating mode
    // 0x01 = 8086/88 mode (required for modern protected-mode kernels)
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // OCW1: Interrupt masks (which IRQ lines are enabled/disabled)
    // Bit = 1 → masked (disabled)
    // Bit = 0 → unmasked (enabled)

    // Master mask = 00000001
    // Disables IRQ0 (system timer), enables others
    outb(0x21, 0x01);

    // Slave mask = 00000000
    // Enables ALL slave interrupts (IRQ8–15)
    outb(0xA1, 0x00);
}
