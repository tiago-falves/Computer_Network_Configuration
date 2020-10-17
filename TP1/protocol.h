#define FALSE 0
#define TRUE 1

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>	
#include <strings.h>
#include <signal.h>



//Message configurations
#define RESEND_DELAY 3
#define WRITE_NUM_TRIES 3





typedef enum { EMISSOR,RECEPTOR} connection_type;

int llopen(char* arg,connection_type connection_type);
int llclose(int fd);
int llwrite(int fd, char* buffer, int length);
int llread(int fd, char* buffer);
