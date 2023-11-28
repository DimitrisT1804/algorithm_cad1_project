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
            Gatepins_add(event, IO_TYPE);
            
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
            connection_pin = (char *) my_realloc(connection_pin, 1 * (size_event + strlen(event) + 2));
            strcat(connection_pin, " ");
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
                    Gatepins_add(connection_pin, WIRE);
                    //get_gatepin_indices(event, &ghash, &ghashdepth);
                }
                printf("Event is %s IOPin is %s and connection_pins is %s\n", event, IO_pin, connection_pin);
                Gatepin_reload(IO_pin, connection_pin);

                free(connection_pin);
                return CONNECTIONS_COMP;
            }

        default:
            return currentState;
    }
}

enum lib_parse proccesLib(enum lib_parse currentState, char *event) 
{
    int lhash, lhashdepth;
    static char name_of_cell[LINE_MAX];
    static char cell_type_name[LINE_MAX];
    static int cell_type = -1;

    //printf("Event is %s\n", event);
    switch (currentState) 
    {
        case WAIT:
            if(strcmp(event, "Cell_Type:") == 0)
            {
                return CELL_NAME;
            }
            else
            {
                return WAIT;
            }

        case CELL_NAME:
            printf("Cell Name is %s\n", event);
            strcpy(name_of_cell, event);
            return CELL_TIMING_TYPE;

        case CELL_TIMING_TYPE:
            if(strcmp(event, "Cell_Timing_Type:") == 0)
            {
                return GET_CELL_TYPE;
            }
            else
            {
               return CELL_TIMING_TYPE;
            }

        case GET_CELL_TYPE:
            // if(strcmp(event, "\n") == 0 || strcmp(event, " ") == 0)
            //     return WAIT;
            strcpy(cell_type_name, event);

            get_libhash_indices(name_of_cell, &lhash, &lhashdepth);
            if(lhashdepth == -1)    // does not exist! //
            {
                if(strcmp(cell_type_name, "Combinational") == 0)
                {
                    cell_type = COMBINATIONAL;
                }
                else if(strcmp(cell_type_name, "Sequential") == 0)
                {
                    cell_type = SEQUENTIAL;
                }
                Lib_add(name_of_cell, cell_type);
            }
            return WAIT;

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
                //printf("The bucket %d on depth %d is %s of type %d\n", i, j, gatepinhash[i].name[j], gatepinhash[i].type[j]);
                
                //if(strncmp(gatepinhash[i].name[j], "N", 1) == 0 || strncmp(gatepinhash[i].name[j], "clk", 3) == 0) 
                if(gatepinhash[i].type[j] == IO_TYPE)
                {
                    printf("IO: %s CCs: ", gatepinhash[i].name[j]);
                    for(k = 0; k < gatepinhash[i].connections_size[j] - 1; k++)
                    {
                        if(gatepinhash[gatepinhash[i].pinConn[j][k]].name[gatepinhash[i].pinConnDepth[j][k]] != NULL)
                            printf("%s ", gatepinhash[gatepinhash[i].pinConn[j][k]].name[gatepinhash[i].pinConnDepth[j][k]]);
                    }
                    printf("\n");
                }
            }
            //printf("-----------------------\n\n");
        }
    }
}

void print_libhash()
{
    int i, j;

    for (i = 0; i < LIBHASH_SIZE; i++)
    {
        for (j = 0; j < libhash[i].hashdepth - 1; j++)
        {
            if(libhash[i].name[j] != NULL)
            {
                if(libhash[i].cell_type[j] == 1)
                    printf("Cell is %s and type is Combinational\n", libhash[i].name[j]);
                else if (libhash[i].cell_type[j] == 2)
                    printf("Cell is %s and type is Sequential\n", libhash[i].name[j]);
                else
                    printf("ERROR TYPE\n");
            }
        }
    }
}

void call_parser(char *input_file)
{
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
    enum lib_parse currentState3 = WAIT;

    filename = fopen(input_file, "r");  // open file only for reading //

    if(filename == NULL)
    {
        printf("Cannot open file\n");
        return; // cannot open this file //
    }

    // initialize structs //
    Gatepins_init();
    Lib_init();

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

        while(j < strlen(line))
        {
            for(i = j; i < strlen(line)+1; i++)
            {
                if(line[i] == ' ' || line[i] == '\0'|| line[i] == '\v' )
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
            else if (flag == 3)
                currentState3 = proccesLib(currentState3, word);

            while (line[i] == ' ') 
            {
                i++;
            }
            j = i;
        }
    }

    print_gatepinhash();
    print_libhash();

    int count = 0, count_2 = 0;

    for(i = 0; i < HASH_SIZE; i++)
    {
        for(j = 0; j < gatepinhash[i].hashdepth - 1; j++)
        {
            if(gatepinhash[i].name[j] != NULL)
            {
                if(gatepinhash[i].type[j] == IO_TYPE)
                {
                    count++ ;
                }

            }
        }
    }
    printf("Count is %d and count_2 is %d\n", count, count_2);

    // Gatepins_free();
    fclose(filename);
}

// int main(int argc, char **argv)
// {
//     char *input_file = NULL;
//     FILE *filename;
//     char line[LINE_MAX];
//     char *test;
//     int flag = 0;
//     int i = 0;
//     char word[50] = {'\0'};
//     int pos = 0;
//     int j = 0;

//     enum DoorState currentState = START;
//     enum DoorState2 currentState2 = START;
//     enum lib_parse currentState3 = WAIT;

//     input_file = argv[1];


//     filename = fopen(input_file, "r");  // open file only for reading //

//     if(filename == NULL)
//     {
//         printf("Cannot open file\n");
//         return -1;  // cannot open this file //
//     }

//     // initialize structs //
//     Gatepins_init();
//     Lib_init();

//     while(fgets(line, sizeof(line)+1, filename) != NULL)
//     {
//         //line[sizeof(line)+1] = '\0';
//         j = 0;
//         test = strstr(line, "Top-Level I/O Ports:");
//         if(test != NULL)
//         {
//             printf("Test is %s\n", test);
//             flag = 1;  // set the flag that the following line has IO //
//         }

//         test = strstr(line, "Top-Level I/O CCs:");
//         if(test != NULL)
//         {
//             printf("Test is %s\n", test);
//             flag = 2;  // set the flag that the following line has IO //
//         }

//         test = strstr(line, "Components CCs:");
//         if(test != NULL)
//         {
//             printf("Test is %s\n", test);
//             flag = 3;  // set the flag that the following line has IO //
//         }

//         while(j < strlen(line))
//         {
//             for(i = j; i < strlen(line)+1; i++)
//             {
//                 if(line[i] == ' ' || line[i] == '\0'|| line[i] == '\v' )
//                 {
//                     word[pos] = '\0';
//                     break;
//                 }
//                 else
//                 {
//                     word[pos] = line[i];
//                     pos++ ;
//                 }
//             }
//             //printf("The word is %s\n", word);
//             pos = 0;
//             if(flag == 1)
//                 currentState = processEvent(currentState, word);
//             else if (flag == 2)
//                 currentState2 = processEvent2(currentState2, word);
//             else if (flag == 3)
//                 currentState3 = proccesLib(currentState3, word);

//             while (line[i] == ' ') 
//             {
//                 i++;
//             }
//             j = i;
//         }
//     }

//     print_gatepinhash();
//     print_libhash();

//     int count = 0, count_2 = 0;

//     for(i = 0; i < HASH_SIZE; i++)
//     {
//         for(j = 0; j < gatepinhash[i].hashdepth - 1; j++)
//         {
//             if(gatepinhash[i].name[j] != NULL)
//             {
//                 if(gatepinhash[i].type[j] == IO_TYPE)
//                 {
//                     count++ ;
//                 }

//             }
//         }
//     }
//     printf("Count is %d and count_2 is %d\n", count, count_2);

//     Gatepins_free();
//     fclose(filename);

//     return 0;

// }