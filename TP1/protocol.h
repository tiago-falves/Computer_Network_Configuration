#define BAUDRATE B38400
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>	
#include <strings.h>
#include <signal.h>

/* SET & DISC are commands, the rest are answers */
#define SET 0x03
#define DISC 0x0b 
#define UA 0x07
#define RR 0x05    //RR and REJ need to be changed, meaning of bit R (?)
#define REJ 0x01 

/* Trama delimiting flag */
#define F 0x7e

/* A field for commands sent by receptor(REC) and emissor(EM) */
#define AREC 0x03
#define AEM 0x01

/* Escape byte*/
#define ESC 0x7d

/* MACROS for byte stuffing */
#define SUBSESC 0x5e
#define SUBF 0x5d

int llopen(char* arg, struct termios* oldtio);
int llclose(int fd, struct termios* oldtio);
int llwrite(int fd, char* buffer, int length);
int llread(int fd, char* buffer);
