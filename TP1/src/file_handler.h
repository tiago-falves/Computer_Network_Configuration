/**
 * @brief Write content present in buffer, to file with name filepath
 * 
 * @param file_path Name of the file where data will be written to.
 * @param buffer Data to be written.
 * @param data_size Size, in bytes, of buffer.
 * @return int 
 */
int write_file(char* file_path, char* buffer, int data_size);

/**
 * @brief Finds size, in bytes, of file with name file_name.
 * 
 * @param file_name Name of the file.
 * @return int Size of the file.
 */
int findSize(char file_name[]);