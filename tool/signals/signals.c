#include "signals.h"

volatile bool exit_requested;
volatile bool ctrl_c_pressed;

void segfault_handler(int signum) 
{
    fprintf(stderr, ANSI_COLOR_RED "It is a feaature not a bug!\n" ANSI_COLOR_RESET);
    
    // // Storage array for backtrace symbols
    // void* callstack[1000];
    // int frames = backtrace(callstack, sizeof(callstack) / sizeof(callstack[0]));

    // // Print backtrace symbols
    // char** symbols = backtrace_symbols(callstack, frames);
    // if (symbols != NULL) {
    //     for (int i = 0; i < frames; i++) {
    //         fprintf(stderr, "%s\n", symbols[i]);
    //     }
    //     free(symbols);
    // }

    exit(EXIT_FAILURE);
}

void sigint_handler(int signum) 
{
    printf(ANSI_COLOR_ORANGE "\nCTRL+C pressed. Do you want to exit? (y/n): " ANSI_COLOR_RESET);
    char response;
    scanf(" %c", &response);

    while (1) 
    {
        if (response == 'y' || response == 'Y') 
        {
            exit_requested = true;
            ctrl_c_pressed = true;
            break;
        } 
        else if (response == 'n' || response == 'N') 
        {
            ctrl_c_pressed = false;
            printf("Continuing...\nPR> ");
            break;
        } 
        else 
        {
            printf(ANSI_COLOR_RED "Incorrect answer! Please enter 'y' or 'n': " ANSI_COLOR_RESET);
            // Clear input buffer
            ctrl_c_pressed = false;
            while (getchar() != '\n');
            scanf(" %c", &response);
        }
    }
}

int isFolderEmpty(const char *path) 
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) 
    {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) 
    {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            closedir(dir);
            return 0;  // Folder is not empty
        }
    }

    closedir(dir);
    return 1;  // Folder is empty
}

void removeFolder(const char *folderPath) 
{
    printf(ANSI_COLOR_ORANGE"Do you want to remove the folder '%s' that created to store BDDs? (yes/no): " ANSI_COLOR_RESET, folderPath);

    char response[10];
    scanf("%9s", response);

    if (strcmp(response, "yes") == 0) 
    {
        if (rmdir(folderPath) == 0) 
        {
            printf(ANSI_COLOR_GREEN "Folder '%s' removed successfully.\n" ANSI_COLOR_RESET, folderPath);
        } 
        else 
        {
            perror(ANSI_COLOR_RED "Error removing folder" ANSI_COLOR_RESET);
        }
    } 
    else 
    {
        printf(ANSI_COLOR_RED "Folder removal canceled.\n" ANSI_COLOR_RESET);
    }
}
