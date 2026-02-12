#include "io.h"
#include "vga.h"
#include "string.h"

#define AUTHOR "Farhan Tariq"
#define OS "LearnOS"
#define OS_VER "0.1"
#define MODE "32-bit Protected Mode"

typedef struct {
    const char *name;
    void (*func)(void);        // pointer to function
    const char *desc;      // optional (for help command)
} command_t;

#define CMD_COUNT (sizeof(commands) / sizeof(command_t))

void cmd_help(void);
void cmd_exit(void);
void reboot(void);
void cmd_reboot(void);
void cmd_uname(void);
void cmd_author(void);
void cmd_mode(void);
void cmd_about(void);
void cmd_clear(void);
void check_cmd(const char *str);
