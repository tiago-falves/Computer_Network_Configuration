#include <stdbool.h>


typedef struct {
  char *user;
  char * password;
  char * host;
  char * url_path;
} arguments;

/**
 * @brief Parses command line arguments int appropiate structr arguments.
 * 
 * @param args 
 * @param argc 
 * @param argv 
 * @return  
 */
void parse_arguments(arguments *args, int argc, char *argv[]);

 
/**
 * @brief Checks if command line arguments were correctly introduced, returning true or false accordingly.
 * 
 * @param argc 
 * @param argv 
 * @return true
 * @return false 
 */
bool check_arg(int argc, char* argv[]);

int hasUserPassword(char *str);

int parseUserPassword(char *str,arguments* args,int userPasswordEnd);

void printSeparator();


