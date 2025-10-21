#include <ncurses.h>
#include "interface.h"

int main() {
    iniciar_interface();

    char url[512];
    echo();
    mvgetnstr(3, 18, url, sizeof(url) - 1);
    noecho();

    url_info info;
    if (!parse_url(url, &info)) {
        exibir_linha_em_branco();
        exibir_log("URL invalida.", COR_VERMELHA);
        encerrar_interface();
        return 1;
    }

    baixar_arquivo(&info);

    exibir_linha_em_branco();
    encerrar_interface();
    return 0;
}