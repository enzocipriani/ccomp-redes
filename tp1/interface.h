#ifndef INTERFACE_H
#define INTERFACE_H

#include "navegador.h"

void iniciar_interface();
void exibir_log(const char *mensagem, int cor);
void encerrar_interface();
void exibir_linha_em_branco();  

#define COR_PADRAO 1
#define COR_LARANJA 2
#define COR_BARRA_FUNDO 3
#define COR_VERMELHA 4
#define COR_VERDE 5


#endif