clear
gcc ../receptor.c ../protocol.c ../protocol.h ../connection.c ../connection.h ../message.h ../message.c ../state_machine.c ../state_machine.h ../file_handler.c ../file_handler.h   -o receptor
./receptor /dev/ttyS1
