#include "protocol.h"
#include "message.h"
#include "file_handler.h"
#include "application.h"


int sendFile(char * port_num,char * filename){

    //TODO Adicionar verificaçoes de erros
    install_alarm();
    int fd = llopen(port_num,EMISSOR);
    char* buffer = read_file(filename);
    int iterations = 0;
    char** divided_buffer = divideBuffer(buffer, &iterations);
    if (iterations == 0 || buffer == NULL){
        printf("Error dividing buffer");
        return 0;
    }

    free(buffer);

    /*int fileSize = findSize(filename);

    if(sendControlPacket(fd,filename,fileSize,PACKET_CTRL_START) != 0){
        printf("Error sendng controll packet\n");
        return 0;
    }*/

    printf("Writing data\n");

    for (int i = 0; i < iterations; i++) {
        /*if(sendDataPacket(fd,divided_buffer[i],strlen(divided_buffer[i]),i) != 0){
            printf("Error sending data packet\n");
        }*/
        if (llwrite(fd, divided_buffer[i], strlen(divided_buffer[i])) == -1) {
            printf("LLWRITE: error\n");
        }
    }
    llclose(fd);

    return 1;
}
int retrieveFile(char * port_num){
    //TODO Adicionar verificaçoes de erros

    char buffer[] = "";
    int fd = llopen(port_num,RECEPTOR);

    //Como esta a funcionar o llread?
    //Poe a data no buffer? senap devia right?
    if (llread(fd, buffer) == 0){
        printf("File received successfully!\n");
    }else {
        printf("LLREAD failure\n");
        return 0;
    }

    int position = 0;

    if(!parseCtrlPacket(buffer,&position)){
        printf("Error parsing Initial control packet\n");
        return 0;
    }
    if(!parseDataPackets(buffer,position)){
        printf("Error parsing Data Packets\n");
        return 0;
    }
    if(!parseCtrlPacket(buffer,&position)){
        printf("Error parsing end control packet\n");
        return 0;
    }


    llclose(fd);

    return 1;
}

int sendControlPacket(int fd,char * filename,int fileSize,int ctrl){
    u_int file_name_size = strlen(filename) + 1;  
    u_int file_size_size = sizeof(u_int);
    if(file_name_size > 255) {
        printf("Error, filename cannot be greater that 255 characters\n");
    }  

    int controlPacketSize = CTRL_PACKET_SIZE(file_name_size);
    char * controlPacket = malloc(controlPacketSize);

    //FILESIZE
    controlPacket[PACKET_CTRL_IDX] = ctrl;
    controlPacket[CTRL_SIZE_T_IDX] = CTRL_SIZE_OCTET;
    controlPacket[CTRL_SIZE_L_IDX] = file_size_size;
    for (int i = 0; i < file_size_size; i++){
        controlPacket[CTRL_SIZE_V_IDX+i] = (u_int8_t) (fileSize >> 8 * (file_size_size-i-1));
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
int parseCtrlPacket(char * buffer,int * position){

    if ((buffer[PACKET_CTRL_IDX] != PACKET_CTRL_START && *position == 0) || (buffer[PACKET_CTRL_IDX+*position] != PACKET_CTRL_END && *position!=0) ){
        printf("Error recieving control packet\n");
        return 0;
    }
    if (buffer[CTRL_SIZE_T_IDX + *position] != CTRL_SIZE_OCTET){
        printf("Error recieving size of file\n");
        return 0;
    }

    uint fileSize = 0;
    memcpy(&fileSize, buffer + CTRL_SIZE_V_IDX + *position, buffer[CTRL_SIZE_L_IDX+*position]);
    
    printf("File Size: %u\n",fileSize);

    if (buffer[CTRL_NAME_T_IDX + *position] != CTRL_NAME_OCTET){
        printf("Error recieving name of file\n");
        return 0;
    }
    char filename[buffer[CTRL_NAME_L_IDX + *position]];
    memcpy(filename, buffer + CTRL_NAME_V_IDX + *position, buffer[CTRL_NAME_L_IDX + *position]);

    for (int i = 0; i < buffer[CTRL_NAME_L_IDX + *position]; i++) 
        printf("%c",filename[i]);

    
    return 1;
    
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
int parseDataPacket(char * buffer, int nseq,int  * position){
    int dataSize = 256 * buffer[*position+DATA_L1_IDX] + buffer[*position+DATA_L2_IDX];
    for (int i = 0; i < dataSize; i++){
        //Para ja printf mas depois vai escrever para um file né;
        printf("%c",buffer[*position + i]);
    }
    *position += DATA_PACKET_SIZE(dataSize);

    return 1;
    
}

int parseDataPackets(char * buffer,int position){

    char cc;
    int nseq = 0;
 
    while (1){
        cc = buffer[position];
        if(cc == PACKET_CTRL_END){
            printf("Reached end control packet\n");
            return position;
        }else if(cc != PACKET_CTRL_DATA){
            printf("Error parsing data packets\n");
            return 0;
        } else{
            nseq = nseq % 255;
            if(nseq != buffer[position+DATA_N_SEQ_IDX]){
                printf("Error nseq\n");
                return 0;
            }
            if(!parseDataPacket(buffer,nseq,&position)){
                printf("Error parsing data packet: %d",nseq);
                return 0;
            }
        }        
    }
    return 0;

}

