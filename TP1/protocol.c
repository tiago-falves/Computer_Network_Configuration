#include "protocol.h"
#include "connection.h"
#include "message.h"
#include "state_machine.h"
#include "data_stuffing.h"
#include <time.h>

conn_type connection;
static int nr = 1;

int llopen(char* port, conn_type connection_type) {
	link_layer layer;
	strcpy(layer.port,port);
	layer.baud_rate = BAUDRATE;
  	layer.num_transmissions = NUM_TRANSMISSIONS;
  	layer.timeout = LAYER_TIMEOUT;
	connection = connection_type;
	  
	int fd = open_connection(layer);
	if(connection_type == EMISSOR){
		if(write_supervision_message_retry(fd,SET) == -1){
			printf("Error establishing connection\n");
			return -1;
		}
	}else if (connection_type == RECEPTOR){
		int trama_size = 0;
		char* trama = readMessage(fd, &trama_size, 0, 0);

		if (trama == NULL || trama == 0){
			printf("LLOPEN: error reading SET message\n");
			return -1;
		}

		if (trama[CTRL_POS] != SET) {
			printf("LLOPEN: expected SET message\n");
			return -1;
		}

		if(write_supervision_message(fd,UA) == -1){
			printf("Error writing UA\n");
			return -1;
		}
	}
	return fd;
}

int llclose(int fd) {
	if(connection == EMISSOR){
		if(write_supervision_message_retry(fd,DISC) == -1){
			printf("Error establishing connection\n");
		}
		if(write_supervision_message(fd,UA) == -1){
			printf("Error writing UA\n");
		}
	}
	else if (connection == RECEPTOR) {
		int buffer_size = 0;
		char* temp = readMessage(fd, &buffer_size, 0, 0);

		if (temp[CTRL_POS] == DISC) {
			if (write_supervision_message(fd, DISC) == -1){
				printf("LLCLOSE: error writing DISC message back\n");
				return -2;
			}
		}else{
			printf("Error receiving DISC message\n");
		}

		free(temp);
		temp = readMessage(fd, &buffer_size, 0, 0);

		if (temp == NULL || buffer_size == 0){
			printf("LLCLOSE: error reading UA message after sending DISC\n");
			return -1;
		}

		if (temp[CTRL_POS] != UA) {
			printf("LLCLOSE: wrong message after sending DISC\n");
			return -1;
		}
	}

    return close_connection(fd);
}

int llwrite(int fd, char* buffer, int length) {
	return write_inform_message_retry(fd, buffer, length);
}

int llread(int fd, char* buffer) {
	int temp_size = 0;

	char* temp = readMessage(fd, &temp_size, 1, 0);
	if(temp == NULL || temp_size == 0){
		printf("LLREAD: exit after %d failed attempts to read message\n", ERR_LIMIT);
		return -1;
	}

	//GENERATE NOISE
	/*int r = rand() % 15;
	printf("RANDOM: %d\n", r);

	if (r == 0)
		temp[10] = 0x01;
	if (r == 14) 
		temp[10] = 0x0F;
	if (r == 2)
		temp[10] = 0x08;*/
	//---------------

	int seq_number = getSequenceNumber(temp);
	if (getSequenceNumber(temp) == nr % 2) {
		printf("Received repeated trama\n");
		if (write_supervision_message(fd, RR(seq_number)) == -1){
			printf("LLREAD: error writing RR message back\n");
			return -1;
		}
		nr++;
		return -2;
	}

	data_stuff unstuffedData = unstuffData(temp, temp_size);

	int buffer_size = unstuffedData.data_size - 6;
	memcpy(buffer, unstuffedData.data + DATA_INF_BYTE, buffer_size);
  
	if(verifyBCC(unstuffedData.data,unstuffedData.data_size,buffer,buffer_size) < 0){
		printf("Error verifying BCC\n");
		int rd = write_supervision_message(fd, REJ(seq_number));
		if (rd == -1){
			printf("LLREAD: error writing REJ message back\n");
			return -1;
		}
		return -3;
	}
	if (write_supervision_message(fd, RR(seq_number)) == -1){
		printf("LLREAD: error writing RR message back\n");
		return -1;
	}

	nr++;
	return buffer_size;
}
