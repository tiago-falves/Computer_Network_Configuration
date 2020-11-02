/* SET & DISC are commands, the rest are answers */
#define SET 0x03
#define DISC 0x0b 
#define UA 0x07
#define RR(s) ((((s + 1) % 2) << 7) | 0x03)
#define REJ(s) ((((s + 1) % 2) << 7) | 0x01) 

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

/* Stuffing */
#define STUFF 0x20

#define TEST 0xc0

//Size of the information message
#define INFO_SIZE_MSG(data_size)    ((data_size) + 6)   

//Initial position of data blocks in information frame
#define DATA_INF_BYTE 4

//Supervision frame size
#define SUPERVISION_TRAMA_SIZE 5

//Info Message
#define CC_INFO_MSG(s)           (((s) % 2) << 6) 

//Control fiel frame position
#define CTRL_POS 2

//Number of messages without answer
#define ERR_LIMIT 5

typedef struct {
    int stuffed_data;      /** Number of data bytes that were processed and stuffed */
    int stuffed_data_size;     /** Number of bytes that were occupied in the stuffing buffer */
} data_stuffing_t;


void atende(int signo);
void install_alarm();
void reset_alarm();

char* readMessage(int fd, int* size, int i_message, int emissor);

void printSupervisionMessage(char * trama, int onlyC);
void printInformMessage(char * trama,int dataSize,int data);
void printDataInfoMsg(char * trama,int trama_size);

int write_supervision_message(int fd, char cc_value);
int write_supervision_message_retry(int fd, char cc_value);

int write_info_message(int fd, char * data, int data_size, int cc_value);
int write_inform_message_retry(int fd, char * buffer, int dataSize);

char buildBCC2(char * data, int data_size);
int verifyBCC(char * inform,int infMsgSize,char * data,int dataSize);
int parseARQ(char* buffer);
int getSequenceNumber(char* buffer);
