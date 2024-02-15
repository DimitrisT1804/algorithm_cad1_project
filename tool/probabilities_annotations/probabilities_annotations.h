// 
// #include <cuddInt.h>
#include <cudd.h>
#include <stdio.h>
#include <stdlib.h>

extern DdNode ***all_paths;
extern int path_size;
extern int all_paths_size;

void insert_node(DdNode *node, DdNode ***path, int path_size);
void traverse_cudd(DdNode *node, DdNode **path);

