//
#include "stack_bdd.h"

stack_bdd *create_stack_bdd(int capacity)
{
    stack_bdd *newStack = (stack_bdd*) malloc(sizeof(stack_bdd));

    newStack->capacity = capacity;
    newStack->top = -1;
    newStack->array = (DdNode **) malloc(newStack->capacity * sizeof(DdNode*));

    return newStack;
}

int isFull_bdd(stack_bdd *currStack)
{
    return currStack->top == currStack->capacity - 1;
}

int isEmpty_bdd(stack_bdd *currStack)
{
    return currStack->top == -1;
}

void push_bdd(stack_bdd* currStack, DdNode *item)
{
    if(isFull_bdd(currStack))
    {
        return;
    }

    currStack->top++;
    currStack->array[currStack->top] = item;
    #ifdef DEBUG
    printf("Item %c pushed to stack\n", item);
    #endif
}

DdNode *pop_bdd(stack_bdd *currStack)
{
    if(isEmpty_bdd(currStack))
    {
        return NULL;
    }

    currStack->top-- ;
    return currStack->array[currStack->top + 1];
}

DdNode *peek_bdd(stack_bdd *currStack)
{
    if(isEmpty_bdd(currStack))
    {
        return NULL;
    }
    return currStack->array[currStack->top];
}

void delete_stack_bdd (stack_bdd *currStack)
{
    free(currStack->array);
    free(currStack);
}
