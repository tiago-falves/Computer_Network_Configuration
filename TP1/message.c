#include "message.h"
#include "protocol.h"
#include "state_machine.h"

int flag=1, conta=1;

int write_supervision_message(int fd, char cc_value){
    char trama[SUPERVISION_TRAMA_SIZE];
    trama[FLAGI_POSTION] = F;
	trama[ADRESS_POSITION] = AREC;
	trama[CC_POSITION] = cc_value;
	trama[PC_POSITION] = (AREC) ^ (cc_value);
	trama[FLAGF_POSTION] = F;
	trama[SUPERVISION_TRAMA_SIZE - 1] = '\0';

    return write(fd,trama,SUPERVISION_TRAMA_SIZE);
}

int write_info_message(int fd,char * data,int data_size, char cc_value){
	if(data_size==0) return -1;

	int trama_size = INFO_SIZE_MSG(data_size);

	char* trama = malloc(trama_size + 1);

    trama[FLAGI_POSTION] = F;
	trama[ADRESS_POSITION] = AREC;
	trama[CC_POSITION] = CC_INFO_MSG(cc_value); //0S00000
	trama[PC_POSITION] = (AREC) ^ CC_INFO_MSG(cc_value);
	trama[trama_size - 2] = buildBCC2(data, data_size);
	trama[trama_size - 1] = F;
	trama[trama_size] = '\0';

	memcpy(trama + DATA_INF_BYTE,data,data_size);

	return write(fd,trama,INFO_SIZE_MSG(data_size));
}

int write_supervision_message_retry(int fd,char cc_value){
	int success = FALSE, rd;
	char* buffer;

	while(conta <= WRITE_NUM_TRIES && !success){
		if(flag){
			alarm(RESEND_DELAY);
			flag=0;

			/* writing */ 
			int n_bytes = write_supervision_message(fd,cc_value);
			if(n_bytes == -1){
				printf("Error writing supervision message\n");
			}

			/* read message back */
			buffer = readMessage(fd, &rd);
			if (rd != n_bytes || buffer == NULL) 
				success = FALSE;
			else{
				success = TRUE;
				reset_alarm();
			} 
		}
	}
	if (success == TRUE){
		printSupervisionMessage(buffer);
		return 0;
	}
	return -1;
}

int write_inform_message_retry(int fd, char cc_value, int dataSize, char * data){
	int success = FALSE, rd;
	char* buffer;

	while(conta <= WRITE_NUM_TRIES && !success){
		if(flag){
			alarm(RESEND_DELAY);
			flag=0;

			/* writing */
			int n_bytes = write_info_message(fd, data, dataSize, cc_value); 
			if(n_bytes == -1){
				printf("Error writing Inform message\n");
			}

			/* read message back */
			buffer = readMessage(fd, &rd);
			if (rd != n_bytes || buffer == NULL) 
				success = FALSE;
			else{
				success = TRUE;
				reset_alarm();
			} 
		}
	}

	if (success == TRUE){
		printInformMessage(buffer ,dataSize);
		return 0;
	}
	return -1;
}


int readSupervisionMessage(int fd){
	int trama_size;
	char* trama = readMessage(fd, &trama_size);

	if(trama_size==-1){
		printf("Error reading message\n");
		return -1;
	} 
	if(trama_size != SUPERVISION_TRAMA_SIZE){
		return -1;
	}
	printSupervisionMessage(trama);	
	return 0;
}

void printSupervisionMessage(char * trama){
	printf("Supervision message recieved correctly\n");
	printf("F: %04x\nA: %04x\n", trama[0], trama[1]);
	printf("C: %04x\n", trama[2]);
	printf("BCC: %04x\n", trama[3]);
}

void printInformMessage(char * trama,int dataSize){
	printf("Inform message recieved correctly\n");
	printf("F: %04x\nA: %04x\n", trama[0], trama[1]);
	printf("C: %04x\n", trama[2]);
	printf("BCC: %04x\n", trama[3]);
	printf("Data: \n");
	for (size_t i = 4; i < dataSize+4; i++)
		printf("%c", trama[i]);
	printf("\n");
	printf("BCC2: %04x\n", trama[dataSize+5]);
	printf("F: %04x\n", trama[dataSize+6]);
}

void atende(int signo){
	printf("alarme # %d\n", conta);
	flag=1;
	conta++;
}

void install_alarm(){
	struct sigaction action;
    action.sa_handler = atende;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGALRM,&action,NULL) < 0){
        fprintf(stderr,"Unable to install SIGALRM handler\n");
        exit(1);
    }
}

void reset_alarm(){
	flag = 1;
	conta = 1;
	alarm(0);
}

char* readMessage(int fd, int* size){  
	char r;
	int finished = FALSE, rd, pos = 0;
	char* buffer = malloc(1);

	while (!finished){
		//printf("State machine: %d \n", getStateMachine());
		rd = read(fd, &r, 1);
		if (rd <= 0){
			return 0;
		}
		else if (getStateMachine() == STOP){
			finished = TRUE;
		}

		buffer = realloc(buffer, pos + 2);
		handleState(r,0);
		buffer[pos++] = r;
	}
	*size = pos;
	return buffer;
}

char buildBCC2(char * data, int data_size) {
	char xor = data[0] ^ data[1];
	for (int i = 2; i < data_size; i++) {
		xor = xor ^ data[i];
	}
	return xor;
}
