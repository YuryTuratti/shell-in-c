#include "shell.h"

#define TAMANHO_PATH 1024
#define MAX_ARGS 64

//---------------------------------------------------------//

// descobrir qual é o sistema operacional do usuário
#if defined(_WIN32) || defined(_WIN64)
    #define ENV_HOME "USERPROFILE"
    #define GET_CURRENT_DIR(buffer, size) _getcwd(buffer, size)
    #define CREATE_DIR(path) _mkdir(path)
    #define CHANGE_DIR(path) _chdir(path)
    #define DELETE_DIR "rmdir /s /q \"%s\""
    #define CLEAR_SHELL "cls"
#else
    #define ENV_HOME "HOME"
    #define GET_CURRENT_DIR(buffer, size) getcwd(buffer, size)
    #define CHANGE_DIR(path) chdir(path)
    #define CREATE_DIR(path) mkdir(path,0755)
    #define DELETE_DIR "rm -rf \"%s\""
    #define CLEAR_SHELL "clear"
#endif

//---------------------------------------------------------//

int buscar_home(char **caminho){

    char *env_home = getenv(ENV_HOME);

    // caso nao tenha encontrado a home
    if(env_home == NULL) {
        printf("Erro ao buscar a home\n");
        *caminho = NULL;
        return 0;
    }
    //descobrir o tamanho da string
    size_t tam = strlen(env_home) + 1;

    // alocando o tamanho do ponteiro que esta na main
    *caminho = (char*) malloc(tam);

    if(*caminho == NULL){
        printf("Erro ao alocar a memória da home(main)");
        return 0;
    }
    // copia do caminho para a memória recem alocada
    strcpy(*caminho,env_home);
    return 1;
}

//---------------------------------------------------------//

void salvar_caminho_orig(char caminho_original[]){
   
    //getcwd(buffer, tamanho): Copia o caminho do diretório de trabalho atual para dentro do buffer.
    if(GET_CURRENT_DIR(caminho_original, TAMANHO_PATH) != NULL)
        printf("caminho original : %s\n", caminho_original);
    else 
        printf("Erro ao salvar o caminho original\n");
}

//---------------------------------------------------------//

void mostrar_pwd(void){

    char caminho[TAMANHO_PATH];

    if(GET_CURRENT_DIR(caminho, sizeof(caminho)) != NULL){
        printf("%s\n", caminho);
    }
    else{
        printf("Erro ao buscar diretorio atual\n");
    }
}
 
//---------------------------------------------------------//

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

//---------------------------------------------------------//

void limpar_shell(){
    system(CLEAR_SHELL);
}

//---------------------------------------------------------//

void mostrar_help(char *caminho_original){

    char caminho_help[TAMANHO_PATH];
    
    snprintf(caminho_help, sizeof(caminho_help),"%s/help.text",caminho_original);
    
    FILE *arquivo = fopen(caminho_help, "r");

    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo de ajuda\n");
        return;
    }

    char linha[256];

    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        printf("%s", linha);
    }
    printf("\n");
    fclose(arquivo);
}

//---------------------------------------------------------//

void criar_pasta(char *name){

    if(CREATE_DIR(name) == 0){
        printf("Diretorio criado\n");
    }
    else{
        printf("Erro ao criar o Diretorio\n");
    }
    
}

//---------------------------------------------------------//

void apagar_arquivo(char *nome){

    char comando[TAMANHO_PATH];
    snprintf(comando, sizeof(comando), DELETE_DIR,nome);
    system(comando);
}

//---------------------------------------------------------//

void enterpretador(char *args[],char *home,char *caminho_original){

    if(args[0] == NULL)
        return;
    
    if(strcmp(args[0], "cd") == 0){

        //precisa existir exatamente um argumento
        if(args[1] != NULL && args[2] == NULL){
            
            if(strcmp(args[1], "~") == 0){
                if(CHANGE_DIR(home) == 0){
                    printf("Diretorio alterado para a home com sucesso!\n");
                }
                else{
                    printf("Erro ao tentar ir para a Home!\n");
                }
            }
            else if(CHANGE_DIR(args[1]) == 0){
                printf("Diretorio alterado com sucesso!\n");
            }
            else{
                printf("Erro ao tentar alterar o diretorio\n");
            }
        }    
        //entra somente se ele tiver passado outro argumento ou diretorio errado
        else{
            printf("Argumento nao permitido\n");
        }
        return;
    }

    else if(strcmp(args[0], "clear") == 0){
        if(args[1] == NULL){
            limpar_shell();
        }
        else{
            printf("Argumentos nao permitidos\n");
            return;
        }
    }
    else if(strcmp(args[0], "help") == 0){
        if(args[1] == NULL){
            mostrar_help(caminho_original);
        }
        else{
            printf("Argumento nao permitido\n");
        }
    }
    else if(strcmp(args[0], "pwd") == 0){
        if(args[1] == NULL){
            mostrar_pwd();
        }
        else{
            printf("Argumento nao permitido\n");
        }
    }
    else if(strcmp(args[0], "mkdir") == 0){
        if(args[1] == NULL){
            printf("De um nome para o diretorio\n");
        }
        else if(args[2] != NULL){
            printf("Argumento nao permitiro\n");
        }
        else{
            criar_pasta(args[1]);
        }
    }
    else if(strcmp(args[0], "exit") == 0){
        if(args[1] == NULL){
            printf("Saindo bye bye ....\n");
            exit(0);
        }
        else{
            printf("Argumento nao permitido\n");
        }
    }
    else if(strcmp(args[0], "delete") == 0){
        if(args[1] != NULL && args[2] == NULL){
            apagar_arquivo(args[1]);
        }
        else{
            printf("Argumento nao permitido\n");
        }
    }

    else{
        printf("\n'%s' nao eh um argumento nativo no shell-do-yury, se houver duvidas use o comadando help\n\n",args[0]);
    }
}