// Toposort algorithm header file //
#include "../structs/structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ANSI_COLOR_ORANGE  "\x1b[38;5;208m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"


struct gatepin_pos
{
    int *ghash;
    int *gdepth;
    int size;
};
typedef struct gatepin_pos Gatepin_pos;


extern Gatepin_pos *sorted_gatepins;   // L //
extern Gatepin_pos *input_nodes;       // S //

Gatepin_pos *toposort(int *startpoins_ghash, int *startpoints_gdepth);
void add_gatepin_pos(Gatepin_pos *gatepin, int ghash, int gdepth);
void remove_gatepin_pos(Gatepin_pos *gatepin, int ghash, int gdepth);
int max(int a, int b);
void free_gatepin_pos(Gatepin_pos *gatepin);
void add_startpoints();
void assign_level_gatepins(Gatepin_pos *L);


