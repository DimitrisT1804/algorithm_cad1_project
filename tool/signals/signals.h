#include <signal.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_ORANGE  "\x1b[38;5;208m"
#define ANSI_COLOR_GREEN   "\x1b[32m"

extern volatile bool exit_requested;
extern volatile bool ctrl_c_pressed;

void segfault_handler(int signum);
void sigint_handler(int signum);
int isFolderEmpty(const char *path);
void removeFolder(const char *folderPath);
