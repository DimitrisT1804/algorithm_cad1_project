
#ifndef GLOBALS_H
#define GLOBALS_H

struct gatepins
{
    char **name;

    int hashdepth;

    // connections //
    int **pinConn; // hash key //
    int **pinConnDepth;  // depth that pins located //
    int *connections_size;

    // parent component //
    int *parentComponent;
    int *parentComponentDepth;

    int *type;   /* what type this pin is --> 0: input / output
                                             1: wire */
};
typedef struct gatepins gatePins;

extern gatePins *gatepinhash;

#endif