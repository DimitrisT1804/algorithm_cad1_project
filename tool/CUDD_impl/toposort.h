// Toposort algorithm header file //
#include "../structs/structs.h"

struct gatepin_pos
{
    int *ghash;
    int *gdepth;
    int size;
};
typedef struct gatepin_pos Gatepin_pos;


Gatepin_pos *sorted_gatepins;   // L //
Gatepin_pos *input_nodes;       // S //

Gatepin_pos *toposort(int *startpoins_ghash, int *startpoints_gdepth);
void add_gatepin_pos(Gatepin_pos *gatepin, int ghash, int gdepth);
void remove_gatepin_pos(Gatepin_pos *gatepin, int ghash, int gdepth);
void free_gatepin_pos(Gatepin_pos *gatepin);
void add_startpoints();

int max(int a, int b);