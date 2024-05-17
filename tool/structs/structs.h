#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <cudd.h>
#include <pthread.h>
#include "../parser/custom_functions.h"

#define HASHDEPTH 10
#define IO_TYPE 0
#define WIRE 1
#define PO 2
#define COMBINATIONAL 1
#define SEQUENTIAL 2
#define INPUT 0
#define OUTPUT 1

#define ANSI_COLOR_RED     "\x1b[31m"   // define color codes to print TCL messages //
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_ORANGE  "\x1b[38;5;208m"
#define ANSI_COLOR_MAGENDA "\e[1;35m"
#define BOLD_LETTERS "\033[1m"
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
    float location_x[HASHDEPTH];
    float location_y[HASHDEPTH];
    char side[HASHDEPTH];   // E:east / W:west / N:north / S:south //

    int value[HASHDEPTH];

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

    float width[HASHDEPTH];
    float height[HASHDEPTH];
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

struct gatepinhashVisited
{
    int isVisited[HASHDEPTH];
    int level[HASHDEPTH];
    DdNode *gatepin_bdd[HASHDEPTH];
};
typedef struct gatepinhashVisited GatepinhashVisited;

struct gatepinhashProb
{
    double one_prob[HASHDEPTH];
    double zero_prob[HASHDEPTH];
    int valid[HASHDEPTH];
};
typedef struct gatepinhashProb GatepinhashProb;

// struct to store components location --> placed info //
struct componentslocation
{
    float x[HASHDEPTH];
    float y[HASHDEPTH];

    float drawing_x[HASHDEPTH];
    float drawing_y[HASHDEPTH];
    float drawing_x_max[HASHDEPTH];
    float drawing_y_max[HASHDEPTH];
    
    int value[HASHDEPTH];
};
typedef struct componentslocation Componentslocation;

struct coresite
{
    int core_utilisation;
    float core_width;
    float core_height;
    float aspect_ratio;
};
typedef struct coresite Coresite;

struct rowsstruct
{
    char *name;
    float location_x;
    float location_y;
    float width;
    float height;
};
typedef struct rowsstruct Rows;


extern gatePins *gatepinhash;
extern int gatepinhash_size;
extern Lib *libhash; 
extern int libhash_size;
extern char **libarray; // array to store cells temporarily to count them //
extern int libarray_size;
extern Components *comphash;
extern int comphash_size;
extern GatepinhashVisited *gatepinhashv;
extern int isLevelized;
extern int max_design_level;
extern GatepinhashProb *gatepinhash_prob;

// placed informations //
extern Componentslocation *compslocation;
extern Coresite *coresite;
extern Rows *rows;
extern int rows_size;

extern int design_is_placed;
extern char *highlighted_component;

/* gatepinhash functions*/
void Gatepins_init();
void Gatepins_free();
void Gatepins_add(char *pin_name, int pin_type);
void get_gatepin_indices(char *pin_name, int *ghash, int *ghashdepth);
void gatepin_add_CCs(char *source_pin, char *connection_pin);
void gatepins_complete_parent();
void gatepin_characterize_IOs();
void get_predecessors_pin(char *gatepin, int *ghash, int *gdepth);
void add_ios_location(int, int, float, float, char);
void dump_gatepinhash();

unsigned int hash_function(const char *str, unsigned int num_buckets);

/* libhash function */
void Lib_init();
void Lib_add(char *cell_name, int cell_type, char *func_expr);
void get_libhash_indices(char *cell_name, int *lhash, int *lhashdepth);
void lib_add_pins (char *cell_name, char *pin_name, int pin_type);
void libhash_free();
void add_cell(char *cell_name);
void lib_add_func(char *cell_name, char *func_expr, char *cell_pin);
void libhash_add_dimensions(char *cell_name, float width, float height);

/* Compohash functions */
void comphash_init();
void comphash_add(char *comp_name, char *cell_name, int cell_type, char *func_expr);
void get_comphash_indices(char *comp_name, int *chash, int *chashdepth);
void comphash_free();
void structs_init();
void structs_free();
void dump_component();

/* GatepinhashVisited functions */
void gatepinhashVisited_init();
void gatepinhashVisited_make_visited(int ghash, int gdepth);
void gatepinhashVisited_remove_visited(int ghash, int gdepth);
void gatepinhashVisited_add_bdd(int ghash, int gdepth, DdNode *bdd, DdManager *gbm);
void gatepinhashVisited_free();

/* GatepinhashProb functions */
void gatepinhashProb_init();
void gatepinhashProb_free();

int check_gatepin_type(int ghash, int gdepth);

/* Componentslocation functions */
void componentslocation_init();
void componentslocation_free();
void add_components_location(char *comp_name, float x, float y);

/* Coresite functions */
void coresite_init();
void coresite_free();
void add_coresite(int, float, float, float);
void dump_coresite();

/* Rows functions */
void rows_init();
void rows_free();
void add_row(char *name, float x, float y, float width, float height);
void dump_rows();

void get_gatepin_from_value(int value, int *ghash, int *gdepth);

extern pthread_mutex_t mutex;

#endif