#define SERVER_PORT 21 //TODO QUAL é a PORT?
#define SERVER_ADDR "192.168.28.96"
#define MAX_SIZE 255
#define USER "user "
#define PASS "pass "
#define USER_SUCCESSFUL "331"
#define LOGIN_SUCCESSFUL "230"
#define INCORRECT_PASS "530"
#define PASSIVE_MODE_CMD "pasv"
#define RETR_CMD "retr "
#define PASSIVE_MODE_SUCC_CODE "227"
#define FAILED_OPEN_FILE "550"

/**
 * Struct that contains the control and data file descriptors for the FTP
 */
struct ftp {
    int socket_fd; 
    int data_socket_fd; 
};

typedef struct pasv {
    int port_number;
    char* ip_address;
} pasv_info;


int ftpOpenConnection(char *serverAddr, int serverPort);
int ftpWrite(int sockFd,char * buf);
int ftpPollRead(int fd, const char *ready_state, char *buff);
int ftpLogin(int sockFd, char * user, char * pass);
int ftpRead(int fd, char *buff);
int ftpSetPassiveMode(int sockFd, pasv_info* pasv);
int ftpSendRetr(int sockFd, char *path);
