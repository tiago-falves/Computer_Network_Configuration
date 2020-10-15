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
	int fd = establishConnection(argv[1], &oldtio);

	tramaS trama;

	int check = read(fd, &trama, sizeof(trama));
	printf("CHECK: %d\n", check);

	sleep(10);

	printf("F1: %04x  F2: %04x\n", trama.F1, trama.F2);
	printf("C: %04x\n", trama.C);
	printf("A: %04x\n", trama.A);

	write(fd, &trama, sizeof(trama));

	closeConnection(fd, &oldtio);
	return 0;
}
