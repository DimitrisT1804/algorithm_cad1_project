// lab 2 //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enumerate the possible states
enum DoorState 
{
    START,
    IO,
    NAME
};

// Function to process events and transition between states
enum DoorState processEvent(enum DoorState currentState, char *event) 
{
    switch (currentState) 
    {
        case START:
            if(strcmp(event, "IO:") == 0)
            {
                return NAME;
            }
            else
            {
                return START;
            }

        case NAME:
            printf("Name is %s\n", event);
            return START;

        default:
            return currentState;
    }
}




int main(int argc, char **argv)
{
    char *input_file = NULL;
    FILE *filename;
    char line[100];
    char *test;
    int flag = 0;
    int i = 0;
    char word[50] = {'\0'};
    int pos = 0;
    int j = 0;

    enum DoorState currentState = START;

    input_file = argv[1];


    filename = fopen(input_file, "r");  // open file only for reading //

    if(filename == NULL)
    {
        printf("Cannot open file\n");
        return -1;  // cannot open this file //
    }

    while(fgets(line, sizeof(line), filename) != NULL)
    {
        j = 0;
        test = strstr(line, "Level I/O Ports:");
        if(test != NULL)
        {
            printf("Test is %s\n", test);
            flag = 1;  // set the flag that the following line has IO //
        }
        //printf("line is %s\n", line);
        //printf("%s\n", line);

        while(j < strlen(line))
        {
            for(i = j; i < strlen(line); i++)
            {
                if(line[i] == ' ' || line[i] == '\0')
                {
                    word[pos] = '\0';
                    break;
                }
                word[pos] = line[i];
                pos++ ;
            }
            //printf("The word is %s\n", word);
            pos = 0;

            currentState = processEvent(currentState, word);

            while (line[i] == ' ') 
            {
                i++;
            }
            j = i;
        }
    }

    return 0;

}