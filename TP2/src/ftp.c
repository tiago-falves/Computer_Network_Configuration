
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <strings.h>
#include "ftp.h"

int ftpOpenConnection(char *serverAddr, int serverPort)
{

    int sockfd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(serverAddr); /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(serverPort);            /*server TCP port must be network byte ordered */

    /*open an TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket()");
        exit(0);
    }
    /*connect to the server*/
    if (connect(sockfd,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0)
    {
        perror("connect()");
        exit(0);
    }
    return sockfd;
}

int ftpWrite(int sockFd){
    char	buf[] = "Mensagem de teste na travessia da pilha TCP/IP\n";  
	int	bytes;
    /*send a string to the server*/
	bytes = write(sockFd, buf, strlen(buf));
	printf("Bytes escritos %d\n", bytes);
}
int ftpCloseConnection(int sockFd){
	close(sockFd);
}