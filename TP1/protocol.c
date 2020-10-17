#include "protocol.h"
#include "connection.h"
#include "message.h"

int llopen(char* port, connection_type connection_type) {
	link_layer layer;
	strcpy(layer.port,port);
	layer.baud_rate = BAUDRATE;
  	layer.num_transmissions = NUM_TRANSMISSIONS;
  	layer.timeout = LAYER_TIMEOUT;
	  
	int fd = open_connection(layer);
	if(connection_type == EMISSOR){
		printf("Writing SET message\n");

		if(!write_supervision_message_retry(fd,SET)){
			printf("Error establishing connection\n");
		}	
	}else if (connection_type == RECEPTOR){
		if(!readSupervisionMessage(fd)) printf("Error reading SET message\n");

		//if(llwrite(fd, trama, check)==-1) printf("Error wrting message\n");
		if(write_supervision_message(fd,UA) == -1){
			printf("Error writing UA\n");
		}
	}
	return fd;
}

int llclose(int fd) {
    close_connection(fd);
}

int llwrite(int fd, char* buffer, int length) {
	int wr = write(fd, buffer, length);

	if (wr != length){
		return -1;
	}
	return wr;
}

int llread(int fd, char* buffer) {
	char r[2];
	int finished = FALSE, rd, pos = 0;
	while (!finished){
		rd = read(fd, r, 1);
		if (rd <= 0){
			return -1;
		}
		else if (r[0] == '\0'){
			finished = TRUE;
		}
		buffer[pos++] = r[0];
	}
	return pos;
}