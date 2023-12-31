//
#include "stack.h"

stack *create_stack(int capacity)
{
    stack *newStack = (stack*) malloc(sizeof(stack));

    newStack->capacity = capacity;
    newStack->top = -1;
    newStack->array = (char*) malloc(newStack->capacity * sizeof(char));

    return newStack;
}

int isFull(stack *currStack)
{
    return currStack->top == currStack->capacity - 1;
}

int isEmpty(stack *currStack)
{
    return currStack->top == -1;
}

void push(stack* currStack, char item)
{
    if(isFull(currStack))
    {
        return;
    }

    currStack->top++;
    currStack->array[currStack->top] = item;
    #ifdef DEBUG
    printf("Item %c pushed to stack\n", item);
    #endif
}

char pop(stack *currStack)
{
    if(isEmpty(currStack))
    {
        return '\0';
    }

    currStack->top-- ;
    return currStack->array[currStack->top + 1];
}

char peek(stack *currStack)
{
    if(isEmpty(currStack))
    {
        return '\0';
    }
    return currStack->array[currStack->top];
}

void delete_stack (stack *currStack)
{
    free(currStack->array);
    free(currStack);
}