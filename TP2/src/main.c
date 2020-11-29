
#include "utils.h"
#include "stdlib.h"

#define ARGUMENT_MAX_SIZE 255

int main(int argc, char** argv)
{	


	arguments args;
    args.user =(char*) malloc(ARGUMENT_MAX_SIZE);
    args.password =(char*) malloc(ARGUMENT_MAX_SIZE);
    args.url_path =(char*) malloc(ARGUMENT_MAX_SIZE);
    args.host =(char*) malloc(ARGUMENT_MAX_SIZE);
	parse_arguments(&args,argc,argv);
	
	
	return 0;
}