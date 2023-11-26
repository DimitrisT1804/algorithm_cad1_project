#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "structs.h"

// Enumerate the possible states
enum DoorState 
{
    START,
    IO,
    NAME
};

enum DoorState2 
{
    START2,
    NAME2,
    CCS,
    CONNECTIONS_COMP,
    CONNECTIONS
};

enum DoorState processEvent(enum DoorState currentState, char *event);

enum DoorState2 processEvent2(enum DoorState2 currentState, char *event);

void print_gatepinhash();

