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

enum DoorState2 
{
    START2,
    NAME2,
    CCS,
    CONNECTIONS
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

enum DoorState2 processEvent2(enum DoorState currentState, char *event) 
{
    switch (currentState) 
    {
        case START2:
            if(strcmp(event, "IO:") == 0)
            {
                return NAME2;
            }
            else
            {
                return START2;
            }

        case NAME2:
            printf("Name is %s\n", event);
            return CCS;

        case CCS:
            //printf("Name is %s\n", event);
            return CONNECTIONS;

        case CONNECTIONS:       // if there are no connections we shoul not take it
            if(strcmp(event, "IO:") == 0)
            {
                return NAME2;
            }
            else
            {
                printf("Connections is %s\n", event);
                return CONNECTIONS;
            }

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
    enum DoorState2 currentState2 = START;

    input_file = argv[1];


    filename = fopen(input_file, "r");  // open file only for reading //

    if(filename == NULL)
    {
        printf("Cannot open file\n");
        return -1;  // cannot open this file //
    }

    while(fgets(line, sizeof(line), filename) != NULL)
    {
        //line[sizeof(line)+1] = '\0';
        j = 0;
        test = strstr(line, "Top-Level I/O Ports:");
        if(test != NULL)
        {
            printf("Test is %s\n", test);
            flag = 1;  // set the flag that the following line has IO //
        }

        test = strstr(line, "Top-Level I/O CCs:");
        if(test != NULL)
        {
            printf("Test is %s\n", test);
            flag = 2;  // set the flag that the following line has IO //
        }
        //printf("line is %s\n", line);
        //printf("%s\n", line);

        while(j < strlen(line))
        {
            for(i = j; i < strlen(line)+1; i++)
            {
                if(line[i] == ' ' || line[i] == '\0')
                {
                    word[pos] = '\0';
                    break;
                }
                else
                {
                    word[pos] = line[i];
                    pos++ ;
                }
            }
            printf("The word is %s\n", word);
            pos = 0;
            if(flag == 1)
                currentState = processEvent(currentState, word);
            else if (flag == 2)
                currentState2 = processEvent2(currentState2, word);

            while (line[i] == ' ') 
            {
                i++;
            }
            j = i;
            //j = i;


            // if(line[i] == '\0')
            // {
            //     break;
            // }
        }
    }

    return 0;

}