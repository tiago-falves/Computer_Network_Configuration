#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_handler.h"

char* read_file(char* file_path) {
    FILE* file = fopen(file_path, "r");
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

    fclose(file);
    return buffer;
}

int write_file(char* file_path, char* buffer) {
    FILE* file = fopen(file_path, "w");
    if (fputs(buffer, file) < 0){
        printf("Error writing to file!\n");
    }
    fclose(file);
    return 0;
}

//USED FOR TESTING REASONS
/*
int main(){
    char* buffer = read_file("test.txt");
    write_file("hmmm.txt", buffer);
}
*/