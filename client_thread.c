#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#define NBECHANGE 3



char * id = 0;
short sport = 0;
int sock = 0; /* socket de communication */
struct hostent *h;
int main(int argc, char ** argv) {
	struct sockaddr_in moi;
	/* SAP du client */
	struct sockaddr_in serveur; /* SAP du serveur */
	if (argc != 4) {
	fprintf(stderr,"usage: %s id serveur port\n",argv[0]);
	exit(1);
	}
	id = argv[1];
	/*h = gethostbyname(argv[2]);
	serveur.sin_addr= *(struct in_addr*)h->h_addr_list[0];*/
	sport = atoi(argv[3]);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr,"%s: socket %s\n", argv[0],strerror(errno));
		exit(1);
	}
	serveur.sin_family = AF_INET;
	serveur.sin_port = htons(sport);
	inet_aton(argv[2], &serveur.sin_addr);
	if (connect(sock, (struct sockaddr*)&serveur, sizeof(serveur)) < 0) {
		fprintf(stderr,"%s: connect %s\n", argv[0],strerror(errno));
		perror("bind");
		exit(1);
	}
	/*len = sizeof(moi);*/
	socklen_t length = sizeof(moi);

	getsockname(sock, (struct sockaddr*)&moi, &length);

	sleep(3);
    close(sock);
return 0;
}
