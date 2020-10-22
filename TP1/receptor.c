#include "protocol.h"
#include "message.h"

volatile int STOP=FALSE;

int main(int argc, char** argv)
{

	char * initialBufferSize;
	if ( (argc < 2) || 
		((strcmp("/dev/ttyS0", argv[1])!=0) && 
		(strcmp("/dev/ttyS1", argv[1])!=0) )) {
		printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
		exit(1);
	}

	struct termios oldtio;
	int fd = llopen(argv[1],RECEPTOR);

	llread(fd, initialBufferSize);
	
	llclose(fd);
	return 0;
}
