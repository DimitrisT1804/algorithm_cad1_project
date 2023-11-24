// Structs to store design //
#include <stdlib.h>
#include <stdio.h>


gatePins *gatepinhash;

struct gatepins
{
    char *name;

    // connections //
    unsigned long int **pinConn; // hash key //
    int **pinConnDepth;  // depth that pins located //

    // parent component //
    unsigned long int *parentComponent;
    int *parentComponentDepth;

    int type;   /* what type this pin is --> 0: input
                                             1: output
                                             2: wire */
};
typedef struct gatepins gatePins;


void Gatepins_init()
{
    gatepinhash = (gatePins*) calloc(10, sizeof(gatePins)); // size of 10 positions //

    gatepinhash->name = NULL;

    gatepinhash->name = pinn;
    
}
