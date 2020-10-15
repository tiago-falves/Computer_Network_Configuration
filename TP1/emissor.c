#include "protocol.h"
#include "tramas.h"

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
	int fd = establish_connection(argv[1], &oldtio);

	tramaS trama;
	trama.F1 = F;
	trama.F2 = F;
	trama.C = SET;
	trama.A = AREC;

	printf("New termios structure set\n");
		
	/* writing */ 
	int res = write(fd, &trama, sizeof(trama));

	/* read message back */
	tramaS received;

	read(fd, &received, sizeof(tramaS));
	printf("F1: %04x  F2: %04x\n", received.F1, received.F2);
	printf("C: %04x\n", received.C);
	printf("A: %04x\n", received.A);

	close_connection(fd, &oldtio);
	return 0;
}