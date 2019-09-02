#include "config.h"
#include "module.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    parse_config_file("httpd.conf");

    init_modules();

    term_modules();

    return 0;
}