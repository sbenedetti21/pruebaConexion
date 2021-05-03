/*
 * cliente.c
 *
 *  Created on: 30 abr. 2021
 *      Author: utnso
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PUERTO "3490"
#define CONEXIONES_EN_COLA 5
#define BUFFER_SIZE 1024
#define CONEXIONES_TOTALES 3

void atenderCliente(int);

int main() {

	struct addrinfo hints;
	struct addrinfo *servinfo;
	int socketfd;

	pid_t childpid;


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6, AF_INET or AF_INET6 if you want one or the other specifically
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE; // fill in my IP for me

	getaddrinfo(NULL, PUERTO, &hints, &servinfo);

	socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	bind(socketfd, servinfo->ai_addr, servinfo->ai_addrlen);

	freeaddrinfo(servinfo);

	listen(socketfd, CONEXIONES_EN_COLA);

	int socketCliente;
	int a = 0;
	pthread_t t1,t2, t3;
	pthread_t hilos[] = {t1, t2, t3};

	struct sockaddr_in addr;			// Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
	socklen_t addrlen = sizeof(addr);

	while(a < CONEXIONES_TOTALES) {

		socketCliente = accept(socketfd, (struct sockaddr *) &addr, &addrlen);

		printf("Conexion iniciada con %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

		pthread_create(hilos[a], NULL, (void*) atenderCliente, socketCliente);

		a++;
		//close(socketCliente);
		printf("llegue aca 0 \n");
	}

	for (int i = 0; i < CONEXIONES_TOTALES; i++) {
		pthread_join(hilos[i], NULL);
	}

	close(socketCliente);

	return 0;
}


 void atenderCliente(int socketCliente) {
	 char buffer[BUFFER_SIZE];
	 int status = 1;

	 while (status != 0){
	 	status = recv(socketCliente, (void*) buffer, BUFFER_SIZE, 0);
	 	if (status != 0) printf("%s", buffer);
	 }

 }


