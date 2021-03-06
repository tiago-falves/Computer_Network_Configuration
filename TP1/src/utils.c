#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"


char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

bool check_arg(int argc, char* argv[]){
  return (argc >= 3) &&(
         ((argc == 5) && (strcmp("emissor", argv[2]) == 0)) ||
         ((argc == 3) && (strcmp("receptor", argv[2]) == 0)));
}

arguments parse_arguments(int argc, char *argv[]) {
    if (!check_arg(argc, argv)){
        printf("Usage Receptor: main <serial port number> receptor\n");
        printf("Usage Emissor: main <serial port number> emitter (filename) (data_block_size)\n");
        exit(1);
    }

    char * port = concat(PORT_ARG,argv[1]);
    arguments args;
    if (argc == 3) args.role = RECEPTOR;   
    else{
        args.role = EMISSOR;
        args.filename = argv[3];
        args.data_block_size = atoi(argv[4]);
    }
    args.port_num = port;

    return args;
}
