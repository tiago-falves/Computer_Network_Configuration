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

#include "utils.h"





//Message configurations
#define RESEND_DELAY 3
#define WRITE_NUM_TRIES 3





int llopen(char* arg, conn_type connection_type);
int llclose(int fd);
int llwrite(int fd, char* data, int data_size);
int llread(int fd, char* buffer);


