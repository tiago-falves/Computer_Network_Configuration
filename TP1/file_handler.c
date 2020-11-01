#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "file_handler.h"

//remove
#include "data_stuffing.h"

int write_file(char* file_path, char* buffer, int data_size) {
    FILE* file = fopen(file_path, "ab");
    if (file == NULL){
        perror("Error reading file");
    }
    
    if (fwrite(buffer, sizeof(char), data_size, file) < 0){
        printf("Error writing to file!\n");
    }
    fclose(file);
    return 0;
}

int findSize(char file_name[]) 
{ 
    // opening the file in read mode 
    FILE* fp = fopen(file_name, "r"); 
  
    // checking if the file exist or not 
    if (fp == NULL) { 
        printf("File Not Found!\n"); 
        return -1; 
    } 
  
    fseek(fp, 0L, SEEK_END); 
  
    // calculating the size of the file 
    int res = ftell(fp); 
  
    // closing the file 
    fclose(fp); 
  
    return res; 
} 
