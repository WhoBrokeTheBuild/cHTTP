#include <chttp/chttp.h>
#include <stdlib.h>

chttp_module_t CHTTP_MODULE = {
    .id   = "header",
    .init = NULL,
    .term = NULL,
};