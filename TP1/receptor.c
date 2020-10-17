#include "protocol.h"

volatile int STOP=FALSE;

int main(int argc, char** argv)
{
	if ( (argc < 2) || 
		((strcmp("/dev/ttyS0", argv[1])!=0) && 
		(strcmp("/dev/ttyS1", argv[1])!=0) )) {
		printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
		exit(1);
	}

	struct termios oldtio;
	int fd = llopen(argv[1]);

	char trama[5];
	
	int check = llread(fd, trama);
	//printf("CHECK: %d\n", check);

	if(check==-1) printf("Error reading message\n");
	else{
		printf("Message recieved correctly\n");
		printf("F1: %04x  F2: %04x\n", trama[0], trama[1]);
		printf("C: %04x\n", trama[2]);
		printf("A: %04x\n", trama[3]);
	}

	
	if(llwrite(fd, trama, check)==-1) printf("Error wrting message\n");

	llclose(fd);
	return 0;
}
