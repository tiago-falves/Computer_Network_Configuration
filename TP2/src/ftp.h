#define SERVER_PORT 21 //TODO QUAL é a PORT?
#define SERVER_ADDR "192.168.28.96"


int ftpOpenConnection(char *serverAddr, int serverPort);
int ftpRead(int sockFd);
int ftpWrite(int sockFd,char * buf);
