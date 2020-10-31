#include "protocol.h"
#include "message.h"
#include "file_handler.h"
#include "application.h"



int sendFile(char * port_num,char * filename){

    //TODO Adicionar verificaçoes de erros
    install_alarm();
    int fd = llopen(port_num,EMISSOR);

    FILE* file = fopen(filename, "rb");
    if (file == NULL){
        perror("Error reading file");
        return -1;
    }

    int fileSize = findSize(filename);

    //printf("Writing data\n");

    if(sendControlPacket(fd,filename,fileSize,PACKET_CTRL_START) != 0){
        printf("Error sendng controll packet\n");
        return 0;
    }

    int ret = 0;
    char* buffer = calloc(DATA_BLOCK_SIZE - 4, sizeof(char));
    int i = 0;

    while (TRUE){
        //int i = 0; //TODO este i devia mesmo estar aqui?
        ret = fread(buffer , sizeof(char), DATA_BLOCK_SIZE - 4, file);

        if (ret <= 0){
            break;
        }

        if(sendDataPacket(fd, buffer, ret, i) != 0){
            printf("Error sending data packet\n");
        }

        i++;
        progressBar(EMISSOR,(1.0*i*(DATA_BLOCK_SIZE-4)/fileSize) *100);
    }



    fclose(file);

    if(sendControlPacket(fd,filename,fileSize,PACKET_CTRL_END) != 0){
        printf("Error sendng controll packet\n");
        return 0;
    }

    if(llclose(fd)<0){
        printf("Error closing connection\n");
    } else printf("Connection closed successfully\n");

    return 1;
}

int retrieveFile(char * port_num){

    char* buffer = malloc(DATA_BLOCK_SIZE);
    int fd = llopen(port_num, RECEPTOR);
    char filename[FILENAME_MAX];
    memset(filename,0,FILENAME_MAX);

    char ctrl_packet[DATA_BLOCK_SIZE];
    memset(ctrl_packet,0,DATA_BLOCK_SIZE);

    printf("Retreiving file...\n");

    while(TRUE){
        int buffer_size = llread(fd, buffer);
        if (buffer_size == -1){
            printf("LLREAD failure\n");
            return 0;
        }
        else if (buffer_size == -2 || buffer_size == -3){
            continue;
        }

        int r = parsePackets(buffer, buffer_size,filename,ctrl_packet);
        if (r == -1){
            printf("Error parsing packets\n");
            return 0;
        }
        else if (r == -2){
            printf("File retrieved successfully\n");
            break;
        }

        memset(buffer, 0, DATA_BLOCK_SIZE);
    }
    
    if(llclose(fd)<0){
        printf("Error closing connection\n");
    } else printf("Connection closed successfully\n");

    return 1;
}

int sendControlPacket(int fd,char * filename,int fileSize,int ctrl){
    u_int file_name_size = strlen(filename);  
    u_int file_size_size = sizeof(u_int);
    if(file_name_size > DATA_BLOCK_SIZE) {
        printf("Error, filename cannot be greater that 255 characters\n");
    }  

    int controlPacketSize = CTRL_PACKET_SIZE(file_name_size);
    char * controlPacket = malloc(controlPacketSize);

    //FILESIZE
    controlPacket[PACKET_CTRL_IDX] = ctrl;
    controlPacket[CTRL_SIZE_T_IDX] = CTRL_SIZE_OCTET;
    controlPacket[CTRL_SIZE_L_IDX] = file_size_size;
    for (int i = 0; i < file_size_size; i++){
        controlPacket[CTRL_SIZE_V_IDX+i] = (u_int8_t) (fileSize >> (8 * i));
    }

    //FILENAME
    controlPacket[CTRL_NAME_T_IDX] = CTRL_NAME_OCTET;
    controlPacket[CTRL_NAME_L_IDX] = file_name_size;
    for (int i = 0; i < file_name_size; i++)
        controlPacket[CTRL_NAME_V_IDX+i] = filename[i];
    
    int ret = llwrite(fd, controlPacket, controlPacketSize);
    free(controlPacket);

    return ret;
}
int parseCtrlPacket(char * buffer,char *  filename){

    //TODO VERIFY CONTROL PACKET

    if ((buffer[PACKET_CTRL_IDX] != PACKET_CTRL_START) && (buffer[PACKET_CTRL_IDX] != PACKET_CTRL_END ) ){
        printf("Error recieving control packet\n");
        return -1;
    }
    if (buffer[CTRL_SIZE_T_IDX] != CTRL_SIZE_OCTET){
        printf("Error recieving size of file\n");
        return -1;
    }

    u_int fileSize = 0;
    memcpy(&fileSize, buffer + CTRL_SIZE_V_IDX, buffer[CTRL_SIZE_L_IDX]);
    //printf("File Size: %u\n",fileSize);

    if (buffer[CTRL_NAME_T_IDX] != CTRL_NAME_OCTET){
        printf("Error recieving name of file\n");
        return -1;
    }
    //char filename[buffer[CTRL_NAME_L_IDX]];
    memcpy(filename, buffer + CTRL_NAME_V_IDX, buffer[CTRL_NAME_L_IDX]);

    //APAGAR TODO

    char *tmp = strdup(filename);

    strcpy(filename, "Images/"); //Put str2 or anyother string that you want at the begining
    strcat(filename, tmp);  //concatenate previous str1
    return 0;
}

int sendDataPacket(int fd,char * data,short dataSize,int nseq){
    int dataPacketSize = DATA_PACKET_SIZE(dataSize);
    char * dataPacket = malloc(dataPacketSize);

    dataPacket[DATA_CTRL_IDX] = DATA_CTRL_VALUE;
    dataPacket[DATA_N_SEQ_IDX] = nseq % 255;
    dataPacket[DATA_L1_IDX] = (u_int8_t) (dataSize);
    dataPacket[DATA_L2_IDX] = (u_int8_t) (dataSize >> 8);

    for (int i = 0; i < dataSize; i++){
        dataPacket[i + DATA_P_INITIAL_IDX] = data[i];
    }

    int ret = llwrite(fd,dataPacket,dataPacketSize);
    free(dataPacket);
    return ret;
}

int parseDataPacket(char * buffer, int nseq,char * filename){
 
    int dataSize = (u_int8_t) buffer[DATA_L1_IDX] + (u_int8_t) buffer[DATA_L2_IDX] * 256;
    char * file_send = calloc(dataSize, sizeof(char));
    memcpy(file_send, buffer + DATA_INF_BYTE, dataSize); 
    write_file(filename, file_send);
    return 1;
    
}

int parsePackets(char * buffer, int buffer_size,char * filename,char * ctrl_packet){

    char cc = buffer[0];
    int nseq = 0;


    if(cc == PACKET_CTRL_END){
        parseCtrlPacket(buffer,filename);
        
        if(compareCtrlPackets(buffer,ctrl_packet) < 0){
            printf("Error: Control packets are not compatible!\n");
            return -1;
        }
        return -2;
    }
    else if (cc == PACKET_CTRL_START){
        memcpy(ctrl_packet,buffer,buffer_size);
        parseCtrlPacket(buffer,filename);
        return 0;
    }
    else if(cc != PACKET_CTRL_DATA){
        printf("Error recieving packet\n");
        for (int i = 0; i < buffer_size; i++){
            printf("%02x ", (unsigned char) buffer[i]);
        }
        printf("\n\n");
        printf("Error parsing packets cc: %02x\n",cc);
        return -1;
    }
    else{

        nseq = (int)buffer[DATA_N_SEQ_IDX];
        if(!parseDataPacket(buffer,nseq,filename)){
            printf("Error parsing data packet: %d",nseq);
            return -1;
        }
    }
    return 0;

}

void progressBar(conn_type type, int progress) {

    char * msg;
    switch (type) {
        case RECEPTOR:
            msg = "Receiving file: ";
            break;
        case EMISSOR:
            msg = "Sending file: ";
            break;
        default:
            msg = "";
    }
    if (progress < 100)
    {
        printf("\r%s%d%%", msg, progress);
        fflush(stdout);
    }
    
    

    if (progress >= 100){
        printf("\r%s%d%%\n", msg, 100);
        fflush(stdout);
        
    } 
}

int compareCtrlPackets(char * ctrl1,char * ctrl2){
   
    u_int fileSize = 0;
    memcpy(&fileSize, ctrl1 + CTRL_SIZE_V_IDX, ctrl1[CTRL_SIZE_L_IDX]);
    //printf("File Size: %u\n",fileSize);

    u_int fileSize2 = 0;
    memcpy(&fileSize2, ctrl2 + CTRL_SIZE_V_IDX, ctrl2[CTRL_SIZE_L_IDX]);
    if(fileSize != fileSize2){
        return -1;
    }

    char filename[FILENAME_MAX];
    memset(filename,0,FILENAME_MAX);

    //char filename[buffer[CTRL_NAME_L_IDX]];
    memcpy(filename, ctrl1 + CTRL_NAME_V_IDX, ctrl1[CTRL_NAME_L_IDX]);

    char filename2[FILENAME_MAX];
    memset(filename2,0,FILENAME_MAX);

    //char filename[buffer[CTRL_NAME_L_IDX]];
    memcpy(filename2, ctrl2 + CTRL_NAME_V_IDX, ctrl2[CTRL_NAME_L_IDX]);

    if(strcmp(filename,filename2) != 0){
        return -1;
    }
    return 0;
}