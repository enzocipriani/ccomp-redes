# Makefile para o projeto TP1 (Cliente + Servidor HTTP em C)
# Uso: make         -> compila cliente (navegador) e servidor
#      make run-server       -> executa ./servidor
#      make run-client URL=... -> executa ./navegador <URL>
#      make clean      -> remove binários e .o
#
# Requisitos (Debian/Ubuntu/WSL):
# sudo apt install build-essential libncurses5-dev libncursesw5-dev

CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

NCURSES_LIBS = -lncurses

# Cliente (interface + lógica)
SRCS_CLIENT = main.c navegador.c interface.c
OBJS_CLIENT = $(SRCS_CLIENT:.c=.o)
CLIENT_BIN = navegador

# Servidor
SRCS_SERVER = servidor.c
SERVER_BIN = servidor

# Alvo padrão
.PHONY: all
all: $(CLIENT_BIN) $(SERVER_BIN)

# Link do cliente
$(CLIENT_BIN): $(OBJS_CLIENT)
	$(CC) $(CFLAGS) -o $@ $^ $(NCURSES_LIBS)

# Compila servidor
$(SERVER_BIN): $(SRCS_SERVER)
	$(CC) $(CFLAGS) -o $@ $<

# Regra genérica para .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Executar servidor (rodará em foreground)
.PHONY: run-server
run-server: $(SERVER_BIN)
	@echo "Iniciando servidor: ./$(SERVER_BIN)"
	./$(SERVER_BIN)

# Executar client
.PHONY: run-client
run-client: $(CLIENT_BIN)
	@echo "Executando cliente (interface interativa)..."
	./$(CLIENT_BIN)

# Limpeza
.PHONY: clean
clean:
	-rm -f $(OBJS_CLIENT) $(CLIENT_BIN) $(SERVER_BIN) *.o *.txt

# Alvos úteis
.PHONY: rebuild
rebuild: clean all
