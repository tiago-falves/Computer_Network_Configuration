#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "file_handler.h"

//remove
#include "data_stuffing.h"

int read_file(char* file_path, int data_block_size, char* buffer) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL){
        perror("Error reading file");
    }

    int ret = fread(buffer , sizeof(char), data_block_size, file);

    fclose(file);
    return ret;
}

int write_file(char* file_path, char* buffer) {
    FILE* file = fopen(file_path, "ab");
    if (file == NULL){
        perror("Error reading file");
    }
    
    if (fwrite(buffer, sizeof(char), 251, file) < 0){
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

//USED FOR TESTING REASONS

/*
int main(){
    FILE* file = fopen("files/pinguim.gif", "rb");
    if (file == NULL){
        perror("Error reading file");
    }

    int ret = 0;
    char* buffer = calloc(252, sizeof(char));

    while (1){
        int i = 0;
        ret = fread(buffer, sizeof(char), 251, file);

        if (ret <= 0){
            break;
        }

        printf("data\n");
        for (int i = 0; i < ret; i++){
            printf("%02x ", (unsigned char) buffer[i]);
        }
        printf("\n\n");

        write_file("files/hmmm.gif", buffer);

        memset(buffer, 0, 251);
        i++;
    }

    fclose(file);
    
    return 0;
}
*/
