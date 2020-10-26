#include "data_stuffing.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

data_stuff stuffData(char* buffer,int length){
	int stuff_data_index = 0;
	char* stuffed_data_buffer = calloc(2*length, sizeof(char));
	for (int i = 0; i < length; i++)
	{
		if(buffer[i] == FLAG){
			stuffed_data_buffer[stuff_data_index++] =ESC;
			stuffed_data_buffer[stuff_data_index++] = FLAG_STUFFING_BYTE;
		} else if( buffer[i] == ESC){
			stuffed_data_buffer[stuff_data_index++] =ESC;
			stuffed_data_buffer[stuff_data_index++] = ESC_STUFFING_BYTE;
		} else{
			stuffed_data_buffer[stuff_data_index++] = buffer[i];
		}
	}

	data_stuff data;
	data.data = stuffed_data_buffer;
	data.data_size = stuff_data_index;

	return data;

}

data_stuff unstuffData(char * buffer,int length){
    data_stuff data_stuff;

    int i = DATA_INF_BYTE;
    int unstuffed_data_index = 0;
    char* unstuffed_data = calloc(length, sizeof(char));
    for (int j = 0; j < DATA_INF_BYTE; j++){
        unstuffed_data[unstuffed_data_index++] = buffer[j];
    }
    while (i < length-2){
        if (buffer[i] == ESC){
            i++;
            if(buffer[i] == FLAG_STUFFING_BYTE){
                unstuffed_data[unstuffed_data_index++] = FLAG;
            } else if(buffer[i] == ESC_STUFFING_BYTE){
                unstuffed_data[unstuffed_data_index++] = ESC;
            } else printf("ERROR UNSTUFFING DATA\n");
        } else unstuffed_data[unstuffed_data_index++] = buffer[i];
        i++;
    }

    for (; i < length; i++){
        unstuffed_data[unstuffed_data_index++] = buffer[i];
    }

    data_stuff.data = unstuffed_data;
    data_stuff.data_size = unstuffed_data_index;

    return  data_stuff;
}

