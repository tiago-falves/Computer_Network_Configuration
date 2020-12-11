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


int ftp_open_connection(char *serverAddr, int serverPort);
int ftp_close_connection(int sock_fd);
int ftp_write(int sockFd,char * buf);
int ftp_poll_read(int fd, const char *ready_state, char *buff);
int ftp_login(int sockFd, char * user, char * pass);
int ftp_read(int fd, char *buff);
int ftp_set_passive_mode(int sockFd, pasv_info* pasv);
int ftp_send_retr(int sockFd, char *path);
int ftp_retr_file(int sock_fd, char* path);