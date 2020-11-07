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

/**
 * @brief Alarm siganl handler.
 * 
 * @param signo 
 */
void atende(int signo);

/**
 * @brief Intalls alarm signal.
 * 
 */
void install_alarm();

/**
 * @brief Resets alarm signal and associated variables.
 * 
 */
void reset_alarm();

/**
 * @brief Reads message from serial port, byte per byte, updating associated state machine when a byte is received.
 * 
 * @param fd Serial port file decriptor.
 * @param size Size of the message read.
 * @param i_message 1 if a information frame is being received, 0 if it is a supervision message.
 * @param emissor 1 if message is being read by emissor, 0 otherwise.
 * @return char* Message read.
 */
char* readMessage(int fd, int* size, int i_message, int emissor);

/**
 * @brief Builds and writes supervision frame message.
 * 
 * @param fd Serial port file descriptor.
 * @param cc_value Control field of supervision frame.
 * @return int Number of bytes written (return value of write function).
 */
int write_supervision_message(int fd, char cc_value);

/**
 * @brief Writes supervision frame message and waits for receptor, ending if a valid supervision frame is received or if limit of resending tries is reached.
 * 
 * @param fd Serial port file descriptor.
 * @param cc_value Control field of supervision frame.
 * @param cc_compare Value expected from control field of supervision frame message received from receptor.
 * @return int 0 in case of success, -1 otherwise.
 */
int write_supervision_message_retry(int fd, char cc_value, char cc_compare);

/**
 * @brief Builds and writes information frame message.
 * 
 * @param fd Serial port file descriptor.
 * @param data Data to be inserted into information frame.
 * @param data_size Size, in bytes, of field data.
 * @param cc_value Control field of information frame.
 * @return int 
 */
int write_info_message(int fd, char * data, int data_size, int cc_value);

/**
 * @brief Writes information frame message and waits for receptor, ending if a valid supervision frame is received or if limit of resending tries is reached.
 * 
 * @param fd Serial port file descriptor.
 * @param buffer Data to be inserted into information frame.
 * @param dataSize Size, in bytes, of field data.
 * @return int Number of bytes written (return value of write function).
 */
int write_inform_message_retry(int fd, char * buffer, int dataSize);

/**
 * @brief Calculates bcc2 of data.
 * 
 * @param data 
 * @param data_size Size of data, in bytes.
 * @return char Bcc2 value.
 */
char buildBCC2(char * data, int data_size);

/**
 * @brief Verifies if there are bcc2 errors in the information frame received as parameter.
 * 
 * @param inform Information frame received and unstuffed.
 * @param infMsgSize Information frame size.
 * @param data Data fields in information frame received to calculate bcc2 from.
 * @param dataSize Data size.
 * @return int 0 if bcc2 is successfully verified, -1 otherwise.
 */
int verifyBCC(char * inform, int infMsgSize, char * data, int dataSize);

/**
 * @brief Verifies if supervision frame received as parameter is a REJ message.
 * 
 * @param buffer Supervision frame.
 * @return int 1 if buffer is a REJ message, 0 otherwise.
 */
int parseREJ(char* buffer);

/**
 * @brief Verifies if supervision frame received as parameter is a RR message.
 * 
 * @param buffer Supervision frame.
 * @return int 1 if buffer is a RR message, 0 otherwise.
 */
int parseRR(char* buffer);

/**
 * @brief Get the Sequence Number from information frame received.
 * 
 * @param buffer Information received.
 * @return int Sequence number (0 or 1).
 */
int getSequenceNumber(char* buffer);

/**
 * @brief Set the Block Size value
 * 
 * @param value Size of data block.
 */
void setBlockSize(int value);