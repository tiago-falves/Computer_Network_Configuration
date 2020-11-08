#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define NUM_TRANSMISSIONS 3
#define LAYER_TIMEOUT 3

typedef struct{
  char port[20];                 /*Dispositivo /dev/ttySx, x = 0, 1*/
  int baud_rate;                  /*Velocidade de transmissão*/
  unsigned int timeout;          /*Valor do temporizador: 1 s*/
  unsigned int num_transmissions; /*Número de tentativas em caso defalha*/
} link_layer;

/**
 * @brief Opens serial port connection.
 * 
 * @param layer Struct containing necessary information to open connection.
 * @return int Serial port file descriptor.
 */
int open_connection(link_layer layer);

/**
 * @brief Closes serial port connection.
 * 
 * @param fd Serial port file descriptor.
 * @return int Return value of function close(fd).
 */
int close_connection(int fd);