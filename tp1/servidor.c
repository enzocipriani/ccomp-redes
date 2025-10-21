#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define PORTA 5050
#define BUFFER_SIZE 4096
#define SITE_DIR "/mnt/c/Users/enzocipriani/Documents/GitHub/meusite"

// ANSI para cores
#define COR_VERDE   "\033[1;32m"
#define COR_VERMELHA "\033[1;31m"
#define COR_AMARELA "\033[1;33m"
#define COR_RESET   "\033[0m"

// timestamp
void obter_timestamp(char *dest, size_t tamanho) {
    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    strftime(dest, tamanho, "%d/%m/%Y %H:%M:%S", t);
}

// tipo do arquivo
const char *obter_tipo_mime(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (!ext) return "text/plain";
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    return "text/plain";
}

// formatar logs
void log_http(const char *status, const char *recurso, const char *cor) {
    char timestamp[64];
    obter_timestamp(timestamp, sizeof(timestamp));
    printf("%s[%s]%s  %s  (%s)\n", cor, status, COR_RESET, recurso, timestamp);
}

// enviar resposta para o txt
void enviar_resposta(int cliente_fd, const char *caminho_completo, const char *recurso) {
    FILE *arquivo = fopen(caminho_completo, "rb");
    if (!arquivo) {
        const char *resposta_erro =
            "HTTP/1.0 404 Not Found\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<html><body><h1>404 - Arquivo não encontrado</h1></body></html>";
        write(cliente_fd, resposta_erro, strlen(resposta_erro));

        log_http("404 Not Found", recurso, COR_VERMELHA);
        return;
    }

    const char *tipo = obter_tipo_mime(caminho_completo);
    char cabecalho[256];
    snprintf(cabecalho, sizeof(cabecalho),
             "HTTP/1.0 200 OK\r\nContent-Type: %s\r\n\r\n", tipo);
    write(cliente_fd, cabecalho, strlen(cabecalho));

    char buffer[BUFFER_SIZE];
    size_t bytes;
    while ((bytes = fread(buffer, 1, BUFFER_SIZE, arquivo)) > 0) {
        write(cliente_fd, buffer, bytes);
    }
    fclose(arquivo);

    log_http("200 OK", recurso, COR_VERDE);
}

// listar diretorio
void listar_diretorio(int cliente_fd, const char *recurso) {
    DIR *dir = opendir(SITE_DIR);
    if (!dir) {
        perror("Erro ao abrir diretorio");
        return;
    }

    const char *inicio =
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<html><body><h2>Arquivos disponiveis:</h2><ul>";
    write(cliente_fd, inicio, strlen(inicio));

    struct dirent *ent;
    char linha[1024];
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
        snprintf(linha, sizeof(linha),
                 "<li><a href=\"/%s\">%s</a></li>", ent->d_name, ent->d_name);
        write(cliente_fd, linha, strlen(linha));
    }

    const char *fim = "</ul></body></html>";
    write(cliente_fd, fim, strlen(fim));
    closedir(dir);

    log_http("200 OK", recurso, COR_VERDE);
}

int main() {
    int servidor_fd, cliente_fd;
    struct sockaddr_in endereco;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(endereco);

    servidor_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor_fd < 0) {
        perror("Erro ao criar socket");
        exit(1);
    }

    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(PORTA);

    if (bind(servidor_fd, (struct sockaddr*)&endereco, sizeof(endereco)) < 0) {
        perror("Erro em bind");
        close(servidor_fd);
        exit(1);
    }

    listen(servidor_fd, 5);
    printf(COR_AMARELA "Servidor HTTP rodando na porta %d...\n" COR_RESET, PORTA);

    while (1) {
        cliente_fd = accept(servidor_fd, (struct sockaddr*)&endereco, &addr_len);
        if (cliente_fd < 0) {
            perror("Erro em accept");
            continue;
        }

        memset(buffer, 0, BUFFER_SIZE);
        read(cliente_fd, buffer, BUFFER_SIZE - 1);

        char metodo[8], caminho[1024];
        sscanf(buffer, "%s %s", metodo, caminho);

        char timestamp[64];
        obter_timestamp(timestamp, sizeof(timestamp));
        printf(COR_AMARELA "\nRequisição recebida: %s %s (%s)\n" COR_RESET, metodo, caminho, timestamp);

        if (strcmp(metodo, "GET") == 0) {
            if (strcmp(caminho, "/") == 0) {
                char index_path[1024];
                snprintf(index_path, sizeof(index_path), "%s/index.html", SITE_DIR);
                struct stat st;
                if (stat(index_path, &st) == 0)
                    enviar_resposta(cliente_fd, index_path, caminho);
                else
                    listar_diretorio(cliente_fd, caminho);
            } else {
                char caminho_completo[2048];
                snprintf(caminho_completo, sizeof(caminho_completo),
                         "%s%s", SITE_DIR, caminho);
                enviar_resposta(cliente_fd, caminho_completo, caminho);
            }
        } else {
            const char *erro =
                "HTTP/1.0 405 Method Not Allowed\r\n"
                "Content-Type: text/plain\r\n\r\n"
                "Método não permitido.";
            write(cliente_fd, erro, strlen(erro));

            log_http("405 Method Not Allowed", caminho, COR_VERMELHA);
        }

        close(cliente_fd);
    }

    close(servidor_fd);
    return 0;
}