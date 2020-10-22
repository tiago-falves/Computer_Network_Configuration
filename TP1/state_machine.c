#include "message.h"
#include "state_machine.h"
#include "protocol.h"

state_machine current_state = START;

state_machine getStateMachine(){
	return current_state;
}

char addr, ctrl;
char bcc2 = 0x0;

void handleState(char msg, int i_message){
    state_machine state_machine = getStateMachine();

    switch (state_machine){
        case START:
            handleStartState(msg);
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
            handleCtrlState(msg,addr,ctrl);
            break;
        case BCC1_OK:
            handleBcc1State(msg, i_message);
            break;
        case DATA_INF:
            handleDataState(msg);
            break;
        case DATA_FINISHED:
            handleDataFinishedState(msg);
            break;
        case BCC2_OK:
            handleBcc2State(msg);
            break;
        case STOP:
            handleStopState(msg);
            break;
        
        default:
            break;
    }
}

void handleStartState(char msg){
    switch (msg) {
        case F:
            update_state(FLAG_RCV);
            break;
        default:
            break;
    }
}
void handleFlagReceived(char msg) {
    switch (msg) {
        case F:
            break;
        case AREC: case AEM:
            update_state(A_RCV);
            break;
        default:
            update_state(START);
            break;
    }
}


void handleAddrReceived(char msg) {
    switch (msg) {
        case F:
            update_state(FLAG_RCV);
            break;
        case DISC: case SET: case UA:
            update_state(C_RCV);
			break;
        case CC_INFO_MSG(0): case CC_INFO_MSG(1):
            update_state(C_RCV);
            break;
        default:
            update_state(START);
            break;    
    }
}

void handleCtrlState(char msg, char addr, char ctrl){
    switch (msg){
		case F:
            update_state(FLAG_RCV);
            break;
        default:
            if(msg == (addr) ^ (ctrl))
                update_state(BCC1_OK);
            else 
                update_state(START);
            break;
    }

}

void handleBcc1State(char msg, int i_message) {
    switch (msg) {
        case F:
            update_state(STOP);
            break;
        default:
            if (i_message){
                update_state(DATA_INF);
                bcc2 = msg ^ bcc2;
            }
            else
                update_state(START);
            break;
    }
}

void handleDataState(char msg){
    switch (msg){
        case F:
            update_state(FLAG_RCV);
            break;
        case '\0':
            update_state(DATA_FINISHED);
            break;
        default:
            bcc2 = msg ^ bcc2;
            break;
    }
}

void handleDataFinishedState(char msg){
    switch (msg){
        case F:
            update_state(FLAG_RCV);
            break;
        default:
            if(msg == bcc2) 
                update_state(BCC2_OK);
            else
                update_state(START);
            break;
    }
}

void handleBcc2State(char msg) {
    switch (msg) {
        case F:
            update_state(STOP);
            break;
        default:
            update_state(START);
            break;
    }
}

void handleStopState(char msg) {
    switch (msg) {
        case F:
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
