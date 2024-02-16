// 
// #include <cuddInt.h>
#include <cudd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../bdd_annotation/bdd_annotation.h"

extern DdNode ***all_paths;
extern int path_size;
extern int all_paths_size;
extern DdNode **path;

void insert_node(DdNode *node, DdNode ***path);
void remove_node(DdNode ***path, DdNode *node);
void traverse_cudd(DdNode *node);
void read_minterms();
