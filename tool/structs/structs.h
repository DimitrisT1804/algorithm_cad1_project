#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "../parser/custom_functions.h"

#define HASHDEPTH 10
#define IO_TYPE 0
#define WIRE 1
#define PO 2
#define COMBINATIONAL 1
#define SEQUENTIAL 2
#define INPUT 0
#define OUTPUT 1
// #define DEBUG

struct gatepins
{
    char *name[HASHDEPTH];
    
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
    char *name[HASHDEPTH]; // names of cells //
    int cell_type[HASHDEPTH]; // type of each cell (Combinational or Sequential) //
    char **pin_names[HASHDEPTH]; // names of pins for each cell //
    int pin_count[HASHDEPTH]; // number of pins for each cell //
    char **function[HASHDEPTH]; // boolean function for each cell //
    int out_pins_count[HASHDEPTH];
    int hashpresent[HASHDEPTH]; // flag indicating presence in the hash table //
    int *pin_type[HASHDEPTH];
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
char **libarray; // array to store cells temporarily to count them //
int libarray_size;

Components *comphash;
int comphash_size;

/* gatepinhash functions*/
void Gatepins_init();
void Gatepins_free();
void Gatepins_add(char *pin_name, int pin_type);
void get_gatepin_indices(char *pin_name, int *ghash, int *ghashdepth);
void gatepin_add_CCs(char *source_pin, char *connection_pin);
void gatepins_complete_parent();
void gatepin_characterize_IOs();

unsigned int hash_function(const char *str, unsigned int num_buckets);


/* libhash function */
void Lib_init();
void Lib_add(char *cell_name, int cell_type, char *func_expr);
void lib_add_function(char *cell_name, char *func_expr);
void get_libhash_indices(char *cell_name, int *lhash, int *lhashdepth);
void lib_add_pins (char *cell_name, char *pin_name, int pin_type);
void libhash_free();
void add_cell(char *cell_name);
void lib_add_func(char *cell_name, char *func_expr);

/* Compohash functions */
void comphash_init();
void comphash_add(char *comp_name, char *cell_name, int cell_type, char *func_expr);
void get_comphash_indices(char *comp_name, int *chash, int *chashdepth);
void comphash_free();
void structs_init();
void structs_free();