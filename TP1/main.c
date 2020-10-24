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
        llwrite(fd, buffer, strlen(buffer));
        llclose(fd);
    }else if(args.role == RECEPTOR){
        char * buffer;
        struct termios oldtio;
        int fd = llopen(args.port_num,RECEPTOR);
        if(!llread(fd, buffer)){
            //write_file("new_file.txt",buffer);
        }
        llclose(fd);
    }
	
	return 0;
}