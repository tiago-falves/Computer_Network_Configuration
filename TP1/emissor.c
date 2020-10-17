#include "protocol.h"
#include "message.h"

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

	printf("Writing SET message\n");

	if(!write_supervision_message_retry(fd,SET)){
		printf("Error establishing connection\n");
	}

	printf("Reading UA Message\n");
	/*if(!readSupervisionMessage(fd)){
		printf("Error recieving unnumbered acknowledgment)\n");
	}*/

	llclose(fd);
	return 0;
}