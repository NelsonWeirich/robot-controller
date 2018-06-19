
/** TODO
 *  + adicionar execução das tarefas
 *  + melhorar tratamento de mensagem recebida
 *  + desenvolvimento das tarefas periodicas
 *  + variavel de condição
 *  + escalonamento de prioridade
 *  +
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
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

static int position;
static float velocidade, angulo;

// tarefas periódicas
static void *controle_velocidade(void *arg) {
    struct periodic_info info;

    // TODO
    // MUTEX
    make_periodic(10000, &info); // 10 ms
    while (1) {
        // velocidade++;
        printf("\tVelocidade: %f\n", velocidade);
        wait_period(&info);
    }
    // MUTEX
}

static void *controle_posicao(void *arg) {
    struct periodic_info info;

    // TODO
    // MUTEX
    make_periodic(8000, &info); //8ms
    while (1) {
        // position++;
        printf("\tPosição: %d\n", position);
        wait_period(10000, &info);
    }
    // MUTEX
}

static void *controle_equilibrio(void *arg) {
    struct periodic_info info;
    
    // TODO
    // MUTEX
    make_periodic(5000, &info); //5ms
    while (1) {
        // angulo++;
        printf("Ângulo de equilíbrio: %f\n", angulo);
        wait_period(&info);
    }
    // MUTEX
}

void *client(void *arg) {
    int client_id = (int)arg;
    int i, n, len_msg;
    char buffer[BUFFER_LENGTH];

    strcpy(buffer, "Olá. Estou vivo e posso me mover! Diga os comandos.\n\0");

    if (send(clientfd, buffer, strlen(buffer), 0)) {
        fprintf(stdout, "------------------\n");
        fprintf(stdout, "Cliente conectado.\nAguardando resposta ...\n");

        // comunicação enquanto nao enviar comando exit
        do {
            memset(buffer, 0x0, sizeof(buffer));

            len_msg = read(clientfd, buffer, 80);
            if (len_msg > 0) {
                printf("Cliente: %s", buffer);
            } else {
                break;
            }

            // analisa leitura
            if (strcmp(buffer, "right\n") == 0) {
                write(clientfd, "direita\n", 80);
                printf("Executando operação do cliente.\n");
                // TODO
                // MUTEX
                //execução dos comandos
                // MUTEX
            } else if (strcmp(buffer, "left\n") == 0) {
                write(clientfd, "esquerda\n", 80);
                printf("Executando operação do cliente.\n");
                // TODO
                // MUTEX
                //execução dos comandos
                // MUTEX
            } else if (strcmp(buffer, "forward\n") == 0) {
                write(clientfd, "para frente\n", 80);
                printf("Executando operação do cliente.\n");
                // TODO
                // MUTEX
                //execução dos comandos
                // MUTEX
            } else if (strcmp(buffer, "back\n") == 0) {
                write(clientfd, "voltou\n", 80);
                printf("Executando operação do cliente.\n");
                // TODO
                // MUTEX
                //execução dos comandos
                // MUTEX
            } else if (strcmp(buffer, "stop\n") == 0) {
                write(clientfd, "parou\n", 80);
                printf("Executando operação do cliente.\n");
                // TODO
                // MUTEX
                //execução dos comandos
                // MUTEX
            } else if (strcmp(buffer, "exit\n") == 0) {
                printf("Encerrando sessão.\n");
                printf("------------------\n");
                // TODO
                // MUTEX
                //execução dos comandos
                // MUTEX
                write(clientfd, "exit\n", 8);
            } else if (strcmp(buffer, "help\n") == 0) {
                write(clientfd, "\nright   - virar para direita \
                                 \nleft    - virar para esquerda \
                                 \nforward - avançar \
                                 \nback    - recuar \
                                 \nstop    - parar\n", 256);
            } else { // comando inexistente
                write(clientfd, "Comando inválido.\n \
                                 Tente help para saber mais", 80);
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
    pthread_t tarefa_1;
    pthread_t tarefa_2;
    pthread_t tarefa_3;
    sigset_t alarm_signal;

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

    // listen
    if (listen(serverfd, 1) < 0) {
        perror("ERRO. Falha ao fazer listen");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Listen da porta: %d\n", portno);

    sigemptyset(&alarm_signal);

    for (size_t i = SIGRTMIN; i <= SIGRTMAX; i++)
        sigaddset(&alarm_signal, i);

    sigprocmask(SIG_BLOCK, &alarm_signal, NULL);

    pthread_create(&tarefa_1, NULL, controle_velocidade, NULL);
    pthread_create(&tarefa_2, NULL, controle_posicao, NULL);
    pthread_create(&tarefa_3, NULL, controle_equilibrio, NULL);

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
