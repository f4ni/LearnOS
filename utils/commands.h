#include "string.h"

const char *cmds[] = {"exit", "uname", "author", "mode"};

void check_cmd(const char *str)
{
    if (strcmp(str, cmds[0]) == 0) {
        asm volatile("hlt");
        return;
    } else if (strcmp(str, cmds[1]) == 0) {
        print_string("LearnOS\n", YELLOW);
        return;
    } else if (strcmp(str, cmds[2]) == 0) {
        print_string("Farhan Tariq\n", YELLOW);
        return;
    } else if (strcmp(str, cmds[3]) == 0) {
        print_string("32-bit Protected Mode\n", YELLOW);
        return;
    } else
        print_string("Unknown command\n", LIGHT_RED);
}
