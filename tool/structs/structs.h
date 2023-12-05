#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../parser/custom_functions.h"
// #include "globals.h"

#define HASH_SIZE 10000
#define GP_HASHDEPTH 100
#define IO_TYPE 0
#define WIRE 1

#define LIBHASH_SIZE 2000
#define LIB_HASHDEPTH 10
#define COMBINATIONAL 1
#define SEQUENTIAL 2

#define COMPHASH_SIZE 4000
#define COMP_HASHDEPTH 30
// #define DEBUG

struct gatepins
{
    char *name[GP_HASHDEPTH];

    //int hashdepth;

    // connections //
    int *pinConn[GP_HASHDEPTH]; // hash key //
    int *pinConnDepth[GP_HASHDEPTH];  // depth that pins located //
    int connections_size[GP_HASHDEPTH];

    // parent component //
    int parentComponent[GP_HASHDEPTH];
    int parentComponentDepth[GP_HASHDEPTH];

    int type[GP_HASHDEPTH];   /* what type this pin is --> 0: input / output
                                             1: wire */

    int hashpresent[GP_HASHDEPTH];
};
typedef struct gatepins gatePins;

struct library
{
    char *name[LIB_HASHDEPTH];

    int cell_type[LIB_HASHDEPTH];

    char **pin_names[LIB_HASHDEPTH];

    int pin_count[LIB_HASHDEPTH];

    char *function[LIB_HASHDEPTH];

    int hashpresent[LIB_HASHDEPTH];

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
int gatepinhash_size;

Lib *libhash; 

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

/* Compohash functions */

void comphash_init();

void comphash_add(char *comp_name, char *cell_name, int cell_type, char *func_expr);

void get_comphash_indices(char *comp_name, int *chash, int *chashdepth);

void comphash_free();


void structs_init();

void structs_free();