/* MACROS for byte stuffing */ 
#define FLAG_STUFFING_BYTE 0x5e
#define ESC_STUFFING_BYTE 0x5d

typedef struct {
    char * data;      /** Stuffed Data */
    int data_size;     /** Number of bytes after stuffing data*/
} data_stuff;

data_stuff stuffData(char* buffer,int length);
data_stuff unstuffData(char * buffer,int length);
