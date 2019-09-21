#include "module.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chttp/chttp.h>

typedef struct module_record
{
    void *                  handle;
    chttp_module_t *        data;

    struct module_record *  next;
} module_record_t;

static module_record_t * first_module_record = NULL;

module_record_t * add_module_record(void);

module_record_t * add_module_record()
{
    if (first_module_record) {
        module_record_t * last = first_module_record;

        while (last->next) {
            last = last->next;
        }

        last->next = malloc(sizeof(module_record_t));
        return last->next;
    }

    first_module_record = malloc(sizeof(module_record_t));
    return first_module_record;
}

bool load_module(const char * module_filename)
{
    void * module_handle = dlopen(module_filename, RTLD_NOW);
    if (!module_handle) {
        fprintf(stderr, "Failed to load %s\n", module_filename);
        exit(1);
    }

    chttp_module_t * module_data = (chttp_module_t *)dlsym(module_handle, "CHTTP_MODULE");
    if (!module_data) {
        fprintf(stderr, "Failed to load CHTTP_MODULE symbol\n");
        exit(1);
    }

    module_record_t * module_record = add_module_record();
    module_record->handle = module_handle;
    module_record->data = module_data;

    return true;
}

void free_modules()
{
    module_record_t * record = NULL;
    while (first_module_record) {
        record = first_module_record;
        first_module_record = record->next;
        dlclose(record->handle);
        free(record);
    }
}

void modules_init()
{
    module_record_t * record = first_module_record;
    while (record) {
        if (record->data->init) {
            record->data->init();
        }
        record = record->next;
    }
}

void modules_term()
{
    module_record_t * record = first_module_record;
    while (record) {
        if (record->data->term) {
            record->data->term();
        }
        record = record->next;
    }
}

bool is_module_loaded(const char * id)
{
    module_record_t * record = first_module_record;
    while (record) {
        if (strcmp(record->data->id, id) == 0) {
            return true;
        }
        record = record->next;
    }
    return false;
}
