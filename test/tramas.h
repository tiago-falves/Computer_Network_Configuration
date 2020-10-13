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

typedef struct tramaI
{
    unsigned char F1;
    unsigned char A;
    unsigned char C;
    unsigned char BCC1;
    unsigned char BCC2;
    unsigned char F2;
};

typedef struct tramaS
{
    unsigned char F1;
    unsigned char A;
    unsigned char C;
    unsigned char BCC1;
    unsigned char F2;
};