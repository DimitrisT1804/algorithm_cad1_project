#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "custom_functions.h"

#define HASH_SIZE 70
#define IO_TYPE 0
#define WIRE 1

#define LIBHASH_SIZE 10
#define COMBINATIONAL 1
#define SEQUENTIAL 2

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

    int *type;   /* what type this pin is --> 0: input / output
                                             1: wire */
};
typedef struct gatepins gatePins;

struct library
{
    char **name;

    int *cell_type;

    int hashdepth;

};
typedef struct library Lib;


gatePins *gatepinhash;

Lib *libhash; 

void Gatepins_init();

void Gatepins_free();

void Gatepins_add(char *pin_name, int pin_type);

void get_gatepin_indices(char *pin_name, int *ghash, int *ghashdepth);

void Gatepin_reload(char *source_pin, char *connection_pin);

unsigned int hash_function(const char *str, unsigned int num_buckets);


/* libhash function */
void Lib_init();

void Lib_add(char *cell_name, int cell_type);

void get_libhash_indices(char *cell_name, int *lhash, int *lhashdepth);
