#include "../include/enterpretador.h"
#include "../include/utils.h"
#include "../include/builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 64

int main(void){

    char comando[1024];
    char *args[MAX_ARGS];
    char *home = NULL;

    builtins_init();// salvando o caminho original
    buscar_home(&home);

    while(1){
        printf("\033[32mshell-do-yury\033[0m >");

        fgets(comando, 1024, stdin);
        comando[strcspn(comando,"\n")] = '\0';
        
        div_args(comando,args);
        enterpretador(args,home);
    }

    return 0;
}