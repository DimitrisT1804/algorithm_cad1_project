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
    printf("Item %c pushed to stack\n", item);
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

// int main() 
// { 
//     struct Stack* stack = create_stack(100); 
  
//     push(stack, 'A'); 
//     push(stack, 'B'); 
//     push(stack, 'C'); 
  
//     printf("%c popped from stack\n", pop(stack)); 
  
//     return 0; 
// } 