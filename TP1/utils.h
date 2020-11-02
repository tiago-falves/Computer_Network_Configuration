#include <stdbool.h>

typedef enum{
    EMISSOR,
    RECEPTOR    
} conn_type;

typedef struct {
  conn_type role;
  char *filename;
  char * port_num;
  int data_block_size;
} arguments;

#define PORT_ARG "/dev/ttyS"

arguments parse_arguments(int argc, char *argv[]);
bool check_arg(int argc, char* argv[]);
char* concat(const char *s1, const char *s2);
void log_message(char* msg, int sent);
