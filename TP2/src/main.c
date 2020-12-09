
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
	int sockFd = ftpOpenConnection(ipAddr, SERVER_PORT); // O que por no sever port?

	char buff[MAX_SIZE];

	// After successful connection, the server sends a line of welcome text, for example, 220 welcome.
	if (ftpPollRead(sockFd, READY_STATE_WELCOME, buff) < 0)
	{
		printf("Error:  reading welcome message\n");
		return -1;
	}
	//Print Welcome Message
	printf(">%s", buff);

	if (ftpLogin(sockFd, args.user, args.password) < 0)
	{
		printf("Error: Logging In\n");
		return -1;
	}

	if(ftpSetPassiveMode(sockFd)<0){
		printf("Error: Setting passive Mode\n");
		return -1;
	}


	return 0;
}