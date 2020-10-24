#include "data_stuffing.h"
#include "message.h"

data_stuff stuffData(char* buffer,int length){
	int stuff_data_index = 0;
	char stuffed_data_buffer[2*length];
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
    char unstuffed_data[length];
    for (int i = 0; i < DATA_INF_BYTE; i++){
        unstuffed_data[unstuffed_data_index++] = buffer[i];
    }
    while (i < length-2){
        if (buffer[i] == ESC)
        {
            i++;
            if(buffer[i] == FLAG_STUFFING_BYTE){
                unstuffed_data[unstuffed_data_index++] = FLAG;
            } else if(buffer[i] == ESC_STUFFING_BYTE){
                unstuffed_data[unstuffed_data_index++] = ESC;
            } else return data_stuff;
        } else unstuffed_data[unstuffed_data_index++] = buffer[i];
        i++;
    }

    for (i; i < length; i++){
        unstuffed_data[unstuffed_data_index++] = buffer[i];
    }

    data_stuff.data = unstuffed_data;
    data_stuff.data_size = unstuffed_data_index;

    return  data_stuff;
}

