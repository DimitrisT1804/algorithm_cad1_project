#include "custom_functions.h"

/* custom functions for realloc and malloc to don't check for errors every time we call it */

void* my_realloc(void* ptr, size_t size) 
{
    ptr = realloc(ptr, size);
    if(ptr == NULL)
    {
        printf("Error on allocation\n");

        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* my_calloc(int nmemb, size_t size) 
{
    void *ptr;
    ptr = calloc(nmemb, size);
    if(ptr == NULL)
    {
        printf("Error on allocation\n");

        exit(EXIT_FAILURE);
    }
    return ptr;
}