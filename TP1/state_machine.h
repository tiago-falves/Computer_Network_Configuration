typedef enum {
  START = 0,
  FLAG_RCV = 1,
  A_RCV = 2,
  C_RCV = 3,
  BCC_OK = 4,
  STOP = 5
} state_machine;


void handleState(char * msg);
void handleStartState(char * msg);
void handleFlagReceived(char * msg);
void handleAddrReceived(char * msg);
void handleCtrlState(char * msg,char addr,char ctrl);
void handleBccState(char * msg);

void update_state(state_machine state);
state_machine getStateMachine();
