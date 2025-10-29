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

    // extrai host (com ou sem porta)
    char host_port[256];
    if (path_start) {
        strncpy(host_port, host_start, path_start - host_start);
        host_port[path_start - host_start] = '\0';
        strcpy(info->path, path_start);
    } else {
        strcpy(host_port, host_start);
        strcpy(info->path, "/");
    }

    // verifica se tem porta 
    char *colon = strchr(host_port, ':');
    if (colon) {
        *colon = '\0';
        strcpy(info->host, host_port);
        strcpy(info->port, colon + 1);
    } else {
        strcpy(info->host, host_port);
        strcpy(info->port, "80"); // padrão 
    }

    return 1;
}

int baixar_arquivo(const url_info *info) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    char msg[2048];
    snprintf(msg, sizeof(msg), "Conectando a %s:%s ...", info->host, info->port);
    exibir_linha_em_branco();
    exibir_log(msg, COR_PADRAO);

    int status = getaddrinfo(info->host, info->port, &hints, &res);
    if (status != 0) {
        snprintf(msg, sizeof(msg), "Erro em getaddrinfo: %s", gai_strerror(status));
        exibir_log(msg, COR_LARANJA);
        return 0;
    }

    // criação do socket
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) {
        exibir_log("Erro ao criar socket!", COR_LARANJA);
        freeaddrinfo(res);
        return 0;
    }

    // conectar com o servidor
    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        exibir_log("Erro ao conectar ao servidor!", COR_LARANJA);
        close(sock);
        freeaddrinfo(res);
        return 0;
    }

    snprintf(msg, sizeof(msg), "Conectado com sucesso a %s!", info->host);
    exibir_log(msg, COR_PADRAO);
    exibir_linha_em_branco();

    // enviar requisição HTTP
    char request[2048];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n",
             info->path, info->host);
    send(sock, request, strlen(request), 0);

    exibir_log("Requisição enviada:", COR_PADRAO);
    snprintf(msg, sizeof(msg), "GET %s HTTP/1.0", info->path);
    exibir_log(msg, COR_PADRAO);
    snprintf(msg, sizeof(msg), "Host: %s", info->host);
    exibir_log(msg, COR_PADRAO);
    exibir_log("Connection: close", COR_PADRAO);
    exibir_linha_em_branco();

    // resposta
    FILE *file = fopen("saida_http.txt", "wb");
    if (!file) {
        exibir_log("Erro ao criar arquivo de saida!", COR_LARANJA);
        close(sock);
        freeaddrinfo(res);
        return 0;
    }

    char buffer[4096];
    ssize_t bytes;
    int header_done = 0;
    char *header_end;

    while ((bytes = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        if (!header_done) {
            header_end = strstr(buffer, "\r\n\r\n");
            if (header_end) {
                int header_len = header_end - buffer + 4;
                fwrite(buffer + header_len, 1, bytes - header_len, file);
                header_done = 1;
            }
        } else {
            fwrite(buffer, 1, bytes, file);
        }
    }

    fclose(file);
    close(sock);
    freeaddrinfo(res);

    exibir_log("Download concluido com sucesso!", COR_LARANJA);
    exibir_log("Conteúdo salvo em saida_http.txt", COR_VERDE);

    return 1;
}