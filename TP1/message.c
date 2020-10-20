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
	char * trama = malloc(INFO_SIZE_MSG(data_size));
    trama[FLAGI_POSTION] = F;
	trama[ADRESS_POSITION] = AREC;
	trama[CC_POSITION] = cc_value; //0S00000
	trama[PC_POSITION] = F;//???? 

	memcpy(trama + DATA_INF_BYTE,data,data_size);

	return write(fd,trama,INFO_SIZE_MSG(data_size));
}

int write_supervision_message_retry(int fd,char cc_value){
	char received[SUPERVISION_TRAMA_SIZE];

	memset(received, 0, strlen(received));

	int rd;
	int success = FALSE;

	while(conta <= WRITE_NUM_TRIES && !success){
		if(flag){
			alarm(RESEND_DELAY);
			flag=0;

			/* writing */ 
			if(write_supervision_message(fd,cc_value)==-1){
				printf("Error writing supervision message\n");
			}else success = TRUE;

			/* read message back */
			rd = llread(fd, received);
			if (rd != sizeof(received)) 
				success = FALSE;
			else success = TRUE;
		}
	}
	if (success == TRUE){
		printSupervisionMessage(received);
		return 1;
	}
	return 0;
}

int readSupervisionMessage(int fd){
	char trama[SUPERVISION_TRAMA_SIZE];
	int check = llread(fd, trama);

	if(check==-1){
		printf("Error reading message\n");
		return 0;
	} 
	if(check != sizeof(trama)){
		return 0;
	}
	printSupervisionMessage(trama);	
	return 1;

}

void printSupervisionMessage(char * trama){
	printf("Message recieved correctly\n");
	printf("F: %04x\nA: %04x\n", trama[0], trama[1]);
	printf("C: %04x\n", trama[2]);
	printf("BCC: %04x\n", trama[3]);
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

//TODO desactivar o alarme