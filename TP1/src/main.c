#include "protocol.h"
#include "application.h"

#include <time.h>
#define BILLION 1000000000.0

volatile int STOP=FALSE;

int main(int argc, char** argv)
{	
	arguments args = parse_arguments(argc,argv);
	srand(time(0));
    struct timespec start, end;

    if(args.role == EMISSOR){
        clock_gettime(CLOCK_REALTIME, &start);
        sendFile(args.port_num,args.filename,args.data_block_size);
        clock_gettime(CLOCK_REALTIME, &end);
        printf("Time consumed: %fs\n", end.tv_sec - start.tv_sec + (end.tv_nsec - start.tv_nsec) / BILLION - 1);
        
    }else if(args.role == RECEPTOR){
        retrieveFile(args.port_num);
    }
	
	return 0;
}