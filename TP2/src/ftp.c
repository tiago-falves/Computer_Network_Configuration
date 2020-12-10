
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <strings.h>
#include <fcntl.h>
#include "string.h"

#include "ftp.h"

int ftp_open_connection(char *serverAddr, int serverPort)
{

    int sock_fd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(serverAddr); /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(serverPort);            /*server TCP port must be network byte ordered */

    /*open an TCP socket*/
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error: socket()\n");
        return -1;
    }
    /*connect to the server*/
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect\n");
        printf("Error: connect()\n");
        return -1;
    }
    return sock_fd;
}

int ftp_poll_read(int fd, const char *ready_state, char *buff)
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
            printf("Error: Could not reach ready state in ftp_read\n");
            return -1;
        }
    }

    return strlen(buff);
}

int ftp_read(int fd, char *buff)
{
    memset(buff, 0, MAX_SIZE * sizeof(char));
    int n_bytes_read = read(fd, buff, MAX_SIZE);
    if (n_bytes_read < 0)
    {
        printf("Error: Reading from socket\n");
        return -1;
    }

    if (strlen(buff) != 0)
        printf(">%s\n", buff);
    return n_bytes_read;
}

int ftp_write(int sock_fd, char *buf)
{
    int n_bytes = write(sock_fd, buf, strlen(buf));
    if (n_bytes < 0)
    {
        printf("Error: Error write socket message\n");
        return -1;
    }
    return n_bytes;
}

int ftp_login(int sock_fd, char *user, char *pass)
{
    char userMsg[strlen(USER) + strlen(user) + 2];
    char passMsg[strlen(PASS) + strlen(pass) + 2];
    sprintf(userMsg, "%s%s\n", USER, user);
    sprintf(passMsg, "%s%s\n", PASS, pass);
    // sprintf(userMsg, "%s%s\r\n", USER, user);
    // sprintf(passMsg, "%s%s\r\n", PASS, user);

    // SEND USER
    if (ftp_write(sock_fd, userMsg) < 0)
    {
        printf("Error: Sending user\n");
        return -1;
    }

    // RECEIVE ANSWER
    char buff[MAX_SIZE];
    if (ftp_read(sock_fd, buff) < 0)
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
    if (ftp_write(sock_fd, passMsg) < 0)
    {
        printf("Error: Sending Password\n");
        return -1;
    }

    // RECEIVE ANSWER
    if (ftp_read(sock_fd, buff) < 0)
    {
        printf("Error: Error receiving answer after sending pass message\n\n");
        return -1;
    }
    if (strstr(buff, INCORRECT_PASS) != NULL)
    {
        printf("Error: Incorrect Password\n");
        return -1;
    }
    else if (strstr(buff, LOGIN_SUCCESSFUL) == NULL)
    {
        printf("Error: Error Logging In\n");
        return -1;
    }

    return 0;
}

int ftp_set_passive_mode(int sock_fd, pasv_info *pasv)
{
    char pasvMsg[strlen(PASSIVE_MODE_CMD) + 1];
    sprintf(pasvMsg, "%s\n", PASSIVE_MODE_CMD);

    // SEND USER
    if (ftp_write(sock_fd, pasvMsg) < 0)
    {
        printf("Error: Sending Passive command\n");
        return -1;
    }
    char buff[MAX_SIZE];
    if (ftp_read(sock_fd, buff) < 0)
    {
        printf("Error reading passive mode response\n");
        return -1;
    }

    if (strstr(buff, PASSIVE_MODE_SUCC_CODE) == NULL)
    {
        printf("Error setting passive mode\n");
        return -1;
    }

    int ip1, ip2, ip3, ip4, portHigh, portLow;

    if (sscanf(buff, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &ip1, &ip2, &ip3, &ip4, &portHigh, &portLow) < 0)
    {
        printf("Error parsing passive mode msg\n");
        return -1;
    }
    int portNumber = portHigh * 256 + portLow;

    char *ipAdress = malloc(MAX_SIZE);
    sprintf(ipAdress, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
    printf("Port Number: %d\n", portNumber);
    printf("IP Adress: %s\n", ipAdress);

    pasv->ip_address = ipAdress;
    pasv->port_number = portNumber;

    return 0;
}

int ftp_send_retr(int sock_fd, char *path)
{
    char retrMsg[strlen(RETR_CMD) + 1];
    sprintf(retrMsg, "%s%s\n", RETR_CMD, path);

    printf("%s\n", retrMsg);

    // SEND RETR
    if (ftp_write(sock_fd, retrMsg) < 0)
    {
        printf("Error: Sending RETR Command\n");
        return -1;
    }
    char buff[MAX_SIZE];
    if (ftp_read(sock_fd, buff) < 0)
    {
        printf("Error reading RETR Response\n");
        return -1;
    }

    printf("%s\n", path);
    return 0;
}

int ftp_retr_file(int sock_fd, char *path)
{
    int fd;
    if ((fd = open(path, O_WRONLY | O_CREAT, 0660)) < 0)
    {
        perror("Error creating new file");
        return -1;
    }

    char buff[MAX_SIZE];
    int bytes_read = 0;
    while ((bytes_read = read(sock_fd, buff, MAX_SIZE)) > 0)
    {
        if (write(fd, buff, bytes_read) < 0)
        {
            perror("Error writing to new file");
            return -1;
        }
    }

    if (bytes_read < 0)
    {
        printf("Error reading file\n");
        return -1;
    }

    if (close(fd) == -1)
    {
        perror("Error closing new file descriptor");
        return -1;
    }
    return 0;
}

int ftp_close_connection(int sock_fd)
{
    close(sock_fd);
    return 0;
}
