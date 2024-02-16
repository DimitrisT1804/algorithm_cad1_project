//
#include <cudd.h>
// #include <util.h>
// #include "/home/dimitris-zorin/Downloads/cudd-release/util/util.h"
#include "../structs/structs.h"
#include "../CUDD_impl/cudd.h"

extern DdManager *gbm;
extern char **NamesDot;
extern int *ghash_added;
extern int *gdepth_added;
extern int ghash_added_size;

void annotate_bdds();