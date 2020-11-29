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

/**
 * @brief Parses command line arguments int appropiate structr arguments.
 * 
 * @param argc 
 * @param argv 
 * @return arguments 
 */
arguments parse_arguments(int argc, char *argv[]);

/**
 * @brief Checks if command line arguments were correctly introduced, returning true or false accordingly.
 * 
 * @param argc 
 * @param argv 
 * @return true
 * @return false 
 */
bool check_arg(int argc, char* argv[]);

/**
 * @brief Concatenates two buffers.
 * 
 * @param s1 
 * @param s2 
 * @return char* 
 */
char* concat(const char *s1, const char *s2);


