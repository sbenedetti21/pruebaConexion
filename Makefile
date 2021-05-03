
CC = gcc

all:	pruebaConexion

pruebaConexion:
	$(CC) servidor.c -o server -lpthread
	$(CC) cliente.c -o cliente
