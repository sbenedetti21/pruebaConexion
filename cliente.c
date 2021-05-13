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

#define PUERTO "3490"
#define IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {

	struct addrinfo hints;
	struct addrinfo *servinfo;
	int socketfd;
	char buffer[BUFFER_SIZE];
	int enviar = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6, AF_INET or AF_INET6 if you want one or the other specifically
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

	getaddrinfo(IP, PUERTO, &hints, &servinfo);

	socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	connect(socketfd, servinfo->ai_addr, servinfo->ai_addrlen);

	freeaddrinfo(servinfo);

	recv(socketfd, &buffer, 21, 0);
	printf("%s \n",buffer);

	while(enviar){
			fgets(buffer, BUFFER_SIZE, stdin);			// Lee una linea en el stdin (lo que escribimos en la consola) hasta encontrar un \n (y lo incluye) o llegar a PACKAGESIZE.
			if (!strcmp(buffer,"exit\n")) enviar = 0;			// Chequeo que el usuario no quiera salir
			if (enviar) send(socketfd, buffer, strlen(buffer) + 1, 0); 	// Solo envio si el usuario no quiere salir.
	}

	close(socketfd);

	return 0;
}
