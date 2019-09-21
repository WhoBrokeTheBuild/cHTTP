#ifndef CHTTP_H
#define CHTTP_H

typedef struct
{
    const char * id;

    void (*init)(void);
    void (*term)(void);
    
} chttp_module_t;

#endif // CHTTP_H
