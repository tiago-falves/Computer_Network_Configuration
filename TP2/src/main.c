
#include "utils.h"
#include "stdlib.h"
#include "stdio.h"
#include "ftp.h"
#include "getip.h"
#include "string.h"

#define ARGUMENT_MAX_SIZE 255
#define READY_STATE_WELCOME "220"

int main(int argc, char **argv)
{

	arguments args;
	args.user = (char *)malloc(ARGUMENT_MAX_SIZE);
	args.password = (char *)malloc(ARGUMENT_MAX_SIZE);
	args.url_path = (char *)malloc(ARGUMENT_MAX_SIZE);
	args.host = (char *)malloc(ARGUMENT_MAX_SIZE);
	parse_arguments(&args, argc, argv);

	char *ipAddr = (char *)malloc(20);
	ipAddr = getHostIP(args.host);

	printf("IP Address : %s\n", ipAddr);

	//Establish a TCP connection. This protocol uses port 21 by default
	int sock_fd = ftp_open_connection(ipAddr, SERVER_PORT); // O que por no sever port?

	char buff[MAX_SIZE];

	// After successful connection, the server sends a line of welcome text, for example, 220 welcome.
	if (ftp_poll_read(sock_fd, READY_STATE_WELCOME, buff) < 0)
	{
		printf("Error: Reading welcome message\n");
		return -1;
	}
	//Print Welcome Message
	printf(">%s", buff);

	if (ftp_login(sock_fd, args.user, args.password) < 0)
	{
		printf("Error: Logging In\n");
		return -1;
	}

	//After successful login, send another TYPE I command into binary mode, so that when file data is obtained, it will be sent as a binary byte stream.
	//Avoid sending file data in ASCII format.
	//TODO?

	pasv_info pasv;
	if (ftp_set_passive_mode(sock_fd, &pasv) < 0)
	{
		printf("Error: Setting passive Mode\n");
		return -1;
	}

	int recv_fd;
	if ((recv_fd = ftp_open_connection(pasv.ip_address, pasv.port_number)) < 0)
    {
        printf("Error: Opening new connection after passive mode\n");
        return -1;
    }

	if (ftp_send_retr(sock_fd, args.url_path) < 0)
	{
		printf("Error: Setting passive Mode\n");
		return -1;
	}

	if (ftp_retr_file(recv_fd, args.url_path) < 0)
	{
		printf("Error: Retrieving file\n");
		return -1;
	}

	return 0;
}