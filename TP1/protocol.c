#include "protocol.h"
#include "connection.h"
#include "message.h"
#include "state_machine.h"

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

		if(!write_supervision_message_retry(fd,SET)){
			printf("Error establishing connection\n");
		}
	}else if (connection_type == RECEPTOR){
		if(!readSupervisionMessage(fd)) printf("Error reading SET message\n");
		if(write_supervision_message(fd,UA) == -1){
			printf("Error writing UA\n");
		}
	}
	return fd;
}

int llclose(int fd) {
	if(connection == EMISSOR){
		printf("Writing DISC message\n");

		if(!write_supervision_message_retry(fd,DISC)){
			printf("Error establishing connection\n");
		}
		if(write_supervision_message(fd,UA) == -1){
			printf("Error writing UA\n");
		}
	}else if (connection == RECEPTOR){
		if(!readSupervisionMessage(fd)) printf("Error reading DISC message\n");
		if(write_supervision_message(fd,DISC) == -1){
			printf("Error writing UA\n");
		}
		if(!readSupervisionMessage(fd)) printf("Error reading UA message\n");
	}

    return close_connection(fd);
}

int llwrite(int fd, char* buffer, int length) {

	int wr = write_info_message(fd,buffer,length,1);

	if (wr != length){
		return -1;
	}
	return wr;
}

int llread(int fd, char* buffer) {
	return 0;
}