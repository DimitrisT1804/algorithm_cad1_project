#include "custom_functions.h"

void* my_realloc(void* ptr, size_t size) 
{
    ptr = realloc(ptr, size);
    if(ptr == NULL)
    {
        printf("Error on allocation\n");
    }
    return ptr; // Replace with the actual implementation
}

void* my_calloc(int nmemb, size_t size) 
{
    void *ptr;
    ptr = calloc(nmemb, size);
    if(ptr == NULL)
    {
        printf("Error on allocation\n");
    }
    return ptr; // Replace with the actual implementation
}