
/** TODO
 *  + adicionar execução das tarefas
 *  + melhorar tratamento de mensagem recebida
 *  + tratamento de finalização
 *  + desenvolvimento das tarefas periodicas
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#define BUFFER_LENGTH 1024

// Variáveis globais
int clientfd;
int id = 0;

char *position;
float velocidade, angulo;

// tarefas periódicas
void *controle_velocidade(void *arg) {
    // MUTEX
    printf("Velocidade: %f\n", velocidade);
    // MUTEX
}

void *controle_posicao(void *arg) {
    // MUTEX
    printf("Posição: %s\n", position);
    // MUTEX
}

void *controle_equilibrio(void *arg) {
    // MUTEX
    printf("Ângulo de equilíbrio: %f\n", angulo);
    // MUTEX
}

void *client(void *arg) {
    int client_id = (int)arg;
    int i, n, len_msg;
    char buffer[BUFFER_LENGTH];

    strcpy(buffer, "Olá. Estou vivo e posso me mover! Diga os comandos.\n\0");

    if (send(clientfd, buffer, strlen(buffer), 0)) {
        fprintf(stdout, "Cliente conectado.\nAguardando resposta ...\n");

        // comunicação enquanto nao enviar comando exit
        do {
            memset(buffer, 0x0, sizeof(buffer));

            len_msg = read(clientfd, buffer, 80);
            if (len_msg > 0) {
                buffer[len_msg-1] = '\0'; // remover \n lido
                printf("Cliente: %s", buffer);
            }

            // analisa leitura
            if (strcmp(buffer, "right") == 0) {
                write(clientfd, "direita\n", 80);
                // MUTEX
                //execução dos comandos
                // MUTEX
            } else if (strcmp(buffer, "left") == 0) {
                write(clientfd, "esquerda\n", 80);
                // MUTEX
                //execução dos comandos
                // MUTEX
            } else if (strcmp(buffer, "forward") == 0) {
                write(clientfd, "para frente\n", 80);
                // MUTEX
                //execução dos comandos
                // MUTEX
            } else if (strcmp(buffer, "back") == 0) {
                write(clientfd, "voltou\n", 80);
                // MUTEX
                //execução dos comandos
                // MUTEX
            } else if (strcmp(buffer, "stop") == 0) {
                write(clientfd, "parou\n", 80);
                // MUTEX
                //execução dos comandos
                // MUTEX
            } else if (strcmp(buffer, "exit") == 0) {
                write(clientfd, "Encerrando sessão.\n", 80);
                // MUTEX
                //execução dos comandos
                // MUTEX
                write(clientfd, "exit", 4);
            }
        } while(strcmp(buffer, "exit"));
    }
}

int main(int argc, char const *argv[]) {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    int serverfd, portno;
    char buffer[BUFFER_LENGTH];
    pthread_t t;

    // verificação e validação dos paramentros de entrada
    if (argc < 2) {
        perror("ERRO. Porta não definida");
        return EXIT_FAILURE;
    }

    portno = atoi(argv[1]);
    if (portno < 1024) {
        perror("ERRO. Porta fora do intervalo definido: >= 1024");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Iniciando servidor.\n");

    // criação do socket
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0) {
        perror("ERRO. Falha ao abrir o socket");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Socket do servidor criada com fd: %d\n", serverfd);

    // bzero((char *) &server_addr, sizeof(server_addr));
    memset((char *) &server_addr, 0x0, sizeof(server_addr));

    // Propriedades do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portno);
    // memset(server_addr.sin_zero, 0x0, 8);

    // erro de porta em uso
    int yes = 1;
    if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                   sizeof(int)) < 0) {
        perror("ERRO. Falha nas opções do socket");
        return EXIT_FAILURE;
    }

    // bind socket
    if (bind(serverfd, (struct sockaddr *) &server_addr,
                        sizeof(server_addr)) < 0) {
        perror("ERRO. Falha ao fazer bind");
        return EXIT_FAILURE;
    }

    // listen - esperando o cliente
    if (listen(serverfd, 1) < 0) {
        perror("ERRO. Falha ao fazer listen");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Listen da porta: %d\n", portno);

    while (1) {
        client_len = sizeof(client_addr);
        // bloqueante
        clientfd = accept(serverfd, (struct sockaddr *) &client_addr, &client_len);
        if (clientfd < 0) {
            perror("ERRO. Falha no accept");
            return EXIT_FAILURE;
        }
        pthread_create(&t, NULL, client, (void *)id);
    }
    
    pthread_join(t, NULL);

    close(clientfd);

    close(serverfd);

    printf("Conexão encerrada.\n\n");

    return EXIT_SUCCESS;
}
