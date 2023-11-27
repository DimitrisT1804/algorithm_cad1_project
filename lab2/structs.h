#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "custom_functions.h"

#define HASH_SIZE 70

struct gatepins
{
    char **name;

    int hashdepth;

    // connections //
    int **pinConn; // hash key //
    int **pinConnDepth;  // depth that pins located //
    int *connections_size;

    // parent component //
   int *parentComponent;
    int *parentComponentDepth;

    int *type;   /* what type this pin is --> 0: input
                                             1: output
                                             2: wire */
};
typedef struct gatepins gatePins;

gatePins *gatepinhash;

void Gatepins_init();

void Gatepins_add(char *pin_name);

void get_gatepin_indices(char *pin_name, int *ghash, int *ghashdepth);

void Gatepin_reload(char *source_pin, char *connection_pin);

unsigned int hash_function(const char *str, unsigned int num_buckets);
