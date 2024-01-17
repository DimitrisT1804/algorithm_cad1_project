#include "signals.h"

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