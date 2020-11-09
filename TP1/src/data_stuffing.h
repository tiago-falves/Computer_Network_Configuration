/* MACROS for byte stuffing */ 
#define FLAG_STUFFING_BYTE 0x5e
#define ESC_STUFFING_BYTE 0x5d

typedef struct {
    char * data;      /** Stuffed Data */
    int data_size;     /** Number of bytes after stuffing data*/
} data_stuff;

/**
 * @brief Applies data stuffing to data received in parameter buffer.
 * 
 * @param buffer Data to be stuffed.
 * @param length Size, in bytes, of data to be stuffed.
 * @return data_stuff Struct containing stuffed data and its size.
 */
data_stuff stuffData(char* buffer,int length);

/**
 * @brief Reverts data stuffing previously applied.
 * 
 * @param buffer Data to be unstuffed.
 * @param length Size, in bytes, of data to be unstuffed.
 * @return data_stuff Struct containing original data and its size.
 */
data_stuff unstuffData(char * buffer,int length);
