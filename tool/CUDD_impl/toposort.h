// Toposort algorithm header file //

struct gatepin_pos
{
    int ghash;
    int gdepth;
};
typedef struct gatepin_pos Gatepin_pos;


Gatepin_pos *sorted_gatepins;   // L //
Gatepin_pos *input_nodes;       // S //