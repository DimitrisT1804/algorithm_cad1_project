#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../structs/structs.h"

#define ANSI_COLOR_RED     "\x1b[31m"   // define color codes to print TCL messages //
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_ORANGE  "\x1b[38;5;208m"
#define ANSI_COLOR_MAGENDA "\e[1;35m"
#define BOLD_LETTERS "\033[1m"


enum IO_STATES 
{
    START,
    IO_NAME
};

enum IO_STATES_CCS
{
    WAIT_IO,
    NAME_IO,
    CCS,
    CONNECTIONS_COMP,
    CONNECTIONS
};

enum lib_parse
{
    WAIT,
    COMP_NAME,
    CELL_TYPE,
    CELL_NAME,
    CELL_TIMING_TYPE,
    GET_CELL_TYPE,
    WAIT_CCS,
    OUTPUT_PIN_1,
    OUTPUT_PIN_2,
    CONNECTED_PINS,
    CONNECTED_PINS_2,
    COMPONENT_2,
    PIN_OUT,
    FUNCTION_WAIT,
    FUNCTION
};

enum proccess_lib_pins_IO
{
    BEGIN_IO,
    GET_COMP_NAME_IO,
    PIN_IO
};

enum proccess_lib_pins
{
    BEGIN,
    GET_COMP_NAME,
    PIN
};

enum IO_STATES_CCS countIOS(enum IO_STATES_CCS currentState, char *event);
enum lib_parse count_components_CCS(enum lib_parse currentState, char *event);
enum IO_STATES proccessIOS(enum IO_STATES currentState, char *event);
enum IO_STATES_CCS proccessIOS_CCS(enum IO_STATES_CCS currentState, char *event);
enum lib_parse proccessAllComponentsCCS(enum lib_parse currentState, char *event);
enum proccess_lib_pins_IO proccess_lib_pins_IO(enum proccess_lib_pins_IO currentState, char *event);
enum proccess_lib_pins proccess_lib_pins(enum proccess_lib_pins currentState, char *event);

void print_gatepinhash();
void print_libhash();
int call_parser(char *input_file);
void print_comphash();
void print_cells();

