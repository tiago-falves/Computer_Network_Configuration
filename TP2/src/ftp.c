
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
#include "string.h"

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
        return -1;
    }
    /*connect to the server*/
    if (connect(sockfd,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0)
    {
        perror("connect()");
        return -1;
    }
    return sockfd;
}

int ftpRead(int sockFd){
    char * buf = malloc(255);
	int	bytes;
    /*send a string to the server*/
	bytes = read(sockFd, buf, strlen(buf));
	printf("Bytes lidos %d\n", bytes);
    return bytes;
}

int ftpWrite(int sockFd,char * buf){
	int	bytes;
    /*send a string to the server*/
	bytes = write(sockFd, buf, strlen(buf));
	printf("Bytes escritos %d\n", bytes);
	printf("Mensagem escrita: %s\n", buf);
    return bytes;
}
int ftpCloseConnection(int sockFd){
	close(sockFd);
    return 0;
}



