// Structs to store design //
#include "structs.h"


// gatePins *gatepinhash = NULL;

void Gatepins_init()
{
    int i, j;
    gatepinhash = (gatePins*) my_calloc(HASH_SIZE, sizeof(gatePins)); // size of 10 positions //

    for(i = 0; i < HASH_SIZE; i++)
    {   
        //gatepinhash[i].hashdepth = 1;
        // gatepinhash[i].name = (char **) my_calloc(1, sizeof(char*));
        // gatepinhash[i].type = (int *) my_calloc(1, sizeof(int));   // init for 1 pos //
        // gatepinhash[i].connections_size = (int*) my_calloc(1, sizeof(int));
        // gatepinhash[i].pinConn = (int**) my_calloc(1, sizeof(int*));
        // gatepinhash[i].pinConnDepth = (int**) my_calloc(1, sizeof(int*));

        // gatepinhash[i].name = NULL;
        // gatepinhash[i].type = NULL;   // init for 1 pos //
        // gatepinhash[i].connections_size = NULL;
        // gatepinhash[i].pinConn = NULL;
        // gatepinhash[i].pinConnDepth = NULL;

        for(j = 0; j < GP_HASHDEPTH; j++) // what is the value of hashdepth
        {
            gatepinhash[i].name[j] = NULL;

            // gatepinhash[i].parentComponent[j] = 0;
            // gatepinhash[i].parentComponentDepth[j] = 0;

            gatepinhash[i].type[j] = -1;   // init type //

            gatepinhash[i].pinConn[j] = NULL;    // no need i do this in add //
            gatepinhash[i].pinConnDepth[j] = NULL;

            gatepinhash[i].connections_size[j] = 1;

            gatepinhash[i].parentComponent[j] = -1;
            gatepinhash[i].parentComponentDepth[j] = -1;

            gatepinhash[i].hashpresent[j] = 0;
        }
    }
}

void Gatepins_free()
{
    int i, j;

    for(i = 0; i < HASH_SIZE; i++)
    {   
        for(j = 0; j < GP_HASHDEPTH; j++) // what is the value of hashdepth? 
        {
            if(gatepinhash[i].name[j] != NULL)
            {
                free(gatepinhash[i].name[j]);

                // gatepinhash[i].parentComponent[j] = 0;
                // gatepinhash[i].parentComponentDepth[j] = 0;
                free (gatepinhash[i].pinConn[j]);
                free (gatepinhash[i].pinConnDepth[j]);
            }
        }
    }
    free(gatepinhash);
}

void Gatepins_add(char *pin_name, int pin_type)
{
    int i;
    unsigned int key;

    if(pin_name == NULL)
        return;

    key = hash_function(pin_name, HASH_SIZE);


    // gatepinhash[key].name = (char **) my_realloc(gatepinhash[key].name, sizeof(char*) * (gatepinhash[key].hashdepth + 1));    // allocate memory for new pin //
    for(i = 0; i < GP_HASHDEPTH; i++)
    {
        if(gatepinhash[key].hashpresent[i] == 0)    // empty //
            break;
    }
    // i = gatepinhash[key].hashdepth - 1;
    gatepinhash[key].name[i] = (char *) my_calloc(1, (strlen(pin_name) + 1) );

    strcpy(gatepinhash[key].name[i], pin_name);
    // gatepinhash[key].type = (int *) my_realloc(gatepinhash[key].type, sizeof(int) * gatepinhash[key].hashdepth);
    // gatepinhash[key].type[i] = 1;

    /* Realloc for size of pointer array pinConn and pinConnDepth */
    // gatepinhash[key].pinConn = (int**) my_realloc(gatepinhash[key].pinConn, sizeof(int*) * (gatepinhash[key].hashdepth + 1));
    // gatepinhash[key].pinConnDepth = (int**) my_realloc(gatepinhash[key].pinConnDepth, sizeof(int*) * (gatepinhash[key].hashdepth + 1));
    
    /* Calloc for the new pin size for each connections, 1 for begin and go on*/
    gatepinhash[key].pinConn[i] = (int *) my_calloc(1, sizeof(int));
    gatepinhash[key].pinConnDepth[i] = (int *) my_calloc(1, sizeof(int));


    // gatepinhash[key].connections_size = (int*) my_realloc(gatepinhash[key].connections_size, sizeof(int) * gatepinhash[key].hashdepth + 1);
    gatepinhash[key].connections_size[i] = 0;

    // gatepinhash[key].type = (int*) my_realloc(gatepinhash[key].type, sizeof(int) * gatepinhash[key].hashdepth + 1);
    gatepinhash[key].type[i] = pin_type;

    // gatepinhash[key].hashdepth++ ;  // add one pos in hashdepth //
    gatepinhash[key].hashpresent[i] = 1;

    #ifdef DEBUG
    printf("Gatepin inserted succesfully\n");
    #endif
 }

void get_gatepin_indices(char *pin_name, int *ghash, int *ghashdepth)
{
    int i;
    unsigned int key;
    key = hash_function(pin_name, HASH_SIZE);
    *ghash = key;

    *ghashdepth = -1;

    for(i = 0; i < GP_HASHDEPTH; i++)
    {
        if(gatepinhash[*ghash].name[i] != NULL)
        {
            if(strcmp(gatepinhash[*ghash].name[i], pin_name) == 0)
            {
                *ghashdepth = i;
                break;
            }
        }
    }
}

/* Reload pins with their connections */
void Gatepin_reload(char *source_pin, char *connection_pin)
{
    int source_hash, source_hash_depth;
    int connection_hash, connection_hash_depth;
    int size_of_connections = 0;

    if(source_pin == NULL || connection_pin == NULL)
        return;

    get_gatepin_indices(source_pin, &source_hash, &source_hash_depth);
    get_gatepin_indices(connection_pin, &connection_hash, &connection_hash_depth);

    // free(source_pin);
    // free(connection_pin);

    if(source_hash_depth == -1 || connection_hash_depth == -1)
    {
        printf("The pins does not exist!\n");
        return;
    }

    if(gatepinhash[source_hash].pinConnDepth[source_hash_depth] == NULL)
    {
        printf("Invalid pointer\n");
    }

    //gatepinhash[source_hash].pinConn = my_realloc(gatepinhash[source_hash].pinConn, sizeof(int*) * gatepinhash[source_hash].hashdepth);
    gatepinhash[source_hash].pinConn[source_hash_depth] = (int*) my_realloc(gatepinhash[source_hash].pinConn[source_hash_depth], sizeof(int) * (gatepinhash[source_hash].connections_size[source_hash_depth] + 1));
    gatepinhash[source_hash].pinConnDepth[source_hash_depth] = (int*) my_realloc(gatepinhash[source_hash].pinConnDepth[source_hash_depth], sizeof(int) * (gatepinhash[source_hash].connections_size[source_hash_depth] + 1));
    
    size_of_connections = gatepinhash[source_hash].connections_size[source_hash_depth]; // get size of connections for current pin //

    gatepinhash[source_hash].pinConn[source_hash_depth][size_of_connections] = connection_hash;
    gatepinhash[source_hash].pinConnDepth[source_hash_depth][size_of_connections] = connection_hash_depth;

    gatepinhash[source_hash].connections_size[source_hash_depth]++;
}


void gatepins_complete_parent()
{
    int i, j, k;
    char *comp_name = NULL;
    int chash, cdepth;

    for (i = 0; i < HASH_SIZE; i++)
    {
        for (j = 0; j < GP_HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0  && gatepinhash[i].type[j] == WIRE)
            {
                for (k = strlen(gatepinhash[i].name[j]); gatepinhash[i].name[j][k] != '/'; k--);
                //if(k > 0)
                //{
                    comp_name = (char *) calloc(k+1, sizeof(char));
                    strncpy(comp_name, gatepinhash[i].name[j], k);
                    get_comphash_indices(comp_name, &chash, &cdepth);
                    
                    free(comp_name);
                    if(cdepth != -1)
                    {
                        gatepinhash[i].parentComponent[j] = chash;
                        gatepinhash[i].parentComponentDepth[j] = cdepth;
                    }
                //}
            }
            // free(comp_name);
        }
    }
}


unsigned int hash_function(const char *str, unsigned int num_buckets) 
{
    unsigned long hash_value = 5381; // Initial value

    while (*str != '\0') 
    {
        hash_value = (hash_value * 33) ^ (unsigned long)(*str);
        str++;
    }

    // Use modulo to restrict the hash value to the range [0, num_buckets-1]
    return (unsigned int)(hash_value % num_buckets);
}

void Lib_init()
{
    int i, j;
    libhash = (Lib*) my_calloc(LIBHASH_SIZE, sizeof(Lib));

    for(i = 0; i < LIBHASH_SIZE; i++)
    {
        // libhash[i].pin_names = (char**) my_calloc(1, sizeof(char*));

        //libhash[i].cell_type = (int *) my_calloc(1, sizeof(int));


        for(j = 0; j < LIB_HASHDEPTH; j++)
        {
            libhash[i].name[j] = NULL;
            libhash[i].function[j] = NULL;
            libhash[i].cell_type[j] = -1;  // init //
            libhash[i].pin_names[j] = NULL;
            libhash[i].pin_count[j] = 0;
            libhash[i].hashpresent[j] = 0;
            
            // libhash[i].pin_names[j] = (char **) calloc(1, sizeof(char *));
            libhash[i].pin_names[j] = NULL;
        }
    }
}

void Lib_add(char *cell_name, int cell_type, char *func_expr)
{
    int i;
    unsigned int key;

    if(cell_name == NULL)
        return;

    key = hash_function(cell_name, LIBHASH_SIZE);

    //libhash[key].name = (char **) my_realloc(libhash[key].name, sizeof(char*) * (libhash[key].hashdepth + 1) );

    for (i = 0; i < LIB_HASHDEPTH; i++)
    {
        if(libhash[key].hashpresent[i] == 0)
            break;
    }

    // i = LIB_HASHDEPTH;

    libhash[key].name[i] = (char *) my_calloc((strlen(cell_name) + 1), sizeof(char));
    strcpy(libhash[key].name[i], cell_name);

    libhash[key].function[i] = (char *) my_calloc(strlen(func_expr) + 1, sizeof(char));
    strcpy(libhash[key].function[i], func_expr);

    //libhash[key].cell_type = (int *) my_realloc(libhash[key].cell_type, sizeof(int) * (libhash[key].hashdepth + 1) );
    libhash[key].cell_type[i] = cell_type;

    libhash[key].hashpresent[i] = 1;

    //libhash[key].hashdepth++ ;  // add depth in hash table //
    #ifdef DEBUG
    printf("Cell inserted succesfully on libhash\n");
    #endif
}

// void lib_add_function(char *cell_name, char *func_expr)
// {
//     int lhash, lhashdepth;
//     unsigned int key;
//     int i;

//     get_libhash_indices(cell_name, &lhash, &lhashdepth);
//     if(lhashdepth == -1)
//     {
//         printf("pali error");
//         return; // ERROR cell does not exists! //
//     }
//     if(libhash[key].function[i] != NULL)
//     {
//         return;
//     }

//     key = hash_function(cell_name, LIBHASH_SIZE);

//     for (i = 0; i < LIB_HASHDEPTH; i++)
//     {
//         if(libhash[key].name[i] == NULL)
//             break;
//     }
//     libhash[key].function[i] = (char *) my_calloc(strlen(func_expr) + 1, sizeof(char));
//     strcpy(libhash[key].function[i], func_expr);
// }

void get_libhash_indices(char *cell_name, int *lhash, int *lhashdepth)
{
    int i;
    unsigned int key;
    key = hash_function(cell_name, LIBHASH_SIZE);
    *lhash = key;

    *lhashdepth = -1;

    for(i = 0; i < LIB_HASHDEPTH; i++)
    {
        if(libhash[*lhash].hashpresent[i] != 0)
        {
            if(strcmp(libhash[*lhash].name[i], cell_name) == 0)
            {
                *lhashdepth = i;
                break;
            }
        }
    }    
}

void lib_add_pins (char *cell_name, char *pin_name)
{
    int i;
    int lhash, ldepth;

    if(cell_name == NULL)
    {
        return;
    }

    get_libhash_indices(cell_name, &lhash, &ldepth);
    if(ldepth == -1)
    {
        printf("ERROR: There is not this cell\n");
        return;
    }
    free(cell_name);

    for(i = 0; i < libhash[lhash].pin_count[ldepth]; i++)
    {
        if(libhash[lhash].pin_names[ldepth][i] != NULL)
        {
            if (strcmp(libhash[lhash].pin_names[ldepth][i], pin_name) == 0 )
            {
                return; // already exists //
            }
        }
    }
    // add the pin name on pos i //

    libhash[lhash].pin_count[ldepth]++ ;

    libhash[lhash].pin_names[ldepth] = (char **) realloc(libhash[lhash].pin_names[ldepth], (libhash[lhash].pin_count[ldepth]) * sizeof(char *));
    libhash[lhash].pin_names[ldepth][libhash[lhash].pin_count[ldepth] - 1] = (char *) calloc(strlen(pin_name) + 1, sizeof(char));

    strcpy(libhash[lhash].pin_names[ldepth][libhash[lhash].pin_count[ldepth] - 1], pin_name);
}

void libhash_free()
{
    int i, j, k;

    for(i = 0; i < LIBHASH_SIZE; i++)
    {   
        for(j = 0; j < LIB_HASHDEPTH; j++) // what is the value of hashdepth? 
        {
            if(libhash[i].hashpresent[j] != 0)
            {
                free(libhash[i].name[j]);
                free(libhash[i].function[j]);
                for(k = 0; k < libhash[i].pin_count[j]; k++)
                {
                    free(libhash[i].pin_names[j][k]);
                }
                free(libhash[i].pin_names[j]);
            }
            // free(libhash[i].pin_names);
        }

        // free (libhash[i].name);
        // free (libhash[i].cell_type);
        // free (libhash[i].pin_count);
        // free(libhash[i].pin_names);
    }
    free(libhash);
}

void comphash_init()
{
    int i, j;

    comphash = (Components*) my_calloc(COMPHASH_SIZE, sizeof(Components));
    for (i = 0; i < COMPHASH_SIZE; i++)
    {
        for (j = 0; j < COMP_HASHDEPTH; j++)
        {
            comphash[i].name[j] = NULL;
            comphash[i].lib_type[j] = -1;
            comphash[i].lib_type_depth[j] = -1;  
            comphash[i].hashpresent[j] = 0;  
        }
    }
}

void comphash_add(char *comp_name, char *cell_name, int cell_type, char *func_expr)
{
    int i;
    unsigned int key;
    int lhash, ldepth;

    if (comp_name == NULL)
        return;
    key = hash_function(comp_name, COMPHASH_SIZE);

    for (i = 0; i < COMP_HASHDEPTH; i++)
    {
        if(comphash[key].hashpresent[i] == 0) // it is empty //
            break;
    }
    
    if(i == COMP_HASHDEPTH)
    {
        printf("No space!\n");
        return; // no space available //
    }

    comphash[key].name[i] = (char *) calloc(strlen(comp_name) + 1, sizeof(char));
    strcpy(comphash[key].name[i], comp_name);

    comphash[key].hashpresent[i] = 1;
    #ifdef DEBUG
    printf("Comp inserted succesfully\n");
    #endif

    get_libhash_indices(cell_name, &lhash, &ldepth);
    if(ldepth != -1)
    {
        comphash[key].lib_type[i] = lhash;
        comphash[key].lib_type_depth[i] = ldepth;
        
        return;
    }

    Lib_add(cell_name, cell_type, func_expr);  // if it does not exist, add it and get lhash and ldepth //
    get_libhash_indices(cell_name, &lhash, &ldepth);

    comphash[key].lib_type[i] = lhash;
    comphash[key].lib_type_depth[i] = ldepth;
}

void get_comphash_indices(char *comp_name, int *chash, int *chashdepth)
{
    int i;
    unsigned int key;

    key = hash_function(comp_name, COMPHASH_SIZE);
    *chash = key;

    *chashdepth = -1;

    for (i = 0; i < COMP_HASHDEPTH; i++)
    {
        if(comphash[key].name[i] != NULL)
        {
            if(strcmp(comphash[*chash].name[i], comp_name) == 0)
            {
                *chashdepth = i;
                break;
            }
        }
    }
}

void comphash_free()
{
    int i, j;

    for(i = 0; i < COMPHASH_SIZE; i++)
    {   
        for(j = 0; j < COMP_HASHDEPTH; j++) // what is the value of hashdepth? 
        {
            if(comphash[i].hashpresent[j] != 0)
            {
                free(comphash[i].name[j]);
            }
        }
    }
    free(comphash);
}

void structs_init()
{
    Gatepins_init();
    Lib_init();
    comphash_init();
}

void structs_free()
{
    Gatepins_free();
    libhash_free();
    comphash_free();
}