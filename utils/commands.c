#include "commands.h"

const command_t commands[] = {
    {"help",   cmd_help,   "Display Help"},
    {"exit",   cmd_exit,   "Halt CPU"},
    {"reboot", cmd_reboot, "Restart system"},
    {"uname",  cmd_uname,  "Print OS name"},
    {"author", cmd_author, "Show author"},
    {"mode",   cmd_mode,   "Show CPU mode"},
    {"about",  cmd_about,  "OS info"},
    {"clear",  cmd_clear,  "Clear screen"},
};

void cmd_help(void)
{
    print_string("Help menu:\n", YELLOW);
    for (int i = 0; i < CMD_COUNT; i++) {
        print_string(" - ", YELLOW);
        print_string(commands[i].name, YELLOW);
        print_string(": ", YELLOW);
        print_string(commands[i].desc, YELLOW);
        print_string("\n", YELLOW);
    }
    print_string("\n", YELLOW);
}

void cmd_exit(void)
{
    asm volatile("cli");
    asm volatile("hlt");
}

void reboot(void)
{
    while (inb(0x64) & 0x02);
    outb(0x64, 0xFE);

    // if reboot fails
    asm volatile("cli");
    for (;;)
        asm volatile("hlt");
}


void cmd_reboot(void)
{
    print_string("Rebooting...\n", YELLOW);
    reboot();
}

void cmd_uname(void)
{
    print_string(OS, YELLOW);
    print_string("\n", YELLOW);
}

void cmd_author(void)
{
    print_string(AUTHOR, YELLOW);
    print_string("\n", YELLOW);
}

void cmd_mode(void)
{
    print_string(MODE, YELLOW);
    print_string("\n", YELLOW);
}

void cmd_about(void)
{
    print_string(OS, YELLOW);
    print_string(" v", YELLOW);
    print_string(OS_VER, YELLOW);
    print_string("\nBuilt by ", YELLOW);
    print_string(AUTHOR, YELLOW);
    print_string("\n32-bit x86 Kernel\n", YELLOW);
}

void cmd_clear(void)
{
    clear_screen();
}

void check_cmd(const char *str)
{
    for (int i = 0; i < CMD_COUNT; i++) {
        if (strcmp(str, commands[i].name) == 0) {
            commands[i].func();
            return;
        }
    }

    print_string(str, LIGHT_RED);
    print_string(": Unknown command\n", LIGHT_RED);
}
