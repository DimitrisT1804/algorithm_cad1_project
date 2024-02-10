#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../flex_bison/parser.h"
#include "../structs/structs.h"

int getPriority(char operator);

char* parse_infix(char *infix);
