#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "custom_functions.h"
// #include "globals.h"

#define HASH_SIZE 20
#define IO_TYPE 0
#define WIRE 1

#define LIBHASH_SIZE 10
#define LIB_HASHDEPTH 5
#define COMBINATIONAL 1
#define SEQUENTIAL 2

#define COMPHASH_SIZE 20
#define COMP_HASHDEPTH 10

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
    char *name[LIB_HASHDEPTH];

    int cell_type[LIB_HASHDEPTH];

    char **pin_names[LIB_HASHDEPTH];

    int pin_count[LIB_HASHDEPTH];

};
typedef struct library Lib;

struct components
{
    char *name[COMP_HASHDEPTH];

    int lib_type[COMP_HASHDEPTH];

    int lib_type_depth[COMP_HASHDEPTH];

    int hashpresent[COMP_HASHDEPTH];    /* 0: does not exists
                                           1: exits */
};
typedef struct components Components;


gatePins *gatepinhash;

Lib *libhash; 

Components *comphash;

/* gatepinhash functions*/
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

/* Compohash functions */

void comphash_init();

void comphash_add(char *comp_name, char *cell_name, int cell_type);

void get_comphash_indices(char *comp_name, int *chash, int *chashdepth);

void structs_init();