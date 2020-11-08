typedef enum {
  START = 0,
  FLAG_RCV = 1,
  A_RCV = 2,
  C_RCV = 3,
  BCC1_OK = 4,
  DATA_INF = 5,
  STOP = 6
} state_machine;

/**
 * @brief Recognizes state machine current state and updates it according to new char msg received.
 * 
 */
void handleState(char msg, int i_message, int* error, int emissor);

/**
 * @brief Handles start state, advances to state FLAG_RCV if a char of value FLAG is received and no errors were verified. If there were previous errors and a byte of value FLAG was received errors value is updated to 0.
 * 
 * @param msg Byte received.
 * @param error Error indicator variable.
 */
void handleStartState(char msg, int* error);

/**
 * @brief Hadles FLAG_RCV state, advances if a valid adress field value is received.
 * 
 * @param msg Byte received.
 */
void handleFlagReceived(char msg);

/**
 * @brief Handles A_RCV state, advances if a valid control field is received.
 * 
 * @param msg Byte received.
 */
void handleAddrReceived(unsigned char msg);

/**
 * @brief Handles C_RCV state, advances if BCC1 is verified.
 * 
 * @param msg Byte received.
 * @param addr Address field to calculate BCC1.
 * @param ctrl Control field to calculate BCC1.
 * @param emissor 1 if state machine is reading messages that are being received by the emitter, for log reasons (in order not to mess progress bar display).
 */
void handleCtrlState(char msg,char addr,char ctrl,int emissor);

/**
 * @brief Handles BCC1_OK state, if i_message is verified advances to DATA_INF state, otherwise if a FLAG is received updates state machine to STOP state.
 * 
 * @param msg Byte received.
 * @param i_message 1 if byte received belongs to information frame, 0 otherwise.
 */
void handleBcc1State(char msg, int i_message);

/**
 * @brief Handles DATA_INF state, only advancing if a FLAG is received.
 * 
 * @param msg Byte received.
 */
void handleDataState(char msg);

/**
 * @brief Handles STOP state.
 * 
 * @param msg 
 */
void handleStopState(char msg);

/**
 * @brief Sets new state machine value.
 * 
 * @param state New state machine value.
 */
void update_state(state_machine state);

/**
 * @brief Get the current state of the state machine.
 * 
 * @return state_machine 
 */
state_machine getStateMachine();
