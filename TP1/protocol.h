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

// Trama array positions
#define FLAGI_POSTION 0
#define ADRESS_POSITION 1
#define CC_POSITION 2 //Control camp position
#define PC_POSITION 3 //Protection camp position
#define FLAGF_POSTION 4

//Message configurations
#define RESEND_DELAY 3
#define WRITE_NUM_TRIES 3



/* Trama delimiting flag */
#define F 0x7e

/* A field for commands sent by receptor(REC) and emissor(EM) */
#define AREC 0x03 //Commands sent by emissor and responses by receptor
#define AEM 0x01  //Commands sent by receptor and responses by emissor

/* Escape byte*/
#define ESC 0x7d

/* MACROS for byte stuffing */ 
#define FLAG_STUFFING_BYTE 0x5e
#define ESC_STUFFING_BYTE 0x5d

int llopen(char* arg, struct termios* oldtio);
int llclose(int fd, struct termios* oldtio);
int llwrite(int fd, char* buffer, int length);
int llread(int fd, char* buffer);
