# Protocolo HTTP — Trabalho Prático de Redes de Computadores

**Aluno:** Enzo Cipriani  
**Disciplina:** Redes de Computadores  
**Instituição:** UFSJ  
**Professor:** Flávio Schiavoni

---

## Descrição do Projeto

Este trabalho implementa um **Cliente HTTP (navegador em modo texto)** e um **Servidor HTTP básico**, ambos desenvolvidos em **C**, utilizando **sockets TCP** e o **protocolo HTTP/1.0**.

O objetivo é compreender o funcionamento da comunicação **cliente-servidor** na Internet, aplicando na prática os conceitos do protocolo HTTP e da pilha TCP/IP.

---

## Estrutura do Projeto

- tp1/
  - main.c
  - navegador.c
  - navegador.h
  - interface.c
  - interface.h
  - servidor.c
  - Makefile
  - README.md
  - LICENSE

---

## Parte 1 — Cliente HTTP

O cliente HTTP simula um **navegador em modo texto**, capaz de se conectar a um servidor, realizar uma requisição `GET` e salvar o conteúdo retornado.

### Funcionalidades

- Conexão TCP com servidor remoto.
- Suporte a URLs com ou sem porta:  
  `http://host[:porta]/arquivo`
- Requisições **GET** conforme o protocolo **HTTP/1.0**.
- Interface visual com **ncurses**, utilizando cores para destacar mensagens:
  - Branco: texto padrão
  - Laranja: mensagens de progresso
  - Vermelho: mensagens de erro
  - Verde: mensagens de sucesso
- Salvamento automático da resposta em `saida_http.txt`.

### Exemplo de uso

```bash
make run-client
```

## Parte 2 — Servidor HTTP

O **servidor HTTP** responde a requisições **GET**, enviando arquivos de um diretório base `(/home/enzocipriani/meusite)`.
Caso o arquivo index.html não exista, o servidor gera automaticamente uma listagem dos arquivos disponíveis.

### Funcionalidades

- Servidor TCP sequencial (multiconexão iterativa).
- Resposta a arquivos estáticos: .html, .css, .js, imagens, etc.
- Listagem automática de diretórios quando index.html não é encontrado.
- Logs coloridos com timestamp:
  - Verde: 200 OK
  - Vermelho: 404 Not Found
  - Laranja: 405 Method Not Allowed
- Exibição de data e hora em cada requisição recebida.

### Exemplo de uso

```bash
make run-server
```

## Dependências

Para compilar e executar o projeto em sistemas baseados em Ubuntu/WSL, **instale** as bibliotecas necessárias:

```bash
sudo apt update
```

```bash
sudo apt install build-essential libncurses5-dev libncursesw5-dev
```

## Aprendizados

Durante o desenvolvimento deste projeto, foram aplicados e reforçados os seguintes conceitos:

- Comunicação via sockets TCP
- Estrutura e funcionamento do protocolo HTTP
- Implementação de cliente e servidor em C
- Uso da biblioteca ncurses para interfaces em terminal
- Parsing de URLs e cabeçalhos HTTP
- Manipulação de arquivos e diretórios no Linux

## Resultado Final

- Cliente HTTP funcional com interface em modo texto
- Servidor HTTP local com listagem automática de diretórios
- Logs coloridos com timestamp
- Makefile automatizado
- Código modular, comentado e de fácil execução

## Licença

Este projeto é distribuído sob a [MIT License](LICENSE).
