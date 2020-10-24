#include "protocol.h"
#include "message.h"
#include "file_handler.h"

volatile int STOP=FALSE;

int main(int argc, char** argv)
{	
	if ( (argc < 2) || 
			((strcmp("/dev/ttyS0", argv[1])!=0) && 
			(strcmp("/dev/ttyS1", argv[1])!=0) )) {
		printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS1\n");
		exit(1);
	}
	install_alarm();

	int fd = llopen(argv[1],EMISSOR);

	char* buffer = read_file("test.txt");
	
	int iterations = 0;
	char** divided_buffer = divideBuffer(buffer, &iterations);
	if (iterations == 0 || buffer == NULL){
		printf("Error dividing buffer");
	}

	free(buffer);
	printf("Writing data\n");

	for (int i = 0; i < iterations; i++) {
		if (llwrite(fd, divided_buffer[i], strlen(divided_buffer[i])) == -1) {
			printf("LLWRITE: error\n");
		}
	}

	llclose(fd);
	return 0;
}