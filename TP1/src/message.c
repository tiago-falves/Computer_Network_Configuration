#include "message.h"
#include "protocol.h"
#include "state_machine.h"
#include "data_stuffing.h"

int flag=1, conta=1;
static int ns = 0;
static int block_size = 0;

void setBlockSize(int value) {
	block_size = value;
}

int write_supervision_message(int fd, char cc_value){
	//CUT CABLE ON RECEPTOR ANSWER SIMULATION
	/*if (cc_value == RR(0) || cc_value == RR(1) || cc_value == REJ(0) || cc_value == REJ(1)){
		int r = rand() % 100;
		if (r < 5){
			char trama[SUPERVISION_TRAMA_SIZE-2];
			trama[FLAGI_POSTION] = FLAG;
			trama[ADRESS_POSITION] = AREC;
			trama[CC_POSITION] = cc_value;

			return write(fd,trama,SUPERVISION_TRAMA_SIZE-2);
		}
	}*/

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
	}else {
		trama[trama_size - 2] = bcc2;
	}

	trama[trama_size - 1] = FLAG;

	//FLAG NOISE SIMULATION
	/*int r = rand() % 100;
	if (r < 5){
		//REMOVE FIRST FLAG
		trama[FLAGI_POSTION] = 0x14;
		//REMOVE END FLAG
		//trama[trama_size - 1] = 0x14;
		printf("Changed 0xfe\n");
	}*/

	memcpy(trama + DATA_INF_BYTE, stuffedData.data, stuffedData.data_size);

	return write(fd,trama,trama_size);
}

int write_supervision_message_retry(int fd, char cc_value, char cc_compare){
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

			if (rd != n_bytes || buffer == NULL){
				success = FALSE;
			}
			else if (buffer[CTRL_POS] == cc_compare){
				success = TRUE;
				reset_alarm();
			}
			/*else if (cc_value == DISC && buffer[CTRL_POS] == DISC){
				success = TRUE;
				reset_alarm();
			}*/
			else{
				success = FALSE;
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
	int success = FALSE, rd = 0;
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

			if (buffer == NULL){
				//printf("Resending...\n");
				success = FALSE;
			}
			else if (parseREJ(buffer)){
				//printf("REJ received\n");
				success = FALSE;
				reset_alarm();
			}
			else if (parseRR(buffer)){
				ns++;
				success = TRUE;
				reset_alarm();
			}
			else{
				success = FALSE;
			}
		}
	}

	if (success == TRUE){
		return 0;
	}
	printf("Finishing attempt after %d tries have received time out of %d seconds.\n", WRITE_NUM_TRIES, RESEND_DELAY);
	return -1;
}

char* readMessage(int fd, int* size, int i_message, int emissor){  
	char r;
	int rd = 0, pos = 0, nulls_count = 0, error = 0;
	char* buffer = malloc(1);

	while (getStateMachine() != STOP){
		rd = read(fd, &r, 1);
		if (rd <= 0){
			nulls_count++;
			if (nulls_count == ERR_LIMIT || emissor)
				return NULL;
			else
				continue;
			update_state(START);
		}

		buffer = realloc(buffer, pos + 2);
		handleState(r, i_message, &error/*, emissor*/);
		buffer[pos++] = r;

		//printf("Read char = %02x and advanced to state = %d\n", r, getStateMachine());
		
		if (getStateMachine() == START){
			free(buffer);
			buffer = malloc(1);
			pos = 0;
			if (r != FLAG) error = 1;
		}
	}

	update_state(START);
	*size = pos;
	
	return buffer;
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

int parseREJ(char* buffer) {
	unsigned char arq = (unsigned char) buffer[CTRL_POS];
	if (arq == REJ(0) || arq == REJ(1))
		return 1;
	return 0;
}

int parseRR(char* buffer) {
	unsigned char arq = (unsigned char) buffer[CTRL_POS];
	if (arq == RR(0) || arq == RR(1))
		return 1;
	return 0;
}

int getSequenceNumber(char* buffer) {
	return (int) (buffer[CTRL_POS] >> 6);
}
