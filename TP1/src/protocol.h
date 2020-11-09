#define FALSE 0
#define TRUE 1

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>	
#include <strings.h>
#include <signal.h>

#include "utils.h"

//Message configurations
#define RESEND_DELAY 3
#define WRITE_NUM_TRIES 3

/**
 * @brief Opens serial port connection. If connecition_type is of type EMISSOR, write_supervision_message_retry is called to send SET message and wait for UA message. If connection_type is of type RECEPTOR it waits for a SET message and sends a UA message after it.
 * 
 * @param arg Port identifier.
 * @param connection_type EMISSOR | RECEPTOR
 * @return int Serial port file descriptor.
 */
int llopen(char* arg, conn_type connection_type);

/**
 * @brief Closes connection. If 
 * 
 * @param fd Serial port file descriptor.
 * @return int Return value of function close_connection(fd)
 */
int llclose(int fd);

/**
 * @brief Function receives data from file and its size, builds information frame, sends it and waits for acknowledgement message with alarm signal implemented to enable retransmissions.
 * 
 * @param fd Serial port file descriptor.
 * @param data Data to be transmitted.
 * @param data_size Size of data, in bytes.
 * @return int Number of bytes written, negative value in case of error.
 */
int llwrite(int fd, char* data, int data_size);

/**
 * @brief Receives information frame, checks if it consists of a repeated message and if bcc2 is correct and sends correspondent acknowledgement message back to emitter.
 * 
 * @param fd Serial port file descriptor.
 * @param buffer Data received.
 * @return int Number of bytes read, negative value in case of error.
 */
int llread(int fd, char* buffer);
