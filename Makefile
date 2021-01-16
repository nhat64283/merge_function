CFLAGS =-Wall
CC = gcc

client: ServerAPI.o  tcp_client.o FileTransferClient.o
	${CC} ${CFLAGS} tcp_client.o ServerAPI.o FileTransferClient.o -o client

server: ServerFuncs.o tcp_server.o FileTransferServer.o
	${CC} ${CFLAGS} -lpthread tcp_server.o ServerFuncs.o FileTransferServer.o -o server

tcp_server.o : tcp_server.c
	${CC} -c ${CFLAGS} -lpthread tcp_server.c

tcp_client.o : tcp_client.c
	${CC} -c ${CFLAGS} tcp_client.c

ServerFuncs.o :ServerFuncs.c 
	${CC} -c ${CFLAGS} ServerFuncs.c 

ServerAPI.o: ServerAPI.c 
	${CC} -c ${CFLAGS} ServerAPI.c 

FileTransferServer.o : FileTransferServer.c
	${CC} -c ${CFLAGS} FileTransferServer.c

FileTransferClient.o: FileTransferClient.c
	${CC} -c ${CFLAGS} FileTransferClient.c
	
clean: 
	rm -f *.o *~