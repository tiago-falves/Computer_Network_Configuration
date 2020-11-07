// control packet
#define CTRL_SIZE_OCTET             0
#define CTRL_NAME_OCTET             1
#define CTRL_SIZE_T_IDX             1
#define CTRL_SIZE_L_IDX             2
#define CTRL_SIZE_V_IDX             3
#define CTRL_NAME_T_IDX             7
#define CTRL_NAME_L_IDX             8
#define CTRL_NAME_V_IDX             9
#define CTRL_PACKET_SIZE(fname_len) 2*sizeof(u_int) + fname_len + 7
#define DATA_PACKET_MAX_SIZE        4096

// control 
#define PACKET_CTRL_IDX        0
#define PACKET_CTRL_DATA       1
#define PACKET_CTRL_START      2
#define PACKET_CTRL_END        3


//Data packet
#define DATA_PACKET_SIZE(data_size) data_size + 4
#define DATA_CTRL_IDX 0
#define DATA_N_SEQ_IDX 1
#define DATA_L1_IDX 2
#define DATA_L2_IDX 3
#define DATA_P_INITIAL_IDX 4

#define DATA_CTRL_VALUE 1

#define PROGRESS_BAR_SIZE 50

typedef struct{
    char * control_packet;
    int size;
} ctrl_packet;

/**
 * @brief Main function of the emitter side of the application, sends files to the receptor.
 * 
 * @param port_num "dev/ttySX" X being the number corresponding to the port.
 * @param filename Name of the file to be sent.
 * @param data_size Size, in bytes, of each data packet.
 * @return int 
 */
int sendFile(char * port_num, char * filename, int data_size);

/**
 * @brief Main function of the receptor side of the application, receives file from emitter.
 * 
 * @param port_num "dev/ttySX" X being the number corresponding to the port.
 * @return int -1 in case of failure, 0 in case of success.
 */
int retrieveFile(char * port_num);

/**
 * @brief Builds and sends control packet, calling the llwrite function.
 * 
 * @param fd File descriptor to write to.
 * @param filename Name of the file to be sent.
 * @param fileSize Size, in bytes, of the file to be sent.
 * @param ctrl Control field (2 - start; 3 - end).
 * @return int Value returned by the call of the llwrite function.
 */
int sendControlPacket(int fd, char * filename, int fileSize, int ctrl);

/**
 * @brief Gets the filename, filesize and data packet size from the control packet.
 * 
 * @param buffer Buffer containing the control packet.
 * @param filename Buffer where the filename will be written to.
 * @return int -1 in case of failure, 0 in case of success.
 */
int parseCtrlPacket(char * buffer, char * filename);

/**
 * @brief Compares content of start and end control packets
 * 
 * @param ctrl1 Start control packet.
 * @param ctrl2 End control packet.
 * @return int 0 if packets have the same content, -1 if they are different.
 */
int compareCtrlPackets(char * ctrl1, char * ctrl2);

/**
 * @brief Builds and sends data packet, calling the llwrite function.
 * 
 * @param fd File descriptor to write to.
 * @param data Bytes read from file.
 * @param dataSize Number of bytes read from file.
 * @param nseq Sequence number of data packet.
 * @return int Value returned by the call of the llwrite function.
 */
int sendDataPacket(int fd, char * data, short dataSize, int nseq);

/**
 * @brief Parses content from data packet and writes it to file.
 * 
 * @param buffer Buffer that contains data to be parsed.
 * @param nseq Sequence number of data packet.
 * @param filename Name of the file where data will be written to.
 * @return int 
 */
int parseDataPacket(char * buffer, int nseq,char * filename);

/**
 * @brief Generic function to parse application packets. Calls functions parseDataPacket and parseControlPacket depending on the control field from the received packet. If packet is the first control packet, content is copied to char* ctrl_packet.
 * 
 * @param buffer Received packet.
 * @param buffer_size Size of the received packet.
 * @param filename Name of the file where data will be written to.
 * @param ctrl_packet Buffer that keeps first control packet.
 * @return int -1 in case of error, 0 in case of success, -2 if control end packet is received.
 */
int parsePackets(char * buffer, int buffer_size,char * filename,char * ctrl_packet);

/**
 * @brief Displays progress bar.
 * 
 * @param type EMISSOR | RECEPTOR
 * @param progress Current progress.
 */
void progressBar(conn_type type, int progress);