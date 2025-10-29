CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

NCURSES_LIBS = -lncurses

SRCS_CLIENT = main.c navegador.c interface.c
OBJS_CLIENT = $(SRCS_CLIENT:.c=.o)
CLIENT_BIN = navegador

SRCS_SERVER = servidor.c
SERVER_BIN = servidor

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

# Executar servidor 
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

.PHONY: rebuild
rebuild: clean all
