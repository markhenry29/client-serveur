/* serveur_TCP.c (serveur TCP) */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define NBECHANGE 3
char * id = 0;
short port = 0;
int sock = 0; /* socket de communication */
int nb_reponse = 0;
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
serveur.sin_family = AF_INET;
serveur.sin_port = htons(port);
serveur.sin_addr.s_addr = htonl(INADDR_ANY);

if (bind(sock, (struct sockaddr *)&serveur, sizeof(serveur)) < 0) {
	fprintf(stderr,"%s: bind %s\n", argv[0],strerror(errno));
	exit(1);
}
if (listen(sock, 5) != 0) {
	fprintf(stderr,"%s: listen %s\n", argv[0],strerror(errno));
	exit(1);
}
while (1) {
	struct sockaddr_in client; /* SAP du client */
	socklen_t length = sizeof(client);
	int sock_pipe; /* socket de dialogue */
	int ret,nb_question;
	sock_pipe = accept(sock, (struct sockaddr*)&client, &length);
	for (nb_question = 0;nb_question < NBECHANGE; nb_question++) {
		char buf_read[256], buf_write[256];
		ret = read(sock_pipe, buf_read, 256*sizeof(char));
		if (ret <= 0) {
			printf("%s: read=%d: %s\n", argv[0], ret, strerror(errno));
			break;
		}
		printf("srv %s recu de (%s,%4d):%s\n",id, inet_ntoa(client.sin_addr), ntohs(client.sin_port),buf_read);
		sprintf(buf_write,"#%2s=%03d#", id,nb_reponse++);
		ret = write(sock_pipe, buf_write, 256*sizeof(char));
		if (ret <= 0) {
			printf("%s: write=%d: %s\n", argv[0], ret, strerror(errno));
			break;
		}

		sleep(2);
	}
	close(sock_pipe);
	}
return 0;
}
