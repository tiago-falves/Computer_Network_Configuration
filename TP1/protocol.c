#include "protocol.h"
#include "connection.h"
#include "message.h"
#include "state_machine.h"

conn_type connection;

int llopen(char* port, conn_type connection_type) {
	link_layer layer;
	strcpy(layer.port,port);
	layer.baud_rate = BAUDRATE;
  	layer.num_transmissions = NUM_TRANSMISSIONS;
  	layer.timeout = LAYER_TIMEOUT;
	connection = connection_type;
	  
	int fd = open_connection(layer);
	if(connection_type == EMISSOR){
		printf("Writing SET message\n");

		if(write_supervision_message_retry(fd,SET) == -1){
			printf("Error establishing connection\n");
		}
	}else if (connection_type == RECEPTOR){
		if(readSupervisionMessage(fd) == -1){
			printf("Error reading SET message\n");
		}
		if(write_supervision_message(fd,UA) == -1){
			printf("Error writing UA\n");
		}
	}
	return fd;
}

int llclose(int fd) {
	if(connection == EMISSOR){
		printf("Writing DISC message\n");

		if(write_supervision_message_retry(fd,DISC) == -1){
			printf("Error establishing connection\n");
		}
		if(write_supervision_message(fd,UA) == -1){
			printf("Error writing UA\n");
		}
	}

    return close_connection(fd);
}

int llwrite(int fd, char* data, int data_size) {
	return write_inform_message_retry(fd, 1, data_size, data);
}

int llread(int fd, char* buffer) {
	int buffer_size = 0;

	while (TRUE) {
		buffer = readMessage(fd, &buffer_size, 1);

		if (buffer == NULL || buffer_size == 0){
			printf("LLREAD: error reading message\n");
			break;
		}


		if (buffer[CTRL_POS] == DISC) {
			if (write_supervision_message(fd, DISC) == -1){
				printf("LLREAD: error writing DISC message back\n");
				return -1;
			}
			break;
		}

		if (write_supervision_message(fd, RR(1)) == -1){
			printf("LLREAD: error writing message back\n");
			return -1;
		}

		printDataInfoMsg(buffer,buffer_size);
	

		
	}

	buffer = readMessage(fd, &buffer_size, 1);
	if (buffer == NULL || buffer_size == 0){
		printf("LLREAD: error reading UA message after sending DISC\n");
		return -1;
	}

	if (buffer[CTRL_POS] == UA) {
		return 0;
	}
	else {
		printf("LLREAD: wrong message after sending DISC\n");
		return -1;
	}

	free(buffer);
	return 0;
}