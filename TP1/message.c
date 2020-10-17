#include "message.h"
#include "protocol.h"

int flag=1, conta=1;

int write_set_message(int fd){
    char trama[5];
    trama[FLAGI_POSTION] = F;
	trama[ADRESS_POSITION] = AREC;
	trama[CC_POSITION] = SET;
	trama[PC_POSITION] = F;//???? 
	trama[FLAGF_POSTION] = '\0'; //TODO Mudar para FLAG depois de implementar a maquina de estados

    return llwrite(fd,trama,MESSAGE_LENGTH);
}

void write_set_retry(int fd){
	char received[5];

	memset(received, 0, strlen(received));

	int rd;
	int success = FALSE;

	while(conta <= WRITE_NUM_TRIES && !success){
		if(flag){
			alarm(RESEND_DELAY);
			flag=0;

			/* writing */ 
			write_set_message(fd);

			/* read message back */
			rd = llread(fd, received);
			
			if (rd != sizeof(received)) success = FALSE;
			else{
				success = TRUE;
				printf("Connection established correctly\n");
			} 
		}
	}
	if(conta==4) printf("Error establishing connection\n");
	if (success == TRUE){
		/* verification */
		printf("F1: %04x  F2: %04x\n", received[0], received[1]);
		printf("C: %04x\n", received[2]);
		printf("A: %04x\n", received[3]);
	}
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

