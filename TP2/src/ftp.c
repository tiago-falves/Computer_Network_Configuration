
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

int ftpPollRead(int fd, const char *ready_state, char *buff)
{
    memset(buff, 0, MAX_SIZE * sizeof(char));

    char *ret;
    int read_ret;
    while ((ret = strstr(buff, ready_state)) == NULL)
    {
        memset(buff, 0, MAX_SIZE * sizeof(char));

        if ((read_ret = read(fd, buff, MAX_SIZE)) == -1)
        {
            perror("Reading from socket\n");
            return -1;
        }

        if (read_ret == 0)
        {
            perror("Could not reach ready state in ftpRead\n");
            return -1;
        }
    }

    return strlen(buff);
}

int ftpRead(int fd, char *buff)
{
    memset(buff, 0, MAX_SIZE * sizeof(char));
    printf("Before read\n");
    int n_bytes_read = read(fd, buff, 1);
    printf("After read\n");

    if (n_bytes_read < 0)
    {
        perror("Reading from socket\n");
        return -1;
    }

    printf("%s", buff);

    return n_bytes_read;
}

int ftpLogin(int sockFd, char *user, char *pass)
{
    char userMsg[strlen(USER) + strlen(user)];
    char passMsg[strlen(PASS) + strlen(pass)];
    sprintf(userMsg, "%s%s", USER, user);
    sprintf(passMsg, "%s%s", PASS, user);

    // SEND USER
    if (ftpWrite(sockFd, userMsg) < 0)
    {
        perror("Sending user\n");
        return -1;
    }

    printf("%s\n", userMsg);

    // RECEIVE ANSWER
    char buff[MAX_SIZE];
    if (ftpRead(sockFd, buff) < 0)
    {
        perror("Error receiving answer after sending user message\n");
        return -1;
    }

    if (strstr(buff, USER_SUCCESSFUL) == NULL)
    {
        perror("Received wrong message after user input\n");
        return -1;
    }
    printf("%s\n", buff);

    // SEND PASS
    if (ftpWrite(sockFd, passMsg) < 0)
    {
        perror("Sending Password\n");
        return -1;
    }

    // RECEIVE ANSWER
    if (ftpRead(sockFd, buff) < 0)
    {
        perror("Incorrect passowrd\n");
        return -1;
    }
    if (strstr(buff, INCORRECT_PASS) != NULL)
    {
        perror("Incorrect Password\n");
        return -1;
    }else if (strstr(buff, LOGIN_SUCCESSFUL) == NULL)
    {
        perror("Error Logging In\n");
        return -1;
    }else{
        printf("NAO FAÇO IDEIA LOGIN\n");
    }

 


    printf("%s\n", passMsg);
    printf("%s\n", buff);

    return 0;
}

int ftpWrite(int sockFd, char *buf)
{
    int n_bytes = write(sockFd, buf, strlen(buf));
    if (n_bytes < 0)
    {
        perror("Error write socket message\n");
        return -1;
    }
    return n_bytes;
}
int ftpCloseConnection(int sockFd)
{
    close(sockFd);
    return 0;
}
