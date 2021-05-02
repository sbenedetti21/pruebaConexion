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

#define PUERTO "3490"
#define CONEXIONES_EN_COLA 5
#define BUFFER_SIZE 1024
#define CONEXIONES_TOTALES 3

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
	int status = 1;
	char buffer[BUFFER_SIZE];

	while(a < CONEXIONES_TOTALES) {
		struct sockaddr_in addr;			// Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
		socklen_t addrlen = sizeof(addr);

		socketCliente = accept(socketfd, (struct sockaddr *) &addr, &addrlen);

		printf("Conexion iniciada con %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

		if(childpid = fork() == 0) {
			close(socketfd);

			while (status != 0){
					status = recv(socketCliente, (void*) buffer, BUFFER_SIZE, 0);
					if (status != 0) printf("%s:%d says: %s", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buffer);
			}
		}
		a++;
	}

	if (childpid > 0) {
		waitpid(childpid, NULL, 0);
	}

	close(socketCliente);

	return 0;
}
