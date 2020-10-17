gcc ../receptor.c ../protocol.c ../protocol.h ../connection.c ../connection.h ../message.h ../message.c  -o receptor
./receptor /dev/ttyS0
