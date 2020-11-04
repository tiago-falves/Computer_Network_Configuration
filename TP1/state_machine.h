typedef enum {
  START = 0,
  FLAG_RCV = 1,
  A_RCV = 2,
  C_RCV = 3,
  BCC1_OK = 4,
  DATA_INF = 5,
  STOP = 6
} state_machine;


void handleState(char msg, int i_message, int* error);
void handleStartState(char msg, int* error);
void handleFlagReceived(char msg);
void handleAddrReceived(unsigned char msg);
void handleCtrlState(char msg,char addr,char ctrl);
void handleBcc1State(char msg, int i_message);
void handleDataState(char msg);
void handleStopState(char msg);

void update_state(state_machine state);
state_machine getStateMachine();
