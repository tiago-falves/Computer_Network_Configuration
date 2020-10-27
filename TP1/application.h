// control packet
#define CTRL_SIZE_OCTET             0
#define CTRL_NAME_OCTET             1
#define CTRL_SIZE_T_IDX             1
#define CTRL_SIZE_L_IDX             2
#define CTRL_SIZE_V_IDX             3
#define CTRL_NAME_T_IDX             7
#define CTRL_NAME_L_IDX             8
#define CTRL_NAME_V_IDX             9
#define CTRL_PACKET_SIZE(fname_len) sizeof(u_int) + fname_len + 5
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





typedef struct{
    char * control_packet;
    int size;
} ctrl_packet;


int retrieveFile(char * port_num);
int sendFile(char * port_num,char * filename);

int sendControlPacket(int fd,char * filename,int fileSize,int ctrl);
int parseCtrlPacket(char * buffer);

int sendDataPacket(int fd,char * data,short dataSize,int nseq);
int parseDataPacket(char * buffer, int nseq);
int parsePackets(char * buffer, int buffer_size);