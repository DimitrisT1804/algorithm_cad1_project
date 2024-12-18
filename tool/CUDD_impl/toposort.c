// Toposort algorithm source file //
#include "toposort.h"

Gatepin_pos *sorted_gatepins;   // L //
Gatepin_pos *input_nodes;       // S //

Gatepin_pos* init_gatepin_pos(Gatepin_pos *gatepin)
{
    gatepin = (Gatepin_pos *) malloc(sizeof(Gatepin_pos));
    gatepin->ghash = NULL;
    gatepin->gdepth = NULL;
    gatepin->size = 0;

    return gatepin;
}

void add_gatepin_pos(Gatepin_pos *gatepin, int ghash, int gdepth)
{
    // Increase size by 1
    gatepin->size++;

    // Reallocate memory for ghash and gdepth
    gatepin->ghash = (int *)realloc(gatepin->ghash, sizeof(int) * gatepin->size);
    gatepin->gdepth = (int *)realloc(gatepin->gdepth, sizeof(int) * gatepin->size);

    // Shift existing elements to make room for the new element
    for (int i = gatepin->size - 1; i > 0; i--) 
    {
        gatepin->ghash[i] = gatepin->ghash[i - 1];
        gatepin->gdepth[i] = gatepin->gdepth[i - 1];
    }

    // Insert the new element at the first position
    gatepin->ghash[0] = ghash;
    gatepin->gdepth[0] = gdepth;
}


void remove_gatepin_pos(Gatepin_pos *gatepin, int ghash, int gdepth)
{
    int i, j;
    for (i = 0; i < gatepin->size; i++)
    {
        if (gatepin->ghash[i] == ghash && gatepin->gdepth[i] == gdepth)
        {
            for (j = i; j < gatepin->size - 1; j++)
            {
                gatepin->ghash[j] = gatepin->ghash[j + 1];
                gatepin->gdepth[j] = gatepin->gdepth[j + 1];
            }
            gatepin->size--;
            break;
        }
    }
}

void check_visited(int ghash, int gdepth)
{
    int i;
    int curr_ghash;
    int curr_gdepth;

    for(i = 0; i < gatepinhash[ghash].connections_size[gdepth]; i++)
    {
        curr_ghash = gatepinhash[ghash].pinConn[gdepth][i];
        curr_gdepth = gatepinhash[ghash].pinConnDepth[gdepth][i];
        if(gatepinhashv[curr_ghash].isVisited[curr_gdepth] == 1)
        {
            continue;
        }
        else
        {
            printf("Gatepin %s has still not visited connections\n", gatepinhash[curr_ghash].name[curr_gdepth]);
            return;
        }
    }
}

void free_gatepin_pos(Gatepin_pos *gatepin)
{
    if(gatepin == NULL)
    {
        return;
    }
    free(gatepin->ghash);
    free(gatepin->gdepth);
    free(gatepin);
}


Gatepin_pos *toposort(int *startpoins_ghash, int *startpoints_gdepth)
{
    int ghash;
    int gdepth;
    int i;
    int j;
    int curr_ghash;
    int curr_gdepth;
    int chash;
    int cdepth;
    int lhash;
    int ldepth;
    char new_gatepin[100] = {"\0"};
    int size = 0;
    char *curr_pin;
    int k;
    curr_pin = (char *) malloc(1000 * sizeof(char));

    Gatepin_pos *L = NULL;
    L = init_gatepin_pos(L);
    Gatepin_pos *S = NULL;
    S = init_gatepin_pos(S);

    for(i = 0; startpoins_ghash[i] != -1; i++)
    {
        add_gatepin_pos(S, startpoins_ghash[i], startpoints_gdepth[i]);
    }
    S->size = i;

    while(S->size != 0)
    {
        // if we already visited this pin, just remove it //
        if(gatepinhashv[S->ghash[S->size - 1]].isVisited[S->gdepth[S->size - 1]] == 1)
        {
            remove_gatepin_pos(S, S->ghash[S->size - 1], S->gdepth[S->size - 1]);
            continue;
        }
        add_gatepin_pos(L, S->ghash[S->size - 1], S->gdepth[S->size - 1]);
        remove_gatepin_pos(S, S->ghash[S->size - 1], S->gdepth[S->size - 1]);
        
        ghash = L->ghash[0];
        gdepth = L->gdepth[0];
        curr_pin [0] = '\0';
        size = 0;
        new_gatepin[0] = '\0';  

        gatepinhashVisited_make_visited(ghash, gdepth);

        if(gatepinhash[ghash].type[gdepth] != IO_TYPE && gatepinhash[ghash].type[gdepth] != PO)
        {            
            chash = gatepinhash[ghash].parentComponent[gdepth];
            cdepth = gatepinhash[ghash].parentComponentDepth[gdepth];

            lhash = comphash[chash].lib_type[cdepth];
            ldepth = comphash[chash].lib_type_depth[cdepth];
            if(libhash[lhash].cell_type[ldepth] == SEQUENTIAL)
            {
                for(k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
                {
                    if(libhash[lhash].pin_type[ldepth][k] == OUTPUT)
                    {
                        strcpy(new_gatepin, comphash[chash].name[cdepth]);
                        strcat(new_gatepin, libhash[lhash].pin_names[ldepth][k]);
                        break;
                    }
                }
                // it is input of Sequential, so it is endpoint //
                if(strcmp(gatepinhash[ghash].name[gdepth], new_gatepin) != 0)
                {
                    continue;
                }
            }
        }
                
        for(i = 0; i < gatepinhash[ghash].connections_size[gdepth]; i++)
        {
            if(gatepinhash[ghash].type[gdepth] == PO || gatepinhash[ghash].type[gdepth] == IO_TYPE)
            {                
                curr_ghash = gatepinhash[ghash].pinConn[gdepth][i];
                curr_gdepth = gatepinhash[ghash].pinConnDepth[gdepth][i];

                chash = gatepinhash[curr_ghash].parentComponent[curr_gdepth];
                cdepth = gatepinhash[curr_ghash].parentComponentDepth[curr_gdepth];
                lhash = comphash[chash].lib_type[cdepth];
                ldepth = comphash[chash].lib_type_depth[cdepth];

                for(k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
                {
                    strcpy(curr_pin, comphash[chash].name[cdepth]);
                    strcat(curr_pin, libhash[lhash].pin_names[ldepth][k]); 
                    if(strcmp(curr_pin, gatepinhash[curr_ghash].name[curr_gdepth]) == 0)
                    {
                        break;
                    }
                }
                if(libhash[lhash].pin_type[ldepth][k] == INPUT)
                {
                    add_gatepin_pos(S, curr_ghash, curr_gdepth);
                }           
            }
            else
            {
                curr_ghash = gatepinhash[ghash].pinConn[gdepth][i];
                curr_gdepth = gatepinhash[ghash].pinConnDepth[gdepth][i];
            
                add_gatepin_pos(S, curr_ghash, curr_gdepth);
            }
        }

        if(gatepinhash[ghash].type[gdepth] == WIRE)
        {
            #ifdef DEBUG
            printf("proccessing: %s\n", gatepinhash[ghash].name[gdepth]);
            #endif

            if(gatepinhash[ghash].connections_size[gdepth] == 0)    // it is input //
            {
                chash = gatepinhash[ghash].parentComponent[gdepth];
                cdepth = gatepinhash[ghash].parentComponentDepth[gdepth];
                

                lhash = comphash[chash].lib_type[cdepth];
                ldepth = comphash[chash].lib_type_depth[cdepth];

                for(j = 0; j < libhash[lhash].pin_count[ldepth]; j++)
                {
                    size = 0;
                    if(libhash[lhash].pin_type[ldepth][j] == OUTPUT)
                    {                      
                        strcpy(new_gatepin, comphash[chash].name[cdepth]);
                        strcat(new_gatepin, libhash[lhash].pin_names[ldepth][j]);
                        // break;
                        for (k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
                        {
                            if (libhash[lhash].pin_type[ldepth][k] == INPUT)
                            {
                                strcpy(curr_pin, comphash[chash].name[cdepth]);
                                strcat(curr_pin, libhash[lhash].pin_names[ldepth][k]); 

                                get_gatepin_indices(curr_pin, &curr_ghash, &curr_gdepth);
                                if(gatepinhashv[curr_ghash].isVisited[curr_gdepth] == 1)
                                {
                                    size++;
                                }
                            }
                        }
                        /* if we found input pin, we check the output pin and if all pins
                        are visited, we can add this output pin to L */
                        
                        if(size == libhash[lhash].pin_count[ldepth] - libhash[lhash].out_pins_count[ldepth])
                        {
                            get_gatepin_indices(new_gatepin, &ghash, &gdepth);
                            add_gatepin_pos(S, ghash, gdepth);
                        }
                    }
                }
            }
        }
    }
    free_gatepin_pos(S);
    free(curr_pin);
    if(L != NULL)
    {
        return L;
    }
    return NULL;
}

// !!! Old implementation for assign_level_gatepins --> is really slow !!! //

// void assign_level_gatepins(Gatepin_pos *L)
// {
//     int i;
//     int chash;
//     int cdepth;
//     int lhash;
//     int ldepth;
//     int ghash;
//     int gdepth;
//     int max_level;
//     int j;
//     char *curr_gatepin = NULL;
//     int input_ghash;
//     int input_gdepth;

//     if(L == NULL)
//     {
//         printf("Error: L is NULL\n");
//         return;
//     }

//     for(i = L->size - 1; i >= 0; i--)
//     {
//         max_level = 0;
//         if(gatepinhash[L->ghash[i]].type[L->gdepth[i]] == IO_TYPE || gatepinhash[L->ghash[i]].type[L->gdepth[i]] == PO)
//         {
//             gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = 0;
//         }
//         else
//         {
            
//             chash = gatepinhash[L->ghash[i]].parentComponent[L->gdepth[i]];
//             cdepth = gatepinhash[L->ghash[i]].parentComponentDepth[L->gdepth[i]];

//             lhash  = comphash[chash].lib_type[cdepth];
//             ldepth = comphash[chash].lib_type_depth[cdepth];

//             if(libhash[lhash].pin_count[ldepth] == 1)   // it is logic_0 or logic_1 //
//             {
//                 gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = 0;
//                 continue;
//             }

//             get_predecessors_pin(gatepinhash[L->ghash[i]].name[L->gdepth[i]], &ghash, &gdepth);
//             // it is output or Primary Output //
//             if( (gdepth == -1) || (gatepinhash[ghash].type[gdepth] == PO))
//             {
//                 //printf("Warning: %s is not connected to any other gate\n", gatepinhash[L->ghash[i]].name[L->gdepth[i]]);


//                 // if it is Output of Sequential we need to assign leve 0 //
//                 if(libhash[lhash].cell_type[ldepth] == SEQUENTIAL)
//                 {
//                     curr_gatepin = (char *) realloc(curr_gatepin, sizeof(char) * (strlen(comphash[chash].name[cdepth]) + 1));
//                     strcpy(curr_gatepin, comphash[chash].name[cdepth]);
//                     // strcat(curr_gatepin, libhash[lhash].pin_names[ldepth][j]);

//                     for(j = 0; j < libhash[lhash].pin_count[ldepth]; j++)
//                     {
//                         if(libhash[lhash].pin_type[ldepth][j] == OUTPUT)
//                         {
//                             curr_gatepin = (char *) realloc(curr_gatepin, sizeof(char) * (strlen(comphash[chash].name[cdepth]) + strlen(libhash[lhash].pin_names[ldepth][j]) + 1));
//                             strcat(curr_gatepin, libhash[lhash].pin_names[ldepth][j]);
//                             break;
//                         }
//                     }
//                     if(strcmp(curr_gatepin, gatepinhash[L->ghash[i]].name[L->gdepth[i]]) == 0)
//                     {
//                         gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = 0;
//                         free(curr_gatepin);
//                         curr_gatepin = NULL;
//                         continue;
//                     }
//                 }

//                 for(j = 0; j < libhash[lhash].pin_count[ldepth]; j++)
//                 {
//                     curr_gatepin = (char *) realloc(curr_gatepin, sizeof(char) * (strlen(comphash[chash].name[cdepth]) + strlen(libhash[lhash].pin_names[ldepth][j]) + 1));
//                     strcpy(curr_gatepin, comphash[chash].name[cdepth]);
//                     strcat(curr_gatepin, libhash[lhash].pin_names[ldepth][j]);
//                     get_gatepin_indices(curr_gatepin, &input_ghash, &input_gdepth);

//                     max_level = max(max_level, gatepinhashv[input_ghash].level[input_gdepth]);
//                 }
//                 gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = max_level + 1;
//                 free(curr_gatepin);
//                 curr_gatepin = NULL;
//             }
//             else // it is input //
//             {
//                 gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = gatepinhashv[ghash].level[gdepth] + 1;
//             }
//         }
//     }
// }



void assign_level_gatepins(Gatepin_pos *L)
{
    int i;
    int chash;
    int cdepth;
    int lhash;
    int ldepth;
    int is_input = 0;
    int level = 0;

    if(L == NULL)
    {
        printf("Error: L is NULL\n");
        return;
    }

    for(i = L->size - 1; i >= 0; i--)
    {
        // max_level = 0;
        if(gatepinhash[L->ghash[i]].type[L->gdepth[i]] == IO_TYPE || gatepinhash[L->ghash[i]].type[L->gdepth[i]] == PO)
        {
            gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = 0;
            is_input = 0;
        }
        else
        {
            chash = gatepinhash[L->ghash[i]].parentComponent[L->gdepth[i]];
            cdepth = gatepinhash[L->ghash[i]].parentComponentDepth[L->gdepth[i]];
            lhash = comphash[chash].lib_type[cdepth];
            ldepth = comphash[chash].lib_type_depth[cdepth];

            if(strcmp(libhash[lhash].function[ldepth][0], "1") == 0)
            {
                gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = 0;
                level = 0;
                is_input = 0;
                continue;
            }
            if(strcmp(libhash[lhash].function[ldepth][0], "0") == 0)
            {
                gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = 0;
                level = 0;
                is_input = 0;
                continue;
            }

            if(check_gatepin_type(L->ghash[i], L->gdepth[i]) == 1)
            {
                if(libhash[lhash].cell_type[ldepth] == SEQUENTIAL)
                {
                    gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = 0;
                    level = 0;
                    continue;
                }
                if(is_input == 1 || level == 0) 
                {
                    level++;
                }
                gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = level;
                is_input = 0;
            }
            else
            {
                if(is_input == 0 || level == 0)
                {
                    level++;
                }
                gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = level;
                is_input = 1;
            }
        }
    }
}


void add_startpoints()
{
    int *startpoint_ghash = NULL;
    int *startpoint_gdepth = NULL;
    int i = 0;
    int ghash;
    int gdepth;
    int max_level = 0;
    char *curr_gatepin = NULL;
    int k;
    int chash;
    int cdepth;
    int lhash;
    int ldepth;
    int j = 0;

    clock_t start, end;

    start = clock();

    Gatepin_pos *L = NULL;

    for(ghash = 0; ghash < gatepinhash_size; ghash++)
    {
        for(gdepth = 0; gdepth < HASHDEPTH; gdepth++)
        {
            if(gatepinhash[ghash].hashpresent[gdepth] != 0)
            {
                if(gatepinhash[ghash].type[gdepth] == IO_TYPE)
                {
                    startpoint_ghash = (int *) realloc(startpoint_ghash, sizeof(int) * (i + 1));
                    startpoint_gdepth = (int *) realloc(startpoint_gdepth, sizeof(int) * (i + 1));
                
                    startpoint_ghash[i] = ghash;
                    startpoint_gdepth[i] = gdepth;  
                    i++;
                }
                else if (gatepinhash[ghash].type[gdepth] != PO)
                {
                    chash = gatepinhash[ghash].parentComponent[gdepth];
                    cdepth = gatepinhash[ghash].parentComponentDepth[gdepth];
                    lhash = comphash[chash].lib_type[cdepth];
                    ldepth = comphash[chash].lib_type_depth[cdepth];
                    if(libhash[lhash].pin_count[ldepth] == 1)   // it is logic_0 or logic_1 //
                    {
                        startpoint_ghash = (int *) realloc(startpoint_ghash, sizeof(int) * (i + 1));
                        startpoint_gdepth = (int *) realloc(startpoint_gdepth, sizeof(int) * (i + 1));
                    
                        startpoint_ghash[i] = ghash;
                        startpoint_gdepth[i] = gdepth;  
                        i++;
                    }
                    else if (libhash[lhash].cell_type[ldepth] == SEQUENTIAL)    // only Outputs //
                    {
                        curr_gatepin = (char *) realloc(curr_gatepin, sizeof(char) * (strlen(comphash[chash].name[cdepth]) + 4));   // for D pin //
                        strcpy(curr_gatepin, comphash[chash].name[cdepth]);
                        strcat(curr_gatepin, "/Q");

                        if(strcmp(gatepinhash[ghash].name[gdepth], curr_gatepin) == 0)
                        {
                            startpoint_ghash = (int *) realloc(startpoint_ghash, sizeof(int) * (i + 1));
                            startpoint_gdepth = (int *) realloc(startpoint_gdepth, sizeof(int) * (i + 1));
                        
                            startpoint_ghash[i] = ghash;
                            startpoint_gdepth[i] = gdepth;  
                            i++;
                        }
                        free(curr_gatepin);
                        curr_gatepin = NULL;
                    }
                }
            }
        }
    }

    startpoint_ghash = (int *) realloc(startpoint_ghash, sizeof(int) * (i + 1));
    startpoint_gdepth = (int *) realloc(startpoint_gdepth, sizeof(int) * (i + 1));

    startpoint_ghash[i] = -1;

    L = toposort(startpoint_ghash, startpoint_gdepth);

    // levelize gatepins //
    assign_level_gatepins(L);

    for(i = 0; i < L->size; i++)
    {
        check_visited(L->ghash[i], L->gdepth[i]);
    }
    j = 0;

    // print toposort for debug //
    #ifdef DEBUG
    printf("Toposort is:\n");
    for(i = 0; i < L->size; i++)
    {
        printf("No: %d is %s and level is %d\n", i+1, gatepinhash[L->ghash[i]].name[L->gdepth[i]], gatepinhashv[L->ghash[i]].level[L->gdepth[i]]);
    }
    #endif

    // Sort the pins by level in ascending order //
    max_level = 0;

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                if(gatepinhashv[i].level[j] == -1)
                {
                    if(gatepinhash[i].type[j] == PO)
                    {
                        for(k = 0; k < gatepinhash[i].connections_size[j]; k++)
                        {
                            ghash = gatepinhash[i].pinConn[j][k];
                            gdepth = gatepinhash[i].pinConnDepth[j][k];

                            if(check_gatepin_type(ghash, gdepth) == 1)
                            {
                                gatepinhashv[i].level[j] = gatepinhashv[ghash].level[gdepth] + 1;
                            }
                            else
                            {
                                gatepinhashv[i].level[j] = gatepinhashv[ghash].level[gdepth];
                            }
                        }
                    }
                }
                
            }
        }
    }

    max_level = 0;

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                max_level = max(max_level, gatepinhashv[i].level[j]);
            }
        }
    }
    max_design_level = max_level;

    #ifdef DEBUG
    for (int level = -1; level <= max_level; level++) 
    {
        for (i = 0; i < gatepinhash_size; i++) 
        {
            for(j = 0; j < HASHDEPTH; j++)
            {
                if(gatepinhash[i].hashpresent[j] != 0)
                {
                    if (gatepinhashv[i].level[j] == level) 
                    {
                        printf("No: %d is %s and level is %d\n", i+1, gatepinhash[i].name[j], gatepinhashv[i].level[j]);
                    }
                }
            }
        }
    }
    #endif

    printf(ANSI_COLOR_BLUE "Max depth of design is: %d\n" ANSI_COLOR_RESET, max_level);

    free_gatepin_pos(L);
    free(startpoint_ghash);
    free(startpoint_gdepth);
    free(curr_gatepin);

    end = clock();
    printf(ANSI_COLOR_ORANGE "Time calculation for toposort is %f\n" ANSI_COLOR_RESET, (double)(end - start) / CLOCKS_PER_SEC);
}

int max(int a, int b)
{
    if (a > b) 
    {
        return a;
    } 
    else 
    {
        return b;
    }
}
