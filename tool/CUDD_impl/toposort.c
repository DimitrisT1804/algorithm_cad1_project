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


void remove_gatepin_pos(Gatepin_pos *gatepin, int ghash)
{
    int i, j;
    for (i = 0; i < gatepin->size; i++)
    {
        if (gatepin->ghash[i] == ghash)
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
        add_gatepin_pos(L, S->ghash[0], S->gdepth[0]);
        remove_gatepin_pos(S, S->ghash[0]);
        
        ghash = L->ghash[0];
        gdepth = L->gdepth[0];

        if(gatepinhash[ghash].type[gdepth] == WIRE)
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
                    // strcat(new_gatepin, "/");
                    strcat(new_gatepin, libhash[lhash].pin_names[ldepth][j]);
                    break;
                }
            }

            printf("new_gatepin is %s\n", new_gatepin);
            get_gatepin_indices(new_gatepin, &curr_ghash, &curr_gdepth);
            add_gatepin_pos(L, curr_ghash, curr_gdepth);
        }
        else
        {
            curr_ghash = ghash;
            curr_gdepth = gdepth;
        }
        
        for(i = 0; i < gatepinhash[curr_ghash].connections_size[curr_gdepth]; i++)
        {
            // remove connection of this pin with current one //
            // make this pin as visited //
            ghash = gatepinhash[curr_ghash].pinConn[curr_gdepth][i];
            gdepth = gatepinhash[curr_ghash].pinConnDepth[curr_gdepth][i];
        
            // make this one visited //
            if(gatepinhashv[ghash].isVisited[gdepth] == 1)
            {
                continue;
            }
            gatepinhashVisited_make_visited(ghash, gdepth);

            // if m has no other incoming edges then insert m into S //
            // it is always true because each pin can have only one predecessor //
            // if(gatepinhash[curr_ghash].type[curr_gdepth] == WIRE)
            // {
            //     chash = gatepinhash[curr_ghash].parentComponent[curr_gdepth];
            //     cdepth = gatepinhash[curr_ghash].parentComponentDepth[curr_gdepth];
                
            //     strcpy(new_gatepin, comphash[chash].name[cdepth]);

            //     lhash = comphash[chash].lib_type[cdepth];
            //     ldepth = comphash[chash].lib_type_depth[cdepth];

            //     for(j = 0; j < libhash[lhash].pin_count[ldepth]; i++)
            //     {
            //         if(libhash[lhash].pin_type[ldepth][j] == OUTPUT)
            //         {
            //             // strcat(new_gatepin, "/");
            //             strcat(new_gatepin, libhash[lhash].pin_names[ldepth][j]);
            //             break;
            //         }
            //     }

                //printf("new_gatepin is %s\n", new_gatepin);
                //get_gatepin_indices(new_gatepin, &curr_ghash, &curr_gdepth);
            int size = 0;
            if(gatepinhash[ghash].type[gdepth] == WIRE)
            {
                for(j = 0; j < gatepinhash[ghash].connections_size[gdepth]; j++)
                {
                    curr_ghash = gatepinhash[ghash].pinConn[gdepth][j];
                    curr_gdepth = gatepinhash[ghash].pinConnDepth[gdepth][j];

                    if(gatepinhashv[curr_ghash].isVisited[curr_gdepth] == 1)
                    {
                        size++;
                    }
                }
            }
            if(size == gatepinhash[ghash].connections_size[gdepth])
            {
                add_gatepin_pos(S, ghash, gdepth);
            }
            // }
        }
    }
    if(L != NULL)
    {
        return L;
    }
    return NULL;
}

void add_startpoints()
{
    int *startpoint_ghash;
    int *startpoint_gdepth;
    int i;

    Gatepin_pos *L = NULL;

    startpoint_ghash = (int *) malloc(9 * sizeof(int));
    startpoint_gdepth = (int *) malloc(9 * sizeof(int));

    get_gatepin_indices("N1", &startpoint_ghash[0], &startpoint_gdepth[0]);
    get_gatepin_indices("N3", &startpoint_ghash[1], &startpoint_gdepth[1]);
    get_gatepin_indices("N2", &startpoint_ghash[2], &startpoint_gdepth[2]);
    get_gatepin_indices("N6", &startpoint_ghash[3], &startpoint_gdepth[3]);
    get_gatepin_indices("N7", &startpoint_ghash[4], &startpoint_gdepth[4]);
    get_gatepin_indices("N22", &startpoint_ghash[5], &startpoint_gdepth[5]);
    get_gatepin_indices("N23", &startpoint_ghash[6], &startpoint_gdepth[6]);


    startpoint_ghash[7] = -1;

    L = toposort(startpoint_ghash, startpoint_gdepth);

    for (i = 0; i < L->size; i++)
    {
        printf("No: %d is %s\n", i+1, gatepinhash[L->ghash[i]].name[L->gdepth[i]]);
    }
}
