#include "message.h"
#include "protocol.h"
#include "state_machine.h"
#include "data_stuffing.h"

int flag=1, conta=1;
static int ns = 0;

int write_supervision_message(int fd, char cc_value){
    char trama[SUPERVISION_TRAMA_SIZE];
    trama[FLAGI_POSTION] = FLAG;
	trama[ADRESS_POSITION] = AREC;
	trama[CC_POSITION] = cc_value;
	trama[PC_POSITION] = (AREC) ^ (cc_value);
	trama[FLAGF_POSTION] = FLAG;

    return write(fd,trama,SUPERVISION_TRAMA_SIZE);
}


int write_info_message(int fd, char* data, int data_size, int cc_value){
	if(data_size==0) {
		printf("Recceived empty data buffer\n");
		return -1;
	} 

	data_stuff stuffedData = stuffData(data,data_size);

	int trama_size = INFO_SIZE_MSG(stuffedData.data_size);
	char* trama = malloc(trama_size + 1); //space in case bcc2 needs stuffing

    trama[FLAGI_POSTION] = FLAG;
	trama[ADRESS_POSITION] = AREC;
	trama[CC_POSITION] = CC_INFO_MSG(cc_value);
	trama[PC_POSITION] = (AREC) ^ CC_INFO_MSG(cc_value);
	
	char bcc2 = buildBCC2(data, data_size);
	if (bcc2 == FLAG || bcc2 == ESC) {
		trama_size++;
		trama[trama_size - 3] = ESC;
		trama[trama_size - 2] = bcc2 ^ STUFF;
		trama[trama_size - 1] = FLAG;
	}
	else {
		trama[trama_size - 2] = bcc2;
		trama[trama_size - 1] = FLAG;
	}

	memcpy(trama + DATA_INF_BYTE, stuffedData.data, stuffedData.data_size);

	return write(fd,trama,trama_size);
}

int write_supervision_message_retry(int fd, char cc_value){
	int success = FALSE, rd;
	char* buffer;

	while(conta <= WRITE_NUM_TRIES && !success){
		if(flag){
			alarm(RESEND_DELAY);
			flag=0;

			/* writing */ 
			int n_bytes = write_supervision_message(fd,cc_value);
			if(n_bytes <= 0){
				printf("Error writing supervision message\n");
			}

			/* read message back */
			buffer = readMessage(fd, &rd, 0, 1);

			if (rd != n_bytes || buffer == NULL) 
				success = FALSE;
			else if (cc_value == SET && buffer[CTRL_POS] == UA){
				success = TRUE;
				reset_alarm();
			}
			else if (cc_value == DISC && buffer[CTRL_POS] == DISC){
				success = TRUE;
				reset_alarm();
			} 
		}
	}
	if (success == TRUE){
		return 0;
	}
	printf("Finishing attempt after %d tries have received time out of %d seconds.\n", WRITE_NUM_TRIES, RESEND_DELAY);
	return -1;
}

int write_inform_message_retry(int fd, char * data, int dataSize){
	int success = FALSE, rd;
	char* buffer;

	while(conta <= WRITE_NUM_TRIES && !success){
		if(flag){
			alarm(RESEND_DELAY);
			flag=0;

			/* writing */
			int n_bytes = write_info_message(fd, data, dataSize, ns); 
			if(n_bytes <= 0){
				printf("Error writing Inform message\n");
			}

			/* read message back */
			buffer = readMessage(fd, &rd, 0, 1);

			if (buffer == NULL || parseARQ(buffer)){
				success = FALSE;
			}
			else{
				ns++;
				success = TRUE;
				reset_alarm();
			}
		}
	}

	if (success == TRUE){
		return 0;
	}
	return -1;
}

char* readMessage(int fd, int* size, int i_message, int emissor){  
	char r;
	int rd, pos = 0, nulls_count = 0;
	char* buffer = malloc(1);

	while (getStateMachine() != STOP){
		rd = read(fd, &r, 1);
		if (rd <= 0){
			nulls_count++;
			if (nulls_count == ERR_LIMIT || emissor)
				return NULL;
			else
				continue;
		}

		buffer = realloc(buffer, pos + 2);
		handleState(r, i_message);
		buffer[pos++] = r;
	}

	update_state(START);
	*size = pos;
	
	return buffer;
}

void printSupervisionMessage(char * trama, int onlyC){
	if (onlyC){
		printf("Supervision message C: %04x recieved correctly\n", trama[2]);
	}else {
		printf("Supervision message recieved correctly\n");
		printf("FLAG: %04x\nA: %04x\n", trama[0], trama[1]);
		printf("C: %04x\n", trama[2]);
		printf("BCC: %04x\n", trama[3]);
	}
	
}

void printInformMessage(char * trama, int dataSize, int data){
	if(!data){
		printf("Inform message recieved correctly\n");
		printf("FLAG: %04x\nA: %04x\n", trama[0], trama[1]);
		printf("C: %04x\n", trama[2]);
		printf("BCC: %04x\n\n", trama[3]);
		printf("Data: ");
	}
	for (size_t i = DATA_INF_BYTE; i < dataSize+DATA_INF_BYTE; i++)
		printf("%c", trama[i]);
	printf("\n\n");
	if(!data){
		printf("\n");
		printf("BCC2: %04x\n", trama[dataSize+DATA_INF_BYTE+1]);
		printf("FLAG: %04x\n", trama[dataSize+DATA_INF_BYTE+2]);
	}
}

void printDataInfoMsg(char * trama,int trama_size){
	for (int i = 7; i < trama_size-4; i++)
		printf("%c", trama[i]);	
}

void atende(int signo){
	//printf("alarme # %d\n", conta);
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

char buildBCC2(char * data, int data_size) {
	char xor = data[0] ^ data[1];
	for (int i = 2; i < data_size; i++) {
		xor = xor ^ data[i];
	}
	return xor;
}
int verifyBCC(char * inform, int infMsgSize, char * data, int dataSize){
	char bcc = buildBCC2(data,dataSize);

	if(inform[infMsgSize-2] == bcc) return 0;
	else return -1; 
}

int parseARQ(char* buffer) {
	unsigned char arq = (unsigned char) buffer[CTRL_POS];
	if (arq == REJ(0) || arq == REJ(1))
		return 1;
	return 0;
}

int getSequenceNumber(char* buffer) {
	return (int) (buffer[CTRL_POS] >> 6);
}
