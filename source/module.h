#ifndef CHTTP_MODULE_H
#define CHTTP_MODULE_H

#include <stdbool.h>

bool load_module(const char * module_filename);

void free_modules(void);

void modules_init(void);

void modules_term(void);

bool is_module_loaded(const char * id);

#endif // CHTTP_MODULE_H
