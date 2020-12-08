#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"
#include <assert.h>

#define ARGUMENT_FTP "ftp://"
#define ARGUMENT_POSITION 1
#define ANONYMOUS_USER "anonymous"
#define ANONYMOUS_PASS "something"

void parse_arguments(arguments *args, int argc, char *argv[])
{
    if (!check_arg(argc, argv))
    {
        printf("Usage: ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(1);
    }
    printf("Parsed arguments correctly\n");

    int indexUserPasswordEnd = hasUserPassword(argv[ARGUMENT_POSITION]);
    int hostIndex = strlen(ARGUMENT_FTP);
    if (indexUserPasswordEnd > 0)
        hostIndex = parseUserPassword(argv[ARGUMENT_POSITION], args, indexUserPasswordEnd);
    else{
        args->user = ANONYMOUS_USER;
        args->password = ANONYMOUS_PASS;
    }

    int hostSize = 0;
    for (int i = hostIndex; i < strlen(argv[1]); i++)
    {
        if (argv[ARGUMENT_POSITION][i] == '/')
        {
            strncpy(args->host, argv[ARGUMENT_POSITION]+hostIndex, hostSize);
            break;
        }
        hostSize++;
    }
    strcpy(args->url_path, argv[ARGUMENT_POSITION]+hostIndex +hostSize);

    
    printSeparator();
    printf("Host: %s\n", args->host);
    printf("Url: %s\n", args->url_path);
    printf("User: %s\n",args->user);
    printf("Pass: %s\n",args->password);
    printSeparator();
}

bool check_arg(int argc, char *argv[])
{
    int initialFtp = strncmp(argv[ARGUMENT_POSITION], ARGUMENT_FTP, strlen(ARGUMENT_FTP) - 1);
    return (argc == 2) && (initialFtp == 0); //Ver se é igual a ftp://
}

int hasUserPassword(char *str)
{
    for (int i = strlen(ARGUMENT_FTP); i < strlen(str); i++) 
        if (str[i] == '@')
            return i;
    
    return -1;
}

int parseUserPassword(char *str, arguments *args, int userPasswordEnd)
{
    int userSize = 0;
    int passwordSize = 0;
    int passwordBeggining = 0;
    for (int i = strlen(ARGUMENT_FTP); i < userPasswordEnd; i++)
    {
        if (str[i] == ':')
        {
            strncpy(args->user, str + strlen(ARGUMENT_FTP), userSize);
            userSize++;
            break;
        }
        userSize++;
    }

    passwordBeggining = strlen(ARGUMENT_FTP) + userSize;
    for (int i = passwordBeggining; i < userPasswordEnd + passwordBeggining; i++)
    {

        if (str[i] == '@')
        {
            strncpy(args->password, str+passwordBeggining, passwordSize);
            break;
        }
        passwordSize++;
    }
    return passwordBeggining + passwordSize + 1;
}


void printSeparator(){
    printf("\n***************************\n\n");
}
