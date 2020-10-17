#include "message.h"
#include "protocol.h"

int flag=1, conta=1;



int write_supervision_message(int fd,int cc_value){
    char trama[5];
    trama[FLAGI_POSTION] = F;
	trama[ADRESS_POSITION] = AREC;
	trama[CC_POSITION] = cc_value;
	trama[PC_POSITION] = F;//???? 
	trama[FLAGF_POSTION] = '\0'; //TODO Mudar para FLAG depois de implementar a maquina de estados

    return llwrite(fd,trama,MESSAGE_LENGTH);
}

int write_supervision_message_retry(int fd,int cc_value){
	char received[5];

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
			
			if (rd != sizeof(received)) success = FALSE;
			else{
				success = TRUE;
			} 
		}
	}
	if (success == TRUE){
		printf("CHGUEI AQUI\n");
		/* verification */
		printf("F1: %04x  F2: %04x\n", received[0], received[1]);
		printf("C: %04x\n", received[2]);
		printf("A: %04x\n", received[3]);
		return 1;
	}
	return 0;
}

int readSupervisionMessage(int fd){
	char trama[5];
	int check = llread(fd, trama);
	if(check==-1){
		printf("Error reading message\n");
		return 0;
	} 
	printSupervisionMessage(trama);	
	return 1;

}

void printSupervisionMessage(char * trama){
	printf("Message recieved correctly\n");
	printf("F1: %04x  F2: %04x\n", trama[0], trama[1]);
	printf("C: %04x\n", trama[2]);
	printf("A: %04x\n", trama[3]);
}

void atende(int signo)
{
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

