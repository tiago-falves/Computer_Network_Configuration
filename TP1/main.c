#include "protocol.h"
#include "application.h"
volatile int STOP=FALSE;

int main(int argc, char** argv)
{	
	arguments args = parse_arguments(argc,argv);

    args.data_block_size = 10;

    if(args.role == EMISSOR){
        sendFile(args.port_num,args.filename,args.data_block_size);
        
    }else if(args.role == RECEPTOR){
        retrieveFile(args.port_num);
    }
	
	return 0;
}