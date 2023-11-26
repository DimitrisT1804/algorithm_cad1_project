// lab 2 //
#include "parser.h"


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
            Gatepins_add(event);
            
            return START;

        default:
            return currentState;
    }
}

enum DoorState2 processEvent2(enum DoorState2 currentState, char *event) 
{
    int ghash, ghashdepth;
    static char IO_pin[LINE_MAX];
    static char *connection_pin;
    static int size_event;

    //printf("Event is %s\n", event);
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
            strcpy(IO_pin, event);
            return CCS;

        case CCS:
            //printf("Name is %s\n", event);
            return CONNECTIONS_COMP;

        case CONNECTIONS_COMP:
            if(strcmp(event, "\n") == 0 || strcmp(event, " ") == 0)
                return START2;
            size_event = strlen(event);
            connection_pin = my_calloc(1, strlen(event) + 1);
            strcpy(connection_pin, event);

            return CONNECTIONS;

        case CONNECTIONS:       // if there are no connections we should not take it !!!!!
            connection_pin = (char *) my_realloc(connection_pin, 1 * (size_event + strlen(event) + 1));
            strcat(connection_pin, event);  // concut strings together to create connection pin //
            if(strcmp(event, "IO:") == 0)
            {
                return NAME2;
            }
            else if (strncmp(event, "#", 1) == 0)
            {
                return START2;
            }
            else
            {
                printf("Connections is %s\n", connection_pin);
                get_gatepin_indices(connection_pin, &ghash, &ghashdepth);
                if(ghashdepth == -1)    // it does not exist in hash //
                {
                    Gatepins_add(connection_pin);
                    //get_gatepin_indices(event, &ghash, &ghashdepth);
                }
                printf("Event is %s IOPin is %s and connection_pins is %s\n", event, IO_pin, connection_pin);
                Gatepin_reload(IO_pin, connection_pin);

                return CONNECTIONS_COMP;
            }

        default:
            return currentState;
    }
}


void print_gatepinhash()
{
    int i, j, k;

    for (i = 0; i < HASH_SIZE; i++)
    {
        for(j = 0; j < gatepinhash[i].hashdepth - 1; j++)
        {
            if(gatepinhash[i].name[j] != NULL)
            {
                printf("The bucket %d on depth %d is %s of type %d\n", i, j, gatepinhash[i].name[j], gatepinhash[i].type[j]);
                for(k = 0; k < gatepinhash[i].connections_size[j] - 1; k++)
                {
                    if(gatepinhash[gatepinhash[i].pinConn[j][k]].name[gatepinhash[i].pinConnDepth[j][k]] != NULL)
                        printf("It has connections %s\n", gatepinhash[gatepinhash[i].pinConn[j][k]].name[gatepinhash[i].pinConnDepth[j][k]]);
                }
            }
            printf("-----------------------\n\n");
        }
    }
}

int main(int argc, char **argv)
{
    char *input_file = NULL;
    FILE *filename;
    char line[LINE_MAX];
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

    Gatepins_init();

    while(fgets(line, sizeof(line)+1, filename) != NULL)
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

        test = strstr(line, "Components CCs:");
        if(test != NULL)
        {
            printf("Test is %s\n", test);
            flag = 3;  // set the flag that the following line has IO //
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
            //printf("The word is %s\n", word);
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

    print_gatepinhash();

    int count = 0;

    for(i = 0; i < HASH_SIZE; i++)
    {
        for(j = 0; j < gatepinhash[i].hashdepth - 1; j++)
        {
            if(gatepinhash[i].name[j] != NULL)
            {
                if(strncmp(gatepinhash[i].name[j], "N", 1) == 0)
                {
                    count++;
                }
            }
        }
    }
    printf("Count is %d\n", count);

    return 0;

}