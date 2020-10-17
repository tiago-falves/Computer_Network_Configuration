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

#define MESSAGE_LENGTH 5

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

int write_set_message(int fd);

void atende(int signo);
void install_alarm();
void write_set_retry(int fd);