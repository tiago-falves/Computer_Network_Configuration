
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
        printf("Error: socket()");
        return -1;
    }
    /*connect to the server*/
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Error: connect()");
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
            printf("Error: Reading from socket\n");
            return -1;
        }

        if (read_ret == 0)
        {
            printf("Error: Could not reach ready state in ftpRead\n");
            return -1;
        }
    }

    return strlen(buff);
}

int ftpRead(int fd, char *buff)
{
    memset(buff, 0, MAX_SIZE * sizeof(char));
    int n_bytes_read = read(fd, buff, MAX_SIZE);
    if (n_bytes_read < 0)
    {
        printf("Error: Reading from socket\n");
        return -1;
    }

    if(strlen(buff)!=0) printf(">%s\n",buff);
    return n_bytes_read;
}

int ftpWrite(int sockFd, char *buf)
{
    int n_bytes = write(sockFd, buf, strlen(buf));
    if (n_bytes < 0)
    {
        printf("Error: Error write socket message\n");
        return -1;
    }
    return n_bytes;
}

int ftpLogin(int sockFd, char *user, char *pass)
{
    char userMsg[strlen(USER) + strlen(user) + 2];
    char passMsg[strlen(PASS) + strlen(pass) + 2];
    sprintf(userMsg, "%s%s\n", USER, user);
    sprintf(passMsg, "%s%s\n", PASS, pass);
    // sprintf(userMsg, "%s%s\r\n", USER, user);
    // sprintf(passMsg, "%s%s\r\n", PASS, user);

    // SEND USER
    if (ftpWrite(sockFd, userMsg) < 0)
    {
        printf("Error: Sending user\n");
        return -1;
    }

    // RECEIVE ANSWER
    char buff[MAX_SIZE];
    if (ftpRead(sockFd, buff) < 0)
    {
        printf("Error: Error receiving answer after sending user message\n");
        return -1;
    }

    if (strstr(buff, USER_SUCCESSFUL) == NULL)
    {
        printf("Error: Received wrong message after user input\n");
        return -1;
    }

    // SEND PASS
    if (ftpWrite(sockFd, passMsg) < 0)
    {
        printf("Error: Sending Password\n");
        return -1;
    }

    // RECEIVE ANSWER
    if (ftpRead(sockFd, buff) < 0)
    {
        printf("Error: Error receiving answer after sending pass message\n\n");
        return -1;
    }
    if (strstr(buff, INCORRECT_PASS) != NULL)
    {
        printf("Error: Incorrect Password\n");
        return -1;
    }else if (strstr(buff, LOGIN_SUCCESSFUL) == NULL)
    {
        printf("Error: Error Logging In\n");
        return -1;
    }

    return 0;
}

int ftpSetPassiveMode(int sockFd)
{
    char pasvMsg[strlen(PASSIVE_MODE_CMD)+1];
    sprintf(pasvMsg, "%s\n", PASSIVE_MODE_CMD);
    // sprintf(userMsg, "%s%s\r\n", USER, user);
    // sprintf(passMsg, "%s%s\r\n", PASS, user);

    // SEND USER
    if (ftpWrite(sockFd, pasvMsg) < 0)
    {
        printf("Error: Sending Passive command\n");
        return -1;
    }
    char buff[MAX_SIZE];
    if(ftpRead(sockFd,buff)<0){
        printf("Error reading passive mode response\n");
        return -1;
    }

    if (strstr(buff, PASSIVE_MODE_SUCC_CODE) == NULL) {
        printf("Error setting passive mode\n");
        return -1;
    }

    int ip1,ip2,ip3,ip4,portHigh,portLow;

    if(sscanf(buff,"227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)",&ip1,&ip2,&ip3,&ip4,&portHigh,&portLow)<0){
        printf("Error parsing passive mode msg\n");
        return -1;
    }
    int portNumber = portHigh * 256 + portLow;

    char ipAdress[30];
    sprintf(ipAdress,"%d.%d.%d.%d",ip1,ip2,ip3,ip4);
    printf("Port Number: %d\n",portNumber);
    printf("IP Adress: %s\n",ipAdress);

    if(ftpOpenConnection(ipAdress, portNumber)<0){
        printf("Error Opening new connection after passive mode\n");
        return -1;
    }

    return 0;
}

int ftpCloseConnection(int sockFd)
{
    close(sockFd);
    return 0;
}
