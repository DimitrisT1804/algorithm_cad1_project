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

enum lib_parse
{
    WAIT,
    CELL_NAME,
    CELL_TIMING_TYPE,
    GET_CELL_TYPE
};

enum DoorState processEvent(enum DoorState currentState, char *event);

enum DoorState2 processEvent2(enum DoorState2 currentState, char *event);

enum lib_parse proccesLib(enum lib_parse currentState, char *event);

void print_gatepinhash();

void print_libhash();

void call_parser(char *input_file);

