//
#include <stdlib.h>
#include <stdio.h>
#include <cudd.h>
// #define DEBUG

struct Stack_bdd
{
    int top;
    int capacity;
    DdNode **array;
};
typedef struct Stack_bdd stack_bdd;

stack_bdd *create_stack_bdd(int capacity);

int isFull_bdd(stack_bdd *currStack);

int isEmpty_bdd(stack_bdd *currStack);

void push_bdd(stack_bdd* currStack, DdNode* item);

DdNode* pop_bdd(stack_bdd *currStack);

DdNode* peek_bdd(stack_bdd *currStack);

void delete_stack_bdd(stack_bdd *currStack);



