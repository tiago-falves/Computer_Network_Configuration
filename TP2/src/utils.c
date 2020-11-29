#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"
#include <assert.h>

#define ARGUMENT_FTP "ftp://"
#define ARGUMENT_POSITION 1

arguments parse_arguments(int argc, char *argv[])
{
    if (!check_arg(argc, argv))
    {
        printf("Usage: ftp://[<user>:<password>@]<host>/<url-path>\n");
        printf("%s\n", argv[1]);
        exit(1);
    }
    printf("Parsed arguments correctly\n");

    arguments args;
    int indexUserPasswordEnd = hasUserPassword(argv[ARGUMENT_POSITION]);
    //printf("%d",indexUserPasswordEnd);
    if (indexUserPasswordEnd > 0)
    {
        printf("Parsed User:Password correctly\n");
        parseUserPassword(argv[ARGUMENT_POSITION], &args, indexUserPasswordEnd);
        printf("User: %s\n",args.user);
    }

    return args;
}

bool check_arg(int argc, char *argv[])
{
    int initialFtp = strncmp(argv[ARGUMENT_POSITION], ARGUMENT_FTP, strlen(ARGUMENT_FTP) - 1);
    return (argc == 2) && (initialFtp == 0); //Ver se é igual a ftp://
}

int hasUserPassword(char *str)
{
    for (int i = strlen(ARGUMENT_FTP); i < strlen(str); i++)
    {
        printf("%c", str[i]);
        if (str[i] == '@')
            return i;
    }
    return -1;
}

bool parseUserPassword(char *str, arguments *args, int userPasswordEnd)
{
    int userSize = 0;
    for (int i = strlen(ARGUMENT_FTP); i < userPasswordEnd; i++)
    {
        if(str[i] == ':'){
            strncpy(args->user,str,userSize);
        }
        userSize++;
    }
    return true;
}
