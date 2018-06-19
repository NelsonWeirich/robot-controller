#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define BUFFER_LENGTH 1024

int sockfd;

void *conection(void *arg) {
    char buffer_in[BUFFER_LENGTH];
    char buffer_out[BUFFER_LENGTH];

    while (true) {
        memset(buffer_in, 0x0, sizeof(buffer_in));
        memset(buffer_out, 0x0, sizeof(buffer_out));

        fprintf(stdout, "Digite um comando (exit para sair): ");
        fgets(buffer_out, BUFFER_LENGTH, stdin);

        if (send(sockfd, buffer_out, strlen(buffer_out), 0) < 0) {
            perror("ERRO. Falha ao escrever no socket");
            exit(EXIT_FAILURE);
        }

        recv(sockfd, buffer_in, BUFFER_LENGTH, 0);
        printf("\tServidor: %s\n", buffer_in);

        if (strcmp(buffer_in, "exit\n") == 0) { // servidor confirma saida
            break;
        }
    }
}

int main(int argc, char const *argv[]) {
    int portno, n;
    struct sockaddr_in server_addr;
    pthread_t t;

    int len = sizeof(server_addr);
    int slen;

    char buffer[BUFFER_LENGTH];
    if (argc < 3) {
        perror("ERRO. Ausência de argumentos");
        fprintf(stderr, "Uso: %s nomehost porta\n", argv[0]);
        return EXIT_FAILURE;
    }

    portno = atoi(argv[2]);
    if (portno < 1024) {
        perror("ERRO. Porta fora do intervalo definido: >= 1024");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Iniciando cliente.\n");

    // criação do socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERRO. Falha ao abrir o socket");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Socket do servidor criada com fd: %d\n", sockfd);

    // bzero((char *) &server_addr, sizeof(server_addr));
    memset((char *) &server_addr, 0x0, sizeof(server_addr));

    // Propriedades do servidor
    server_addr.sin_family = AF_INET;
    inet_aton(argv[1], &server_addr.sin_addr); // opcao1
    // server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // opcao2
    // server_addr.sin_addr.s_addr = inet_addr(argv[1]); // opcao3
    server_addr.sin_port = htons(portno);
    // memset(server_addr.sin_zero, 0x0, 8);

    // tenta conexao
    if (connect(sockfd, (struct sockaddr *) &server_addr,
                         sizeof(server_addr)) < 0) {
        perror("ERRO. Falha na conexão");
        return EXIT_FAILURE;
    }

    // recebe mensagem de entrada do servidor
    if (read(sockfd, buffer, BUFFER_LENGTH) > 0) {
        fprintf(stdout, "Servidor: %s\n", buffer);
    }

    pthread_create(&t, NULL, conection, NULL);

    pthread_join(t, NULL);

    close(sockfd);

    fprintf(stdout, "Conexão encerrada\n\n");

    return EXIT_SUCCESS;
}
