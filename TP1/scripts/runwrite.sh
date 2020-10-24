gcc  ../protocol.c ../protocol.h ../connection.c ../connection.h ../message.h ../message.c ../state_machine.c ../state_machine.h ../file_handler.c ../file_handler.h ../utils.c ../utils.h ../main.c -o emissor
clear
./emissor 0 emissor test.txt
