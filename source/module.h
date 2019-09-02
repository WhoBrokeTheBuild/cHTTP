#ifndef CHTTP_MODULE_H
#define CHTTP_MODULE_H

#include <stdbool.h>

bool load_module(const char * module_filename);

void free_modules();

void init_modules();

void term_modules();

#endif // CHTTP_MODULE_H