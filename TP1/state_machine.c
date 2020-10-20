#include "message.h"
#include "state_machine.h"
#include "protocol.h"

state_machine current_state = START;

state_machine getStateMachine(){
	return current_state;
}

char addr, ctrl;
int dataCounter = 0;
char bcc2 = 0xF;

void handleState(char msg,int dataSize){

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
            handleCtrlState(msg,addr,ctrl,0);
            break;
        case BCC_OK:
            handleBccState(msg);
            break;
        case BCC1_INF:
            handleBccStateInf1(msg);
            break;
        case DATA_INF:
            handleDataState(msg,dataSize);
            break;
        case BCC2_INF:
            handleBccState(msg);
            break;
        case STOP:
            update_state(START);
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
            update_state(BCC1_INF);
            break;
        default:
            update_state(START);
            break;    
    }
}

void handleCtrlState(char msg, char addr, char ctrl,int inform){
    switch (msg){
		case F:
            update_state(FLAG_RCV);
            break;
        default:
            if(msg == (addr) ^ (ctrl)){
                if(inform) update_state(BCC1_INF);
                else update_state(BCC_OK);
            } else update_state(START);
            break;
    }

}


//1
//dataSize Numero de bytes dentro de uma trama de informaçao
void handleDataState(char msg,int dataSize){
    switch (msg)
    {
    case F:
        update_state(STOP);
        break;
    default:
        dataCounter++;
        if(dataCounter <= dataSize) bcc2 = msg ^ bcc2;
        if(dataCounter == (dataSize+1)){
            if(msg == bcc2) update_state(BCC2_INF);
        }

        break;
    }
}


void handleBccState(char msg) {
    switch (msg) {
        case F:
            update_state(STOP);
            break;
        default:
            update_state(START);
            break;
    }
}

void handleBccStateInf1(char msg) {
    switch (msg) {
        case F:
            update_state(FLAG_RCV);
            break;
        default:
            update_state(DATA_INF);
            break;
    }
}


void update_state(state_machine state){
    current_state = state;
}
