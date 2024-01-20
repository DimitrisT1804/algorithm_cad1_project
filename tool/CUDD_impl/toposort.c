// Toposort algorithm source file //
#include "toposort.h"

Gatepin_pos* init_gatepin_pos(Gatepin_pos *gatepin)
{
    gatepin = (Gatepin_pos *) malloc(sizeof(Gatepin_pos));
    gatepin->ghash = NULL;
    gatepin->gdepth = NULL;
    gatepin->size = 0;

    return gatepin;
}

// void add_gatepin_pos(Gatepin_pos *gatepin, int ghash, int gdepth)
// {
//     gatepin->ghash = (int *) realloc(gatepin->ghash, sizeof(int) * (gatepin->size + 1));
//     gatepin->gdepth = (int *) realloc(gatepin->gdepth, sizeof(int) * (gatepin->size + 1));
//     gatepin->ghash[gatepin->size] = ghash;
//     gatepin->gdepth[gatepin->size] = gdepth;

//     gatepin->size++;
// }

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

void free_gatepin_pos(Gatepin_pos *gatepin)
{
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
    curr_pin = (char *) malloc(100 * sizeof(char));

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

        if(gatepinhash[ghash].type[gdepth] != IO_TYPE)
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
                if(strcmp(gatepinhash[ghash].name[gdepth], new_gatepin) != 0)
                {
                    continue;
                }
            }
        }
                
        for(i = 0; i < gatepinhash[ghash].connections_size[gdepth]; i++)
        {
            // remove connection of this pin with current one //
            // make this pin as visited //
            curr_ghash = gatepinhash[ghash].pinConn[gdepth][i];
            curr_gdepth = gatepinhash[ghash].pinConnDepth[gdepth][i];
        
            // make this one visited //
            // if(gatepinhashv[curr_ghash].isVisited[curr_gdepth] == 1)
            // {
            //     continue;
            // }
            // gatepinhashVisited_make_visited(curr_ghash, curr_gdepth);
            add_gatepin_pos(S, curr_ghash, curr_gdepth);
        }
        // add_gatepin_pos(L, ghash, gdepth);

        if(gatepinhash[ghash].type[gdepth] == WIRE)
        {
            if(gatepinhash[ghash].connections_size[gdepth] == 0)    // einai input //
            {
                chash = gatepinhash[ghash].parentComponent[gdepth];
                cdepth = gatepinhash[ghash].parentComponentDepth[gdepth];
                
                strcpy(new_gatepin, comphash[chash].name[cdepth]);

                lhash = comphash[chash].lib_type[cdepth];
                ldepth = comphash[chash].lib_type_depth[cdepth];

                for(j = 0; j < libhash[lhash].pin_count[ldepth]; i++)
                {
                    if(libhash[lhash].pin_type[ldepth][j] == OUTPUT)
                    {                      
                        strcat(new_gatepin, libhash[lhash].pin_names[ldepth][j]);
                        break;
                    }
                } 
                for (j = 0; j < libhash[lhash].pin_count[ldepth]; j++)
                {
                    if (libhash[lhash].pin_type[ldepth][j] == INPUT)
                    {
                        strcpy(curr_pin, comphash[chash].name[cdepth]);
                        strcat(curr_pin, libhash[lhash].pin_names[ldepth][j]); 

                        get_gatepin_indices(curr_pin, &curr_ghash, &curr_gdepth);
                        if(gatepinhashv[curr_ghash].isVisited[curr_gdepth] == 1)
                        {
                            size++;
                        }
                    }
                }
                if(size == libhash[lhash].pin_count[ldepth] - 1)
                {
                    // printf("new_gatepin is %s\n", new_gatepin);
                    get_gatepin_indices(new_gatepin, &ghash, &gdepth);
                    // if(gatepinhashv[ghash].isVisited[gdepth] == 1)
                    // {
                    //     continue;
                    // }
                    add_gatepin_pos(S, ghash, gdepth);
                    // gatepinhashVisited_make_visited(ghash, gdepth);
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

void assign_level_gatepins(Gatepin_pos *L)
{
    int i;
    int chash;
    int cdepth;
    int lhash;
    int ldepth;
    int ghash;
    int gdepth;
    int max_level;
    int j;
    char *curr_gatepin = NULL;
    int input_ghash;
    int input_gdepth;

    if(L == NULL)
    {
        printf("Error: L is NULL\n");
        return;
    }

    for(i = L->size - 1; i >= 0; i--)
    {
        max_level = 0;
        if(gatepinhash[L->ghash[i]].type[L->gdepth[i]] == IO_TYPE)
        {
            gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = 0;
        }
        else
        {
            get_predecessors_pin(gatepinhash[L->ghash[i]].name[L->gdepth[i]], &ghash, &gdepth);
            if( (gdepth == -1) || (gatepinhash[ghash].type[gdepth] == PO) )
            {
                //printf("Warning: %s is not connected to any other gate\n", gatepinhash[L->ghash[i]].name[L->gdepth[i]]);

                chash = gatepinhash[L->ghash[i]].parentComponent[L->gdepth[i]];
                cdepth = gatepinhash[L->ghash[i]].parentComponentDepth[L->gdepth[i]];

                lhash  = comphash[chash].lib_type[cdepth];
                ldepth = comphash[chash].lib_type_depth[cdepth];

                for(j = 0; j < libhash[lhash].pin_count[ldepth]; j++)
                {
                    curr_gatepin = (char *) realloc(curr_gatepin, sizeof(char) * (strlen(comphash[chash].name[cdepth]) + strlen(libhash[lhash].pin_names[ldepth][j]) + 1));
                    strcpy(curr_gatepin, comphash[chash].name[cdepth]);
                    strcat(curr_gatepin, libhash[lhash].pin_names[ldepth][j]);
                    get_gatepin_indices(curr_gatepin, &input_ghash, &input_gdepth);

                    max_level = max(max_level, gatepinhashv[input_ghash].level[input_gdepth]);
                }
                gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = max_level + 1;
                free(curr_gatepin);
                curr_gatepin = NULL;
            }
            else
            {
                gatepinhashv[L->ghash[i]].level[L->gdepth[i]] = gatepinhashv[ghash].level[gdepth] + 1;
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
    int output_ghash;
    int output_gdepth;
    int input_ghash;
    int input_gdepth;
    int size = 0;

    Gatepin_pos *L = NULL;

    for(ghash = 0; ghash < gatepinhash_size; ghash++)
    {
        for(gdepth = 0; gdepth < HASHDEPTH; gdepth++)
        {
            if(gatepinhash[ghash].hashpresent[gdepth] == 1)
            {
                if(gatepinhash[ghash].type[gdepth] == IO_TYPE)
                {
                    startpoint_ghash = (int *) realloc(startpoint_ghash, sizeof(int) * (i + 1));
                    startpoint_gdepth = (int *) realloc(startpoint_gdepth, sizeof(int) * (i + 1));
                
                    startpoint_ghash[i] = ghash;
                    startpoint_gdepth[i] = gdepth;  
                    i++;
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
    j = 0;


    startpoint_ghash = (int *) realloc(startpoint_ghash, sizeof(int) * (1000 + 1));
    startpoint_gdepth = (int *) realloc(startpoint_gdepth, sizeof(int) * (1000 + 1));
    

    curr_gatepin = (char *) malloc(1000 * sizeof(char)); // needs modify !!!!!!! //
    while(startpoint_ghash[0] != -1)
    {
        for(i = 0; i < L->size; i++)
        {
            if(gatepinhash[L->ghash[i]].type[L->gdepth[i]] == IO_TYPE)
            {
                continue;
            }
            chash = gatepinhash[L->ghash[i]].parentComponent[L->gdepth[i]];
            cdepth = gatepinhash[L->ghash[i]].parentComponentDepth[L->gdepth[i]];
            lhash = comphash[chash].lib_type[cdepth];
            ldepth = comphash[chash].lib_type_depth[cdepth];

            if(libhash[lhash].cell_type[ldepth] == SEQUENTIAL)
            {
                //startpoint_ghash = (int *) realloc(startpoint_ghash, sizeof(int) * (j + 1));
                //startpoint_gdepth = (int *) realloc(startpoint_gdepth, sizeof(int) * (j + 1));

                size = 0;
                // for(k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
                // {
                //     if (libhash[lhash].pin_type[ldepth][k] == INPUT)
                //     {
                //         strcpy(curr_gatepin, comphash[chash].name[cdepth]);
                //         strcat(curr_gatepin, libhash[lhash].pin_names[ldepth][j]); 

                //         get_gatepin_indices(curr_gatepin, &input_ghash, &input_gdepth);
                //         if(gatepinhashv[input_ghash].isVisited[input_gdepth] == 1)
                //         {
                //             size++;
                //         }
                //     }

                // }
                strcpy(curr_gatepin, comphash[chash].name[cdepth]);
                strcat(curr_gatepin, "/D"); 
                get_gatepin_indices(curr_gatepin, &input_ghash, &input_gdepth);
                if(gatepinhashv[input_ghash].isVisited[input_gdepth] == 1)
                {
                    curr_gatepin = NULL;
                    for(k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
                    {
                        if(libhash[lhash].pin_type[ldepth][k] == OUTPUT)
                        {
                            // curr_gatepin = (char *) realloc(curr_gatepin, sizeof(char) * (strlen(comphash[chash].name[cdepth]) + strlen(libhash[lhash].pin_names[ldepth][k]) + 1));
                            strcpy(curr_gatepin, comphash[chash].name[cdepth]);
                            strcat(curr_gatepin, libhash[lhash].pin_names[ldepth][k]);
                            get_gatepin_indices(curr_gatepin, &output_ghash, &output_gdepth);
                            break;
                        }
                    }
                    startpoint_ghash[j] = output_ghash;
                    startpoint_gdepth[j] = output_gdepth;
                    j++;
                }

                // if(size == libhash[lhash].pin_count[ldepth] - 1)
                // {
                //     curr_gatepin = NULL;
                //     for(k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
                //     {
                //         if(libhash[lhash].pin_type[ldepth][k] == OUTPUT)
                //         {
                //             // curr_gatepin = (char *) realloc(curr_gatepin, sizeof(char) * (strlen(comphash[chash].name[cdepth]) + strlen(libhash[lhash].pin_names[ldepth][k]) + 1));
                //             strcpy(curr_gatepin, comphash[chash].name[cdepth]);
                //             strcat(curr_gatepin, libhash[lhash].pin_names[ldepth][k]);
                //             get_gatepin_indices(curr_gatepin, &output_ghash, &output_gdepth);
                //             break;
                //         }
                //     }
                //     startpoint_ghash[j] = output_ghash;
                //     startpoint_gdepth[j] = output_gdepth;
                //     j++;
                // }
            }
        }
        startpoint_ghash[j] = -1;
        if(j != 0)
        {
            L = toposort(startpoint_ghash, startpoint_gdepth);
            assign_level_gatepins(L);
        }
        // free(startpoint_ghash);
        // free(startpoint_gdepth);
        // free(curr_gatepin);
        // startpoint_gdepth = NULL;
        // startpoint_ghash = NULL;
        // curr_gatepin = NULL;
        j = 0;
    }

    // for (i = 0; i < L->size; i++)
    // {
    //     printf("No: %d is %s and level is %d\n", i+1, gatepinhash[L->ghash[i]].name[L->gdepth[i]], gatepinhashv[L->ghash[i]].level[L->gdepth[i]]);
    // }

    // Sort the pins by level in ascending order //
    max_level = 0;

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] == 1)
            {
                max_level = max(max_level, gatepinhashv[i].level[j]);
            }
        }
    }

    for (int level = 0; level <= max_level; level++) 
    {
        for (i = 0; i < gatepinhash_size; i++) 
        {
            for(j = 0; j < HASHDEPTH; j++)
            {
                if(gatepinhash[i].hashpresent[j] == 1)
                {
                    if (gatepinhashv[i].level[j] == level) 
                    {
                        printf("No: %d is %s and level is %d\n", i+1, gatepinhash[i].name[j], gatepinhashv[i].level[j]);
                    }
                }
            }
        }
    }

    free_gatepin_pos(L);
    free(startpoint_ghash);
    free(startpoint_gdepth);
    free(curr_gatepin);
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

