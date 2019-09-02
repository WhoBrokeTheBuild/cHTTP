#include <chttp/chttp.h>
#include <stdio.h>

void hello_init() 
{
    printf("Hello, Module!\n");
}

void hello_term() 
{
    printf("Goodbye, Module!\n");
}

chttp_module_t CHTTP_MODULE = {
    .init = hello_init,
    .term = hello_term,
};
