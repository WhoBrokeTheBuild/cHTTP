#ifndef CHTTP_CONFIG_H
#define CHTTP_CONFIG_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum ip_version
{
    IP_VERSION_4            = 4,
    IP_VERSION_6            = 6,
    
} ip_version_t;

typedef struct listen_config
{
    uint16_t                port;
    uint8_t                 address[16];
    ip_version_t            ip_version;
    
    struct listen_config *  next;
    
} listen_config_t;

typedef struct site_config
{
    char *                  server;
    char *                  document_root;
    
    listen_config_t *       first_listen_config;

    char *                  access_log_file;
    char *                  error_log_file;

    struct site_config *    next;
    
} site_config_t;

typedef struct
{
    site_config_t           default_site;
    site_config_t *         first_site_config;
    
} config_t;

listen_config_t * add_listen_config(site_config_t * site);

site_config_t * add_site_config();

bool parse_config_file(const char * filename);

void free_all_config();

#endif // CHTTP_CONFIG_H
