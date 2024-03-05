// 
// #include <cuddInt.h>
#include <cudd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../bdd_annotation/bdd_annotation.h"
// #include "/home/dimitris-zorin/Downloads/cudd-release/cudd/cuddInt.h"
#include "/home/dimitris/Downloads/cudd-release/cudd/cuddInt.h"
#include <omp.h>

#define ANSI_COLOR_RED     "\x1b[31m"   // define color codes to print TCL messages //
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_ORANGE  "\x1b[38;5;208m"
#define ANSI_COLOR_MAGENDA "\e[1;35m"
#define BOLD_LETTERS "\033[1m"

extern DdNode ***all_paths;
extern int path_size;
extern int all_paths_size;
extern DdNode **path;
extern double number_of_paths;
extern int *nodes_array;
extern double probability_gatepin;


double my_ddCountPathsToNonZero(DdNode * N, st_table * table);
void insert_node(DdNode *node, DdNode ***path);
void remove_node(DdNode ***path, DdNode *node);
void traverse_cudd(DdNode *node);
void write_minterms(int ghash, int gdepth);
double calculate_probabilities(int *vars_value);
void read_minterms(char *gatepin_name);
void print_paths();
void printInorder(DdNode* node);
