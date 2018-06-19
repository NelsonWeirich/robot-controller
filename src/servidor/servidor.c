
/** TODO
 *  + adicionar execução das tarefas
 *  + melhorar tratamento de mensagem recebida
 *  + desenvolvimento das tarefas periodicas
 *  + variavel de condição
 *  + escalonamento de prioridade
 *  + colocar em biblioteca separada
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

// #include "../lperiodic.h"

struct periodic_info
{
	int sig;
	sigset_t alarm_sig;
};

static int make_periodic (int unsigned period, struct periodic_info *info)
{
	static int next_sig;
	int ret;
	unsigned int ns;
	unsigned int sec;
	struct sigevent sigev;
	timer_t timer_id;
	struct itimerspec itval;

	/* Initialise next_sig first time through. We can't use static
	   initialisation because SIGRTMIN is a function call, not a constant */
	if (next_sig == 0)
		next_sig = SIGRTMIN;
	/* Check that we have not run out of signals */
	if (next_sig > SIGRTMAX)
		return -1;
	info->sig = next_sig;
	next_sig++; // proteger a variavel
	/* Create the signal mask that will be used in wait_period */
	sigemptyset (&(info->alarm_sig));
	sigaddset (&(info->alarm_sig), info->sig);

	/* Create a timer that will generate the signal we have chosen */
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = info->sig;
	sigev.sigev_value.sival_ptr = (void *) &timer_id;
	ret = timer_create (CLOCK_MONOTONIC, &sigev, &timer_id);
	if (ret == -1)
		return ret;

	/* Make the timer periodic */
	sec = period/1000000;
	ns = (period - (sec * 1000000)) * 1000;
	itval.it_interval.tv_sec = sec;
	itval.it_interval.tv_nsec = ns;
	itval.it_value.tv_sec = sec;
	itval.it_value.tv_nsec = ns;
	ret = timer_settime (timer_id, 0, &itval, NULL);
	return ret;
}

static void wait_period (struct periodic_info *info)
{
	int sig;
	sigwait (&(info->alarm_sig), &sig);
}

#define BUFFER_LENGTH 1024

// Variáveis globais
int clientfd;
int id = 0;

pthread_mutex_t mutex_angulo, mutex_position, mutex_speed = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_angulo, cond_position, cond_speed = PTHREAD_COND_INITIALIZER;

static int position = 0;
static float velocidade, angulo = 0.0;

// tarefas periódicas
static void *controle_velocidade(void *arg) {
    struct periodic_info info;

    make_periodic(10000000, &info); // 10 s
    while (1) {
        pthread_mutex_lock(&mutex_speed);
            printf("\tVelocidade: %f\n", velocidade);
            while (velocidade == 0) {
                pthread_cond_wait(&cond_speed, &mutex_speed);
            }
            // TODO ADICIONAR ALGUMA AÇÃO SOBRE A VARIAVEL
            wait_period(&info);
        pthread_mutex_unlock(&mutex_speed);
    }
}

static void *controle_posicao(void *arg) {
    struct periodic_info info;

    make_periodic(8000000, &info); //8s
    while (1) {
        pthread_mutex_lock(&mutex_position);
            printf("\tPosição: %d\n", position);
            while (position == 0) {
                pthread_cond_wait(&cond_position, &mutex_position);
            }
            // TODO ADICIONAR ALGUMA AÇÃO SOBRE A VARIAVEL
            wait_period(&info);
        pthread_mutex_unlock(&mutex_position);
    }
}

static void *controle_equilibrio(void *arg) {
    struct periodic_info info;

    make_periodic(5000000, &info); //5s
    while (1) {
        pthread_mutex_lock(&mutex_angulo);
            printf("\tÂngulo: %f\n", angulo);
            while (angulo == 0 ) {
                pthread_cond_wait(&cond_angulo, &mutex_angulo);
            }
            // TODO ADICIONAR ALGUMA AÇÃO SOBRE A VARIAVEL
            wait_period(&info);
        pthread_mutex_unlock(&mutex_angulo);
    }
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

            // RIGHT
            if (strcmp(buffer, "right\n") == 0) {
                write(clientfd, "direita\n", 80);
                printf("Executando operação do cliente.\n");
                pthread_mutex_lock(&mutex_angulo);
                    // TODO
                    //execução dos comandos
                    angulo--;
                    pthread_cond_signal(&cond_angulo);
                pthread_mutex_unlock(&mutex_angulo);
            // LEFT
            } else if (strcmp(buffer, "left\n") == 0) {
                write(clientfd, "esquerda\n", 80);
                printf("Executando operação do cliente.\n");
                pthread_mutex_lock(&mutex_angulo);
                    // TODO
                    //execução dos comandos
                    angulo++;
                    pthread_cond_signal(&cond_angulo);
                pthread_mutex_unlock(&mutex_angulo);
            // FORWARD
            } else if (strcmp(buffer, "forward\n") == 0) {
                write(clientfd, "para frente\n", 80);
                printf("Executando operação do cliente.\n");
                pthread_mutex_lock(&mutex_position);
                    // TODO
                    //execução dos comandos
                    position++;
                    pthread_cond_signal(&cond_position);
                pthread_mutex_unlock(&mutex_position);
            // BACK
            } else if (strcmp(buffer, "back\n") == 0) {
                write(clientfd, "voltou\n", 80);
                printf("Executando operação do cliente.\n");
                pthread_mutex_lock(&mutex_position);
                    // TODO
                    //execução dos comandos
                    position--;
                    pthread_cond_signal(&cond_position);
                pthread_mutex_unlock(&mutex_position);
                // STOP
            } else if (strcmp(buffer, "stop\n") == 0) {
                write(clientfd, "parou\n", 80);
                printf("Executando operação do cliente.\n");
                pthread_mutex_lock(&mutex_speed);
                    // TODO
                    //execução dos comandos
                    velocidade = 0;
                    pthread_cond_signal(&cond_speed);
                pthread_mutex_unlock(&mutex_speed);
            // EXIT
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
