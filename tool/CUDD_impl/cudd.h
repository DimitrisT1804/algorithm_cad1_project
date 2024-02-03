#ifndef CUDD_H
#define CUDD_H

#include <cudd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../convert_infix/parse_infix.h"
#include "stack_bdd.h"

enum operators 
{
    AND,
    OR,
    XOR,
    NOT
};

void print_dd (DdManager *gbm, DdNode *dd, int n, int pr, char *name);
void write_dd (DdManager *gbm, DdNode *dd, char* filename);
void generate_bdd(char *infix, char *cell_name);
void generate_bdd_two(char *infix, char *cell_name);
DdNode *concat_bdds(char *infix, char *cell_name, DdNode **vars, DdNode *bdd, DdNode *temp_bdd_1, DdNode *temp_bdd_2, DdManager *gbm);


#endif