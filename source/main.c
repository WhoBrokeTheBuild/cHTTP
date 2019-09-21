#include "config.h"
#include "module.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    parse_config_file("httpd.conf");

    modules_init();

    
    modules_term();

    return 0;
}
