#include "protocol.h"
#include "connection.h"
#include "message.h"
#include "state_machine.h"
#include "data_stuffing.h"

connection_type connection;

int llopen(char* port, connection_type connection_type) {
	link_layer layer;
	strcpy(layer.port,port);
	layer.baud_rate = BAUDRATE;
  	layer.num_transmissions = NUM_TRANSMISSIONS;
  	layer.timeout = LAYER_TIMEOUT;
	state_machine current_state	= getStateMachine();
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
	}else if (connection == RECEPTOR){
		if(readSupervisionMessage(fd) == -1){ 
			printf("Error reading DISC message\n");
		}
		if(write_supervision_message(fd,DISC) == -1){
			printf("Error writing UA\n");
		}
		if(readSupervisionMessage(fd) == -1){
			printf("Error reading UA message\n");
		}
	}

    return close_connection(fd);
}

int llwrite(int fd, char* buffer, int length) {
	//divide buffer and determine number of information tramas
	int num_iterations = 0;

	data_stuff stuffedData = stuffData(buffer,length);
	
	//int wr = write_inform_message_retry(fd, 1 ,length, buffer);
	int wr = write_inform_message_retry(fd, 1 ,stuffedData.data_size, stuffedData.data);

	if (wr != length){
		return -1;
	}
	return wr;
}

int llread(int fd, char* buffer) {
	int buffer_size = 0, r = 1;

	buffer = readMessage(fd, &buffer_size, 1);
	if (buffer == NULL || buffer_size == 0){
		printf("LLREAD: error reading message\n");
	}

	if (write_supervision_message(fd, RR(1)) == -1){
		printf("LLREAD: error writing message back\n");
	}

	return 0;
}