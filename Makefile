
CC = gcc

all:	pruebaConexion

pruebaConexion:
	$(CC) servidor.c -o server
	$(CC) cliente.c -o cliente
