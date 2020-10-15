#include "protocol.h"

int llopen(char* arg, struct termios* oldtio) {
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

	newtio.c_cc[VTIME]    = 20;   /* inter-character timer unused */
	newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */

    tcflush(fd, TCIOFLUSH);

	if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
		perror("tcsetattr");
		exit(-1);
	}

    return fd;
}

int llclose(int fd, struct termios* oldtio) {
    if ( tcsetattr(fd,TCSANOW,oldtio) == -1) {
		perror("tcsetattr");
		exit(-1);
	}

	sleep(1);	
	close(fd);
	return 0;
}

int llwrite(int fd, char* buffer, int length) {
	int wr = write(fd, buffer, length);

	if (wr != length){
		return -1;
	}
	return 0;
}

int llread(int fd, char* buffer) {
	char r[2];
	int finished = FALSE, rd, pos = 0;

	while (!finished){
		rd = read(fd, r, 1);
		if (rd <= 0){
			return -1;
		}
		else if (r[0] == '\0'){
			finished = TRUE;
		}
		buffer[pos++] = r[0];
	}
	return pos;
}