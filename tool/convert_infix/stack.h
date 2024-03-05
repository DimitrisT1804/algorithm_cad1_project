//
#include <stdlib.h>
#include <stdio.h>
// #define DEBUG

struct Stack
{
    int top;
    int capacity;
    char *array;
};
typedef struct Stack stack;

stack *create_stack(int capacity);
int isFull(stack *currStack);
int isEmpty(stack *currStack);
void push(stack* currStack, char item);
char pop(stack *currStack);
char peek(stack *currStack);
void delete_stack (stack *currStack);