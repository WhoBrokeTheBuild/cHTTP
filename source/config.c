#include "config.h"
#include "defines.h"
#include "module.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

config_t config;

listen_config_t * add_listen_config(site_config_t * site)
{
    if (site->first_listen_config) {
        listen_config_t * last = site->first_listen_config;

        while (last->next) {
            last = last->next;
        }

        last->next = malloc(sizeof(listen_config_t));
        return last->next;
    }

    site->first_listen_config = malloc(sizeof(listen_config_t));
    return site->first_listen_config;
}

site_config_t * add_site_config()
{
    if (config.first_site_config) {
        site_config_t * last = config.first_site_config;

        while (last->next) {
            last = last->next;
        }

        last->next = malloc(sizeof(site_config_t));
        return last->next;
    }
    
    config.first_site_config = malloc(sizeof(site_config_t));
    return config.first_site_config;
}

bool parse_config_file(const char * filename)
{
    site_config_t * site = &config.default_site;

    printf("Loading %s\n", filename);

    FILE * file = fopen(filename, "rt");
    if (!file) {
        fprintf(stderr, "Failed to open config file %s\n", filename);
        exit(1);
    }

    char * pch = NULL;
    char line[MAX_CONFIG_LINE_LENGTH + 1];
    while (fgets(line, MAX_CONFIG_LINE_LENGTH, file)) {
        line[MAX_CONFIG_LINE_LENGTH] = '\0';

        pch = strchr(line, '\n');
        if (pch) {
            *pch = '\0';
        }

        pch = line;
        if (*pch == '#') {
            continue;
        }

        while (isspace(*pch)) {
            ++pch;
        }

        char * command = strtok(pch, " \t");
        char * argument = strtok(NULL, " \t\n");

        if (!command) {
            continue;
        }

        if (strcmp(command, "LoadModule") == 0) {
            if (!load_module(argument)) {
                fprintf(stderr, "Failed to load module %s\n", argument);
                exit(1);
            }
        }
        else if (strcmp(command, "DocumentRoot") == 0) {
            site->document_root = strdup(argument);
        }
        else if (strcmp(command, "Listen") == 0) {
            listen_config_t * listen = add_listen_config(site);
            if (*argument == '[') {
                listen->ip_version = IP_VERSION_6;
                // IPV6??
            }
            else {
                int a1, a2, a3, a4, port;

                sscanf(argument, "%d.%d.%d.%d:%d", &a1, &a2, &a3, &a4, &port);
                
                listen->address[0] = a1;
                listen->address[1] = a2;
                listen->address[2] = a3;
                listen->address[3] = a4;
                listen->port = port;

                listen->ip_version = IP_VERSION_4;
            }
        }
    }

    fclose(file);

    return true;
}

void free_site_config(site_config_t * site) 
{
    free(site->server);
    free(site->document_root);

    listen_config_t * listen = NULL;
    while (site->first_listen_config) {
        listen = site->first_listen_config;
        site->first_listen_config = listen->next;
        free(listen);
    }

    free(site->access_log_file);
    free(site->error_log_file);
}

void free_config()
{
    free_site_config(&config.default_site);
    
    site_config_t * site = NULL;
    while (config.first_site_config) {
        site = config.first_site_config;
        config.first_site_config = site->next;
        free_site_config(site);
        free(site);
    }
}
