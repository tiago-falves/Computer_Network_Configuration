#include "protocol.h"
#include "application.h"
volatile int STOP=FALSE;

int main(int argc, char** argv)
{	
	arguments args = parse_arguments(argc,argv);

    if(args.role == EMISSOR){
        sendFile(args.port_num,args.filename);
        
    }else if(args.role == RECEPTOR){
        retrieveFile(args.port_num);
        
    }
	
	return 0;
}