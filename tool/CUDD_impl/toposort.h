// Toposort algorithm header file //

struct gatepin_pos
{
    int *ghash;
    int *gdepth;
    int size;
};
typedef struct gatepin_pos Gatepin_pos;


Gatepin_pos *sorted_gatepins;   // L //
Gatepin_pos *input_nodes;       // S //

Gatepin_pos *init_gatepin_pos(Gatepin_pos *gatepin);
void add_gatepin_pos(Gatepin_pos *gatepin, int ghash, int gdepth);
void remove_gatepin_pos(Gatepin_pos *gatepin, int ghash);
void free_gatepin_pos(Gatepin_pos *gatepin);

// End of snippet //