#ifndef NAVEGADOR_H
#define NAVEGADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef struct {
    char host[256];
    char path[1024];
    char port[10];
} url_info;


int parse_url(const char *url, url_info *info);
int baixar_arquivo(const url_info *info);


#endif