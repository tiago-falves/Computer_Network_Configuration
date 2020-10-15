#include "protocol.h"

int establishConnection(char* arg, struct termios* oldtio) {
    int fd, c, res;
	struct termios newtio;
	char buf[255];
	int i, sum = 0, speed = 0;

    fd = open(arg, O_RDWR | O_NOCTTY );
	if (fd <0) {
        perror(arg); 
        exit(-1); 
    }

	if ( tcgetattr(fd, oldtio) == -1) { /* save current port settings */
		perror("tcgetattr");
		exit(-1);
	}

	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;

	newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
	newtio.c_cc[VMIN]     = 5;   /* blocking read until 5 chars received */

    tcflush(fd, TCIOFLUSH);

	if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
		perror("tcsetattr");
		exit(-1);
	}

    return fd;
}

int closeConnection(int fd, struct termios* oldtio) {
    if ( tcsetattr(fd,TCSANOW,oldtio) == -1) {
		perror("tcsetattr");
		exit(-1);
	}

	sleep(1);	
	close(fd);
}