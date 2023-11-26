// Structs to store design //
#include "structs.h"


void Gatepins_init()
{
    int i, j;
    gatepinhash = (gatePins*) my_calloc(HASH_SIZE, sizeof(gatePins)); // size of 10 positions //

    for(i = 0; i < HASH_SIZE; i++)
    {   
        gatepinhash[i].hashdepth = 1;
        gatepinhash[i].name = (char **) my_calloc(1, sizeof(char*));
        gatepinhash[i].type = (int *) my_calloc(1, sizeof(int));   // init for 1 pos //
        gatepinhash[i].connections_size = (int*) my_calloc(1, sizeof(int));
        gatepinhash[i].pinConn = (int**) my_calloc(1, sizeof(int*));
        gatepinhash[i].pinConnDepth = (int**) my_calloc(1, sizeof(int*));

        for(j = 0; j < gatepinhash[i].hashdepth; j++) // what is the value of hashdepth? 
        {
            gatepinhash[i].name[j] = NULL;

            // gatepinhash[i].parentComponent[j] = 0;
            // gatepinhash[i].parentComponentDepth[j] = 0;

            gatepinhash[i].type[j] = -1;   // init type //

            gatepinhash[i].pinConn[j] = (int*) my_calloc(1, sizeof(int));
            gatepinhash[i].pinConnDepth[j] = (int*) my_calloc(1, sizeof(int));

            gatepinhash[i].connections_size[j] = 1;
        }
    }
}

void Gatepins_add(char *pin_name)
{
    int i;
    unsigned int key;
    key = hash_function(pin_name, HASH_SIZE);

    if(pin_name == NULL)
        return;

    gatepinhash[key].name = (char **) my_realloc(gatepinhash[key].name, sizeof(char*) * (gatepinhash[key].hashdepth + 1));    // allocate memory for new pin //
    for(i = 0; i < gatepinhash[key].hashdepth; i++)
    {
        if(gatepinhash[key].name[i] == NULL)    // empty //
            break;
    }
    if(gatepinhash[key].name == NULL)
    {
        printf("ERROR on allocation\n");
    }
    gatepinhash[key].name[i] = (char *) my_calloc(1, (strlen(pin_name) + 1) );

    strcpy(gatepinhash[key].name[i], pin_name);
    // gatepinhash[key].type = (int *) my_realloc(gatepinhash[key].type, sizeof(int) * gatepinhash[key].hashdepth);
    // gatepinhash[key].type[i] = 1;

    gatepinhash[key].pinConn = (int**) my_realloc(gatepinhash[key].pinConn, sizeof(int*) * (gatepinhash[key].hashdepth + 1));
    gatepinhash[key].pinConnDepth = (int**) my_realloc(gatepinhash[key].pinConnDepth, sizeof(int*) * (gatepinhash[key].hashdepth + 1));
    
    gatepinhash[key].connections_size = (int*) my_realloc(gatepinhash[key].connections_size, sizeof(int) * gatepinhash[key].hashdepth + 1);
    gatepinhash[key].connections_size[i] = 1;

    gatepinhash[key].hashdepth++ ;  // add one pos in hashdepth //
    printf("Component inserted succesfully\n");
 }

void get_gatepin_indices(char *pin_name, int *ghash, int *ghashdepth)
{
    int i;
    unsigned int key;
    key = hash_function(pin_name, HASH_SIZE);
    *ghash = key;

    *ghashdepth = -1;

    for(i = 0; i < gatepinhash[*ghash].hashdepth - 1; i++)
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
    gatepinhash[source_hash].pinConn[source_hash_depth] = (int*) my_realloc(gatepinhash[source_hash].pinConn[source_hash_depth], sizeof(int) * (gatepinhash[source_hash].connections_size[source_hash_depth] +1 ));
    gatepinhash[source_hash].pinConnDepth[source_hash_depth] = (int*) my_realloc(gatepinhash[source_hash].pinConnDepth[source_hash_depth], sizeof(int) * (gatepinhash[source_hash].connections_size[source_hash_depth] + 1));
    
    size_of_connections = gatepinhash[source_hash].connections_size[source_hash_depth]; // get size of connections for current pin //

    gatepinhash[source_hash].pinConn[source_hash_depth][size_of_connections-1] = connection_hash;
    gatepinhash[source_hash].pinConnDepth[source_hash_depth][size_of_connections-1] = connection_hash_depth;

    gatepinhash[source_hash].connections_size[source_hash_depth]++;
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

