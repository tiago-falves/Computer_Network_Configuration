
#include "utils.h"
#include "stdlib.h"
#include "stdio.h"
#include "ftp.h"
#include "getip.h"

#define ARGUMENT_MAX_SIZE 255

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

	int sockFd = ftpOpenConnection(ipAddr, SERVER_PORT); // O que por no sever port?

	//char	buf[] = "Mensagem de teste na travessia da pilha TCP/IP\n";  
	ftpWrite(sockFd,"EU QUERO IR");

	ftpRead(sockFd);

	//ftpWrite(sockFd,buf);

	

	// 	unique use case:
	//  connect
	//  login host
	//  passive
	//  get path
	//  success (file saved in CWD) or un-success (indicating failing phase)
	// – challenging programming aspects: gethostbyname, sockets, control connection, passive, data connection

	return 0;
}