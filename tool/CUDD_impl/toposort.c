// Toposort algorithm source file //
#include "toposort.h"

Gatepin_pos* init_gatepin_pos(Gatepin_pos *gatepin)
{
    gatepin = (Gatepin_pos *) malloc(sizeof(Gatepin_pos));
    gatepin->ghash = NULL;
    gatepin->gdepth = NULL;

    return gatepin;
}

void add_gatepin_pos(Gatepin_pos *gatepin, int ghash, int gdepth)
{
    gatepin->ghash = (int *) realloc(gatepin->ghash, sizeof(int) * (gatepin->size + 1));
    gatepin->gdepth = (int *) realloc(gatepin->gdepth, sizeof(int) * (gatepin->size + 1));
    gatepin->ghash[gatepin->size] = ghash;
    gatepin->gdepth[gatepin->size] = gdepth;

    gatepin->size++;
}

void remove_gatepin_pos(Gatepin_pos *gatepin, int ghash)
{
    int i, j;
    for (i = 0; i < gatepin->size; i++)
    {
        if (gatepin->ghash[i] == ghash)
        {
            for (j = i; j < gatepin->size - 1; j++)
            {
                gatepin->ghash[j] = gatepin->ghash[j + 1];
                gatepin->gdepth[j] = gatepin->gdepth[j + 1];
            }
            gatepin->size--;
            break;
        }
    }
}

void free_gatepin_pos(Gatepin_pos *gatepin)
{
    free(gatepin->ghash);
    free(gatepin->gdepth);
    free(gatepin);
}

void toposort()
{
    Gatepin_pos *L = init_gatepin_pos(L);
    Gatepin_pos *S = init_gatepin_pos(S);

    while(S->size != 0)
    {
        add_gatepin_pos(L, S->ghash[0], S->gdepth[0]);
        remove_gatepin_pos(S, S->ghash[0]);

        
    }
}
