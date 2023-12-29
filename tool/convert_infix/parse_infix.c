//
#include "parse_infix.h"

int getPriority(char operator)
{
    if(operator == '!')
    {
        return 4;
    }
    else if(operator == '*')
    {
        return 3;
    }
    else if(operator == '+')
    {
        return 2;
    }
    else if(operator == '^')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

char* parse_infix(char *infix)
{
    int i;
    stack *operatorStack;
    int infix_size = 0;
    char *postfix = NULL;
    int postfix_pos = 0;
    char token;

    infix_size = strlen(infix) + 1;
    postfix = (char*) malloc(sizeof(char) * infix_size);

    operatorStack = create_stack(infix_size);

    for(i = 0; i < infix_size; i++)
    {
        if(infix[i] != ' ' && infix[i] != '\0')
        {
            token = infix[i];
            if(token == '(')
            {
                push(operatorStack, token);
            }
            else if(token == ')')
            {
                while (!isEmpty(operatorStack) && operatorStack->array[operatorStack->top] != '(')
                {
                    postfix[postfix_pos] = pop(operatorStack);
                    postfix_pos++ ;
                }
                pop(operatorStack); // discard ( //
                
            }
            else if(getPriority(token) == 0)
            {
                postfix[postfix_pos] = token;
                postfix_pos++ ;
            }
            else
            {
                while(!isEmpty(operatorStack) && getPriority(operatorStack->array[operatorStack->top]) >= getPriority(token))
                {
                    postfix[postfix_pos] = pop(operatorStack);
                    postfix_pos++ ;
                }
                push(operatorStack, token);
            }
        }
    }

    while(!isEmpty(operatorStack))
    {
        postfix[postfix_pos] = pop(operatorStack);
        postfix_pos++ ;
    }

    
    postfix[postfix_pos] = '\0';

    return postfix;
}

// int main()
// {
//     char infix[] = "!((I0 * !(S)) + (I1 * S) + (I0 * I1)) ";

//     printf("The postfix is %s\n", parse_infix(infix));

//     return 0;
// }