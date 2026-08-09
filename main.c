#include "shell.h"

#define MAX_ARGS 64

int main(void){

    char comando[1024];
    char caminho_original[1024];
    char *args[MAX_ARGS];
    char *home = NULL;

    salvar_caminho_orig(caminho_original);// salvando o caminho original
    buscar_home(&home);// Buscando a home do usuário

    while(1){
        printf("shell-do-yury >");

        fgets(comando, 1024, stdin);
        comando[strcspn(comando,"\n")] = '\0';
        
        div_args(comando,args);
        enterpretador(args,home,caminho_original);
    }

    return 0;
}