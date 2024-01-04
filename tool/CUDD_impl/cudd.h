// #include "../external_libraries/cudd-source/util/util.h"
// #include "../external_libraries/cudd-source/cudd/cudd.h"
#include <cudd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../convert_infix/parse_infix.h"

void print_dd (DdNode *dd, int n, int pr, char *name);
void write_dd (DdManager *gbm, DdNode *dd, char* filename);
void generate_bdd(char *infix, char *cell_name);