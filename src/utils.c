#include "../include/utils.h"

#include <string.h>

#define MAX_ARGS 64

void div_args(char comando[], char *args[]){
    
    char* token;
    int i = 0;
    
    token = strtok(comando, " \n");
    
    while(token != NULL && i < MAX_ARGS - 1){
        args[i] = token;
        i++;

        token = strtok(NULL, " \n");
    }

    args[i] = NULL;

}
