#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../parser/custom_functions.h"
// #include "globals.h"

// #define HASH_SIZE 10000
#define GP_HASHDEPTH 0
#define IO_TYPE 0
#define WIRE 1

// #define LIBHASH_SIZE 2000
#define LIB_HASHDEPTH 0
#define COMBINATIONAL 1
#define SEQUENTIAL 2

// #define COMPHASH_SIZE 4000
#define COMP_HASHDEPTH 0
// #define DEBUG
#define HASHDEPTH 10

struct gatepins
{
    char *name[HASHDEPTH];

    //int hashdepth;

    // connections //
    int *pinConn[HASHDEPTH]; // hash key //
    int *pinConnDepth[HASHDEPTH];  // depth that pins located //
    int connections_size[HASHDEPTH];

    // parent component //
    int parentComponent[HASHDEPTH];
    int parentComponentDepth[HASHDEPTH];

    int type[HASHDEPTH];   /* what type this pin is --> 0: input / output
                                             1: wire */

    int hashpresent[HASHDEPTH];
};
typedef struct gatepins gatePins;

struct library
{
    char *name[HASHDEPTH];

    int cell_type[HASHDEPTH];

    char **pin_names[HASHDEPTH];

    int pin_count[HASHDEPTH];

    char *function[HASHDEPTH];

    int hashpresent[HASHDEPTH];

};
typedef struct library Lib;

struct components
{
    char *name[HASHDEPTH];

    int lib_type[HASHDEPTH];

    int lib_type_depth[HASHDEPTH];

    int hashpresent[HASHDEPTH];    /* 0: does not exists
                                           1: exits */
};
typedef struct components Components;


gatePins *gatepinhash;
int gatepinhash_size;

Lib *libhash; 
int libhash_size;
char **libarray;
int libarray_size;

Components *comphash;
int comphash_size;

/* gatepinhash functions*/
void Gatepins_init();

void Gatepins_free();

void Gatepins_add(char *pin_name, int pin_type);

void get_gatepin_indices(char *pin_name, int *ghash, int *ghashdepth);

void Gatepin_reload(char *source_pin, char *connection_pin);

void gatepins_complete_parent();

unsigned int hash_function(const char *str, unsigned int num_buckets);


/* libhash function */
void Lib_init();

void Lib_add(char *cell_name, int cell_type, char *func_expr);

void lib_add_function(char *cell_name, char *func_expr);

void get_libhash_indices(char *cell_name, int *lhash, int *lhashdepth);

void lib_add_pins (char *cell_name, char *pin_name);

void libhash_free();

void add_cell(char *cell_name);

/* Compohash functions */

void comphash_init();

void comphash_add(char *comp_name, char *cell_name, int cell_type, char *func_expr);

void get_comphash_indices(char *comp_name, int *chash, int *chashdepth);

void comphash_free();


void structs_init();

void structs_free();