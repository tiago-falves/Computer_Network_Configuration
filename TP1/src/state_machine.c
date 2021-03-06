#include "message.h"
#include "state_machine.h"
#include "protocol.h"

state_machine current_state = START;

state_machine getStateMachine(){
	return current_state;
}

char addr, ctrl;
int inf_bytes = 0;

void handleState(char msg, int i_message, int* error, int emissor){
    state_machine state_machine = getStateMachine();

    switch (state_machine){
        case START:
            handleStartState(msg, error);
            break;
        case FLAG_RCV:
            handleFlagReceived(msg);
            addr = msg;
            break;
        case A_RCV:
            handleAddrReceived(msg);
            ctrl = msg;
            break;
        case C_RCV:
            handleCtrlState(msg,addr,ctrl,emissor);
            break;
        case BCC1_OK:
            handleBcc1State(msg, i_message);
            break;
        case DATA_INF:
            handleDataState(msg);
            break;
        case STOP:
            handleStopState(msg);
            break;
        
        default:
            break;
    }
}

void handleStartState(char msg, int* error){
    if (msg == FLAG && *error){
        *error = 0;
    }else if (msg == FLAG){
        update_state(FLAG_RCV);
    }
}
void handleFlagReceived(char msg) {
    switch (msg) {
        case FLAG:
            break;
        case AREC: case AEM:
            update_state(A_RCV);
            break;
        default:
            update_state(START);
            break;
    }
}

void handleAddrReceived(unsigned char msg) {
    switch (msg) {
        case FLAG:
            update_state(FLAG_RCV);
            break;
        case DISC: case SET: case UA:
            update_state(C_RCV);
			break;
        case CC_INFO_MSG(0): case CC_INFO_MSG(1):
            update_state(C_RCV);
            break;
        case RR(0):
            update_state(C_RCV);
            break;
        case REJ(0): case REJ(1):
            update_state(C_RCV);
            break;
        default:
            update_state(START);
            break;    
    }
}

void handleCtrlState(char msg, char addr, char ctrl, int emissor){
    switch (msg){
		case FLAG:
            update_state(FLAG_RCV);
            break;
        default:
            if (!emissor && (ctrl == CC_INFO_MSG(0) || ctrl == CC_INFO_MSG(1)))
                errorsBCC1(&addr, &ctrl);
            if(msg == (addr ^ ctrl))
                update_state(BCC1_OK);
            else{
                update_state(START);
                if (!emissor) printf("BCC1 error\n");
            }
            break;
    }

}

void handleBcc1State(char msg, int i_message) {
    switch (msg) {
        case FLAG:
            update_state(STOP);
            break;
        default:
            if (i_message)
                update_state(DATA_INF);
            else
                update_state(START);
            break;
    }
}

void handleDataState(char msg){
    switch (msg){
        case FLAG:
            update_state(STOP);
            break;
        default:
            break;
    }
}


void handleStopState(char msg) {
    switch (msg) {
        case FLAG:
            update_state(FLAG_RCV);
            break;
        default:
            update_state(START);
            break;
    }
}

void update_state(state_machine state){
    current_state = state;
}
