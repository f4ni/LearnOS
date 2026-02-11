# Keyboard Driver

This is a basic **PS/2 Keyboard Driver** for LearnOS, implementing basic scancode-to-ASCII translation.

## Implementation Details

### Hardware Interface
- **I/O Port**: `0x60` (Data Port)
- **IRQ**: `1`
- **Interrupt Vector**: `0x21` (Remapped via PIC)

### Features
- **Scancode Set 1**: Supports basic US keyboard layout.
- **Interrupt Driven**: Uses the IDT (Interrupt Descriptor Table) to handle key presses asynchronously.
- **Handling**: Currently handles key presses (Make codes) and ignores key releases (Break codes).

### How it works
1. When a key is pressed, the keyboard controller triggers **IRQ 1**.
2. The CPU looks up the interrupt handler at vector **0x21** in the IDT.
3. The assembly wrapper (`keyboard_handler` in `cpu/keyboard_handler.asm`) calls the C function `keyboard_handler_main`.
4. The C handler reads the scancode from port **0x60**.
5. The scancode is mapped to an ASCII character using a lookup table and printed to the screen via the VGA driver.

## Future Improvements
- Support for **Shift**, **Caps Lock**, and special keys.
- Input buffer (keyboard queue) for shell integration.
- Support for Scancode Set 2.
