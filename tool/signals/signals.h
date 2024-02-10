#include <signal.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_ORANGE  "\x1b[38;5;208m"

extern volatile bool exit_requested;
extern volatile bool ctrl_c_pressed;

void segfault_handler(int signum);
void sigint_handler(int signum);