#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
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

int establishConnection(char* arg, struct termios* oldtio);
int closeConnection(int fd, struct termios* oldtio);
