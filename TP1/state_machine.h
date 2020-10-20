typedef enum {
  START = 0,
  FLAG_RCV = 1,
  A_RCV = 2,
  C_RCV = 3,
  BCC_OK = 4,
  BCC1_INF = 5,
  DATA_INF = 6,
  BCC2_INF = 7,
  STOP = 8
} state_machine;


void handleState(char msg,int dataSize);
void handleStartState(char msg);
void handleFlagReceived(char msg);
void handleAddrReceived(char msg);
void handleCtrlState(char msg,char addr,char ctrl,int inform);
void handleBccState(char msg);
void handleBccStateInf1(char msg);
void handleDataState(char msg,int dataSize);

void update_state(state_machine state);
state_machine getStateMachine();
