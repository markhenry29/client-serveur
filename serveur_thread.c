/* serveur_TCP.c (serveur TCP) */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>



struct tab_sock{
    int socket[10];
    int point;
    sem_t access;
};

struct tab_sock tab_s;

int retirer()
{
    if(tab_s.point == 0)
    {
        printf("Il n'y a pas de socket à retirer\n");
        return -1;
    }

    else
    {
        tab_s.point--;
        return tab_s.socket[tab_s.point];
    }

}

void ajouter(int sock)
{
    if(tab_s.point >= 10)
        printf("il n'y a plus de place !\n");
    else
    {
        tab_s.socket[tab_s.point] = sock;
        tab_s.point++;
    }
}

void traitement_client(int sock)
{

}

void affichage_client(int sock)
{
    struct sockaddr_in tmp;
    socklen_t length = sizeof(tmp);

    getsockname(sock,(struct sockaddr*)&tmp, &length);
    printf("Client IP:%s port:%d\n", inet_ntoa(tmp.sin_addr), ntohs(tmp.sin_port));
}
void* traitement_thread(void* arg)
{
    while(1)
    {
        sem_wait(&tab_s.access);
        int socket_client = retirer(&tab_s);
        sem_post(&tab_s.access);
        affichage_client(socket_client);
        traitement_client(socket_client);
        close(socket_client);
    }
}

/* variables globales */
char * id = 0;
short port = 0;
int sock = 0; /* socket de communication */
int main(int argc, char ** argv) {


    struct sockaddr_in serveur; /* SAP du serveur */

    if (argc != 3) {
        fprintf(stderr,"usage: %s id port\n",argv[0]);
        exit(1);
    }

    id = argv[1];
    port = atoi(argv[2]);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr,"%s: socket %s\n", argv[0],strerror(errno));
        exit(1);
    }

    /* les threads ici */
    pthread_t tabThreads[10];
    for(int i=0; i<10;i++)
        pthread_create(&tabThreads[i], NULL, traitement_thread, NULL);

    tab_s.point = 0;
    sem_init(&tab_s.access, 0, 0);
    serveur.sin_family = AF_INET;
    serveur.sin_port = htons(port);
    serveur.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&serveur, sizeof(serveur)) < 0) {
        fprintf(stderr,"%s: bind %s\n", argv[0],strerror(errno));
        exit(1);
    }
    if (listen(sock, 15) != 0) {
        fprintf(stderr,"%s: listen %s\n", argv[0],strerror(errno));
        exit(1);
    }
    while (1) {
        struct sockaddr_in client; /* SAP du client */
        socklen_t length = sizeof(client);
        int sock_pipe; /* socket de dialogue */

        sock_pipe = accept(sock, (struct sockaddr*)&client, &length);
        printf("ok\n");
        sem_wait(&tab_s.access);
        ajouter(sock_pipe);
        sem_post(&tab_s.access);
        close(sock_pipe);
    }
    return 0;
}
