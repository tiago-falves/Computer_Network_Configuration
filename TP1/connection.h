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



int open_connection(link_layer layer);
int close_connection(int fd);