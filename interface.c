#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>      // auta einai dika mou
#include <sys/file.h>                               //
#include <sys/stat.h>                               //
#include <sys/errno.h>                              //

#include <readline/readline.h>
#include <readline/history.h>

#define LINE_MAX 100        // auta einai dika mou

static const char *commands[] = {"hello", "kati", "help", "kat", "trial", "try", "testing1", "testing2", "test", "less", "ls", "quit", NULL};

char *custom_generator(const char *text, int state)
{
    static int list_index;      // should be static to has the same value in all iterations //
    static int len;
    //const char *match;

    if(!state)
    {
        list_index = 0;
        len = strlen(text);
    }

    // while (commands[list_index] != NULL)
    // {
    //     const char* match = commands[list_index];
    //     list_index++;

    //     if(strncmp(match, text, len) == 0)
    //     {
    //         return strdup(match);
    //     }
    // }

    while(commands[list_index] != NULL)
    {
        const char *match = commands[list_index];
        list_index++;
        if(strncmp(match, text, len) == 0)
        {
            return strdup(match);
        }
    }   

    return NULL;
}

char **custom_completer(const char *text, int start, int end)
{
    char **matches = NULL;
    // char **matches[100][100];
    int i;
    int matches_len = 0;

    // matches = (char**) malloc(1*sizeof(char*));
    // for(int i = 0; i < 100; i++)
    // {
    //     matches[i] = malloc(10*sizeof(char));
    // }

    if(text == NULL || text[0] == '\0')
    {
        matches = rl_completion_matches("", custom_generator);
    }
    else
    {
        for (i = 0; commands[i] != NULL; i++)
        {
            if(strncmp(text, commands[i], end-start) == 0)      // if the word matches with one in array
            {
                // if you add matches = rl_completion_matches(commands[i], custom_generator); it does not work correct //
                matches = rl_completion_matches(text, custom_generator);     // GNU Readline passes correct arguments on custom_generator //
                // char *match = custom_generator(text, 0);
                // if (match != NULL) 
                // {
                //     matches[matches_len] = realloc(matches, (matches_len + 2) * sizeof(char));
                //     matches[matches_len] == match;
                //     // matches[matches_len] = NULL;
                // }
            }
        }
    }

    return matches;
}


int main(int argc, char *argv[])
{
    char *text = NULL; // readline result //
    char *textexpansion; // readline result history expanded //
    int expansionresult;

    char file_name[LINE_MAX];

    HIST_ENTRY **the_history_list; // readline commands history list - NULL terminated //
    char command[LINE_MAX]; // current command //
    unsigned long i;
    // Readline Initialisation //
    rl_completion_entry_function = NULL; // use rl_filename_completion_function(), the default filename completer //
    rl_attempted_completion_function = custom_completer;
    rl_completion_append_character = '\0';
    using_history(); // initialise history functions //
    while (1)
    {
        text = readline("PR> ");
        if (text != NULL)
        {
            expansionresult = history_expand(text, &textexpansion);
            if ((expansionresult == 0) || // no expansion //
                (expansionresult == 2)) // do not execute //
            {
                add_history(text);
                strcpy(command, text); // store command //
            }
            else
            {
                add_history(textexpansion);
                strcpy(command, textexpansion); // store command //
            }
            free(textexpansion);
            free(text);
        }
        // handle two basic commands: history and quit //
        if (strcmp(command, "quit") == 0)
        {
            return EXIT_SUCCESS;
        }
        else if (strcmp(command, "history") == 0)
        {
            the_history_list = history_list(); // get history list //
            if (the_history_list != NULL)
            {
                i = 0;
                while (*(the_history_list + i) != NULL) // history list - NULL terminated //
                {
                    printf("%lu: %s\n", (i + history_base), (*(the_history_list + i))->line);
                    i++;
                }
            }
        }
        else if (strncmp(command, "less ", 5) == 0)
        {
            // scanf(" %s", file_name);
            // // sprintf()
            // printf("it is %s\n", file_name);
            // //sprintf(command, file_name);
            // strcat(command, " ");
            //strcat(command, file_name);
            system(command);
        }
        else if(strcmp(command, "ls") == 0)
        {
            system(command);
        }
    }
}
