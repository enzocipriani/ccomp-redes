#include "navegador.h"
#include "interface.h"
#include "string.h"


int parse_url(const char *url, url_info *info) {
    if (strncmp(url, "http://", 7) != 0) {
        exibir_log("Erro: apenas URLs HTTP são suportadas.", COR_LARANJA);
        return 0;
    }

    const char *host_start = url + 7;
    const char *path_start = strchr(host_start, '/');

    char host_port[256];
    if (path_start) {
        strncpy(host_port, host_start, path_start - host_start);
        host_port[path_start - host_start] = '\0';
        strcpy(info->path, path_start);
    } else {
        strcpy(host_port, host_start);
        strcpy(info->path, "/");
    }

    // verifica se existe porta
    char *colon = strchr(host_port, ':');
    if (colon) {
        *colon = '\0';
        strcpy(info->host, host_port);
        strcpy(info->port, colon + 1);
    } else {
        strcpy(info->host, host_port);
        strcpy(info->port, "80"); 
    }

    return 1;
}

int baixar_arquivo(const url_info *info) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(info->host, info->port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "Erro em getaddrinfo: %s\n", gai_strerror(status));
        return 0;
    }

    // socket
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) {
        perror("Erro ao criar socket");
        freeaddrinfo(res);
        return 0;
    }

    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        perror("Erro ao conectar");
        close(sock);
        freeaddrinfo(res);
        return 0;
    }

    // requisição http
    char request[2048];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n",
             info->path, info->host);

    send(sock, request, strlen(request), 0);


    char buffer[4096];
    ssize_t bytes;

    char *header_end = NULL;
    int header_done = 0;

    char headers[8192] = {0};
    size_t headers_len = 0;

    FILE *arquivo = NULL;

    while ((bytes = recv(sock, buffer, sizeof(buffer), 0)) > 0) {

        // le cabeçalho
        if (!header_done) {
            memcpy(headers + headers_len, buffer, bytes);
            headers_len += bytes;

            header_end = strstr(headers, "\r\n\r\n");
            if (header_end) {
                header_done = 1;
                size_t header_size = header_end - headers + 4;

                //verifica se é index.html

                int tem_index = strstr(info->path, "/") && strlen(info->path) <= 2;
                int html_ok = strstr(headers, "text/html") != NULL;

                if (tem_index && html_ok) {
                    // salvcar index
                    arquivo = fopen("index.html", "wb");
                } else {
                    // salva os  arquivos
                    arquivo = fopen("saida_http.txt", "wb");
                }

                if (!arquivo) {
                    perror("Erro ao salvar arquivo");
                    close(sock);
                    freeaddrinfo(res);
                    return 0;
                }

                // depois do cabeçalho
                fwrite(headers + header_size, 1, headers_len - header_size, arquivo);
            }
        } 
        else {
            fwrite(buffer, 1, bytes, arquivo);
        }
    }

    if (arquivo) fclose(arquivo);

    close(sock);
    freeaddrinfo(res);

    return 1;
}