#include "protocol.h"
#include "tramas.h"

volatile int STOP=FALSE;

int flag=1, conta=1;

void atende()
{
	printf("alarme # %d\n", conta);
	flag=1;
	conta++;
}

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

	(void) signal(SIGALRM, atende);

	tramaS trama, received;

	trama.F1 = F;
	trama.F2 = F;
	trama.C = SET;
	trama.A = AREC;

	int wr, rd;
	int success = FALSE;

	while(conta < 4 && !success){
		if(flag){
			flag=0;

			/* writing */ 
			wr = write(fd, &trama, sizeof(trama));
			alarm(3);

			/* read message back */
			rd = read(fd, &received, sizeof(received));
			
			if (rd != sizeof(received)) success = FALSE;
			else success = TRUE;
		}
	}

	/* verification */
	printf("F1: %04x  F2: %04x\n", received.F1, received.F2);
	printf("C: %04x\n", received.C);
	printf("A: %04x\n", received.A);

	closeConnection(fd, &oldtio);
	return 0;
}