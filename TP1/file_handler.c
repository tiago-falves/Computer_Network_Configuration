#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "file_handler.h"

char* read_file(char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL){
        perror("Error reading file");
    }

    char* line = malloc(1024);
    char* buffer = malloc(1024);

    while (fread(line , sizeof(*line), 10, file) > 0){
        int new_size = strlen(line) + strlen(buffer);
        if (strlen(line) + strlen(buffer) > sizeof(buffer)){
            buffer = realloc(buffer, new_size);
        }

        strncat(buffer, line, strlen(line));
        memset(line, 0, sizeof(line));
    }

    free(line);
    fclose(file);
    return buffer;
}

int write_file(char* file_path, char* buffer) {
    FILE* file = fopen(file_path, "w");
    if (file == NULL){
        perror("Error reading file");
    }
    
    if (fputs(buffer, file) < 0){
        printf("Error writing to file!\n");
    }
    fclose(file);
    return 0;
}

//USED FOR TESTING REASONS

/*
char** divideBuffer(char* buffer, int* size) {
	int position = 0, pos = 0;

	char** divided_data = malloc(sizeof(char*));
    divided_data[0] = malloc(255);

	for (int i = 0; i < strlen(buffer); i++){
		pos = i % 255;
		if (pos == 0 && i != 0){
			position++;
			divided_data = realloc(divided_data, (position + 1) * sizeof(char*));
			divided_data[position] = (char*)calloc(255, sizeof(char));
		}
		divided_data[position][pos] = buffer[i];
	}
    *size = position + 1;
	return divided_data;
}

int main(){
    char* buffer = read_file("scripts/test.txt");
    int divided_size = 0;
    char** divided_buffer = divideBuffer(buffer, &divided_size);

    for (int i = 0; i < divided_size; i++) {
        printf("Line %d \n %s\n\n", i, divided_buffer[i]);
    }
    
    write_file("hmmm.txt", buffer);
    return 0;
}
*/