/* SET & DISC are commands, the rest are answers */
#define SET 0x03
#define DISC 0x0b 
#define UA 0x07
#define RR(s) (((s) << 7) | 0x03)
#define REJ(s) (((s) << 7) | 0x01) 

// Trama array positions
#define FLAGI_POSTION 0
#define ADRESS_POSITION 1
#define CC_POSITION 2 //Control camp position
#define PC_POSITION 3 //Protection camp position
#define FLAGF_POSTION 4

/* Trama delimiting flag */
#define FLAG 0x7e

/* A field for commands sent by receptor(REC) and emissor(EM) */
#define AREC 0x03 //Commands sent by emissor and responses by receptor
#define AEM 0x01  //Commands sent by receptor and responses by emissor

/* Escape byte*/
#define ESC 0x7d

/* MACROS for byte stuffing */ 
#define FLAG_STUFFING_BYTE 0x5e
#define ESC_STUFFING_BYTE 0x5d

//Size of the information message
#define INFO_SIZE_MSG(data_size)    ((data_size) + 8)   

//Initial position of data blocks in information frame
#define DATA_INF_BYTE 4

//Supervision frame size
#define SUPERVISION_TRAMA_SIZE 6

//Info Message
#define CC_INFO_MSG(s)           (((s) % 2) << 6) 

//Information frame data block size
#define DATA_BLOCK_SIZE 255

typedef struct {
    int stuffed_data;      /** Number of data bytes that were processed and stuffed */
    int stuffed_data_size;     /** Number of bytes that were occupied in the stuffing buffer */
} data_stuffing_t;


void atende(int signo);
void install_alarm();
void reset_alarm();

int readSupervisionMessage(int fd);

char* readMessage(int fd, int* size, int i_message);

void printSupervisionMessage(char * trama);
void printInformMessage(char * trama,int dataSize,int data);

int write_supervision_message(int fd, char cc_value);
int write_supervision_message_retry(int fd, char cc_value);

int write_inform_message_retry(int fd,char cc_value,int dataSize,char * buffer);
int write_info_message(int fd,char * data,int data_size, char cc_value);

char buildBCC2(char * data, int data_size);

char** divideBuffer(char* buffer, int* size);