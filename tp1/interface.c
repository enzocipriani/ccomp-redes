#include <ncurses.h>
#include "interface.h"

static int linha_atual = 6; 

void iniciar_interface() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    clear();


    init_pair(COR_PADRAO, COLOR_WHITE, COLOR_BLACK);
    init_pair(COR_LARANJA, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COR_BARRA_FUNDO, COLOR_BLACK, COLOR_BLACK);
    init_pair(COR_VERMELHA, COLOR_RED, COLOR_BLACK);
    init_pair(COR_VERDE, COLOR_GREEN, COLOR_BLACK);

    
    attron(COLOR_PAIR(COR_LARANJA) | A_BOLD);
    mvprintw(1, 10, "==== CLIENTE HTTP ====");
    attroff(COLOR_PAIR(COR_LARANJA) | A_BOLD);

    
    attron(COLOR_PAIR(COR_PADRAO) | A_BOLD);
    mvprintw(3, 4, "Digite a URL: ");
    attroff(COLOR_PAIR(COR_PADRAO) | A_BOLD);

    linha_atual = 6;
    refresh();
}

void exibir_log(const char *mensagem, int cor) {
    attron(COLOR_PAIR(cor));
    mvprintw(linha_atual, 4, "%s", mensagem);
    attroff(COLOR_PAIR(cor));
      linha_atual += 1;
    refresh();
}

void exibir_linha_em_branco() {
    linha_atual += 2; 
}

void encerrar_interface() {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    attron(COLOR_PAIR(COR_LARANJA) | A_BOLD);
    mvprintw(yMax - 2, (xMax / 2) - 17, "Pressione qualquer tecla para sair...");
    attroff(COLOR_PAIR(COR_LARANJA) | A_BOLD);

    refresh();
    getch();
    endwin();
}
