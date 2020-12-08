#define SERVER_PORT 21 //TODO QUAL é a PORT?
#define SERVER_ADDR "192.168.28.96"
#define MAX_SIZE 255
#define USER "user "
#define PASS "pass "
#define USER_SUCCESSFUL "331"
#define LOGIN_SUCCESSFUL "230"
#define INCORRECT_PASS "530"


int ftpOpenConnection(char *serverAddr, int serverPort);
int ftpWrite(int sockFd,char * buf);
int ftpPollRead(int fd, const char *ready_state, char *buff);
int ftpLogin(int sockFd, char * user, char * pass);
int ftpRead(int fd, char *buff);
