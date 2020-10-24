#include "protocol.h"
#include "message.h"
#include "file_handler.h"
volatile int STOP=FALSE;

int main(int argc, char** argv)
{	
	arguments args = parse_arguments(argc,argv);

    if(args.role == EMISSOR){
        install_alarm();
        int fd = llopen(args.port_num,EMISSOR);
        char* buffer = read_file(args.filename);
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
    }else if(args.role == RECEPTOR){
        char buffer[] = "";
        int fd = llopen(args.port_num,RECEPTOR);

        if (llread(fd, buffer) == 0){
		    printf("File received successfully!\n");
        }else {
            printf("LLREAD failure\n");
        }

        llclose(fd);
    }
	
	return 0;
}