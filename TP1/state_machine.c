#include "message.h"
#include "state_machine.h"
#include "protocol.h"

state_machine current_state = START;

state_machine getStateMachine(){
	return current_state;
}

void handleState(char * msg){

    state_machine state_machine = getStateMachine();

    char addr,ctrl;
    switch (state_machine)
    {
    case START:
        handleStartState(msg);
        break;
    case FLAG_RCV:
        handleFlagReceived(msg);
        break;
    case A_RCV:
        handleAddrReceived(msg);
        addr = *msg;
        break;
    case C_RCV:
        handleCtrlState(msg,addr,ctrl);
        ctrl = *msg;
        break;
    case BCC_OK:
        handleBccState(msg);
        break;
    case STOP:

        break;
      
    default:
        break;
    }
}

void handleStartState(char * msg){
    switch (*msg) {
        case F:
            update_state(FLAG_RCV);
            break;
        default:
            break;
    }
}
void handleFlagReceived(char * msg) {
    switch (*msg) {
        case F:
            break;
        case AREC:
        case AEM:
            update_state(A_RCV);
            //state_machine.addr = msg;
            break;
        default:
            update_state(START);
            break;
    }
}


void handleAddrReceived(char * msg) {
    switch (*msg) {
        case F:
            update_state(FLAG_RCV);
            break;
        case DISC:
        case SET: 
        case UA:
        default:
            update_state(START);
            break;    
    }
}

void handleCtrlState(char * msg,char addr,char ctrl){
    switch (*msg)
    {
    case F:
        update_state(START);
        break;
    default:
        if(*msg == addr ^ctrl){
            update_state(BCC_OK);
        } else update_state(START);
        break;
    }

}

void handleBccState(char * msg) {
    switch (*msg) {
        case F:
            update_state(STOP);
            break;
        default:
            update_state(START);
            break;
    }
}



void update_state(state_machine state){
    if(state == START){

    }
}
