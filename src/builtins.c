#include "../include/builtins.h"
#include "../include/enterpretador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAMANHO_PATH 1024
#define MAX_ARGS 64

#if defined(_WIN32) || defined(_WIN64)

    #include <direct.h>
    #include <windows.h>

    #define ENV_HOME "USERPROFILE"
    #define GET_CURRENT_DIR(buffer, size) _getcwd(buffer, size)
    #define CREATE_DIR(path) _mkdir(path)
    #define CHANGE_DIR(path) _chdir(path)
    #define DELETE_DIR(path) _rmdir(path)
    #define CLEAR_SHELL "cls"

#else

    #include <unistd.h>
    #include <dirent.h>
    #include <sys/stat.h>

    #define ENV_HOME "HOME"
    #define GET_CURRENT_DIR(buffer, size) getcwd(buffer, size)
    #define CHANGE_DIR(path) chdir(path)
    #define CREATE_DIR(path) mkdir(path, 0755)
    #define DELETE_DIR(path) rmdir(path)
    #define CLEAR_SHELL "clear"

#endif

static char home_global[TAMANHO_PATH];

typedef struct {
    char *nome;
    void (*funcao)(char *args[]);
}Comando;

static Comando comandos[] = {
    {"help",cmd_help},
    {"cd",cmd_cd},
    {"pwd",cmd_pwd},
    {"mkdir",cmd_mkdir},
    {"ls",cmd_ls},
    {"exit",cmd_exit},
    {"clear",cmd_clear},
    {"delete",cmd_delete}
};

int executar_builtins(char *args[]){
    
    int total = sizeof(comandos) / sizeof(comandos[0]);

    for (int i = 0; i < total; i++) {
        if (strcmp(args[0], comandos[i].nome) == 0) {
            comandos[i].funcao(args);
            return 1;
        }
    }

    return 0;
}

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

void builtins_init(void){
   
    //getcwd(buffer, tamanho): Copia o caminho do diretório de trabalho atual para dentro do buffer.
    if(GET_CURRENT_DIR(home_global, TAMANHO_PATH) != NULL)
        printf("caminho original : %s\n", home_global);
    else 
        printf("Erro ao salvar o caminho original\n");
}

//---------------------------------------------------------//

void cmd_cd(char *args[]){

    if(strcmp(args[0], "cd") == 0){

        //precisa existir exatamente um argumento
        if(args[1] != NULL && args[2] == NULL){
            
            if(strcmp(args[1], "~") == 0){
                if(CHANGE_DIR(home_global) == 0){
                    printf("Diretorio alterado para a home com sucesso!\n");
                }
                else{
                    printf("\033[31mErro ao tentar ir para a Home!\033[0m\n");
                }
            }
            else if(CHANGE_DIR(args[1]) == 0){
                printf("Diretorio alterado com sucesso!\n");
            }
            else{
                printf("\033[31mErro ao tentar alterar o diretorio\033[0m\n");
            }
        }    
        //entra somente se ele tiver passado outro argumento ou diretorio errado
        else{
            printf("\033[31mArgumento nao permitido\033[0m\n");
        }
    }
}

//---------------------------------------------------------//

void cmd_pwd(char *args[]){

    if(args[1] != NULL){
        printf("\033[31mArgumento nao permitido\033[0m\n");
        return;
    }
    
    else{
        char caminho[TAMANHO_PATH];

        if(GET_CURRENT_DIR(caminho, sizeof(caminho)) != NULL){
            printf("%s\n", caminho);
        }
        else{
            printf("Erro ao buscar diretorio atual\n");
        }
    }
}

//---------------------------------------------------------//

void cmd_clear(char *args[]){
    if(args[1] == NULL){
        system(CLEAR_SHELL);
    }
    else{
        printf("\033[31mArgumento nao permitido\033[0m\n");
    }
}
//---------------------------------------------------------//

void cmd_help(char *args[]){

    if(args[1] != NULL){
        printf("\033[31mArgumento nao permitido\033[0m\n");
        return;
    }
    else{
        char caminho_help[TAMANHO_PATH];
        
        strcpy(caminho_help,"../docs/help.txt");
        
        FILE *arquivo = fopen(caminho_help, "r");

        if(arquivo == NULL){
            printf("Erro ao abrir o arquivo de ajuda\n");
            return;
        }

        char linha[256];

        while(fgets(linha, sizeof(linha), arquivo) != NULL){
            printf("\033[36m%s\033[0m", linha);
        }
        printf("\n");
        fclose(arquivo);
    }
}

//---------------------------------------------------------//

void cmd_mkdir(char *args[]){

    if(args[1] == NULL){
        printf("\033[31mDe um nome para o diretorio\033[0m\n");
        return;
    }
    else if(args[2] != NULL){
        printf("\033[31mArgumento nao permitido\033[0m\n");
        return;
    }
    else{
        if(CREATE_DIR(args[1]) == 0){
            printf("Diretorio criado\n");
        }
        else{
            printf("Erro ao criar o Diretorio\n");
        }
    }
}

//---------------------------------------------------------//

void cmd_exit(char *args[]){

    if(args[1] != NULL){
        printf("\033[31mArgumento nao permitido\033[0m\n");
        return;
    }
    else{
        printf("Saindo bye bye . . .\n");
        exit(0);
    }
}

//---------------------------------------------------------//

void cmd_delete(char *args[]){

    if (args[1] == NULL) {
        printf("\033[31mAdicione o nome do diretorio\033[0m\n");
        return;
    }

    if (args[2] != NULL) {
        printf("\033[31mArgumentos demais\033[0m\n");
        return;
    }

    if (DELETE_DIR(args[1]) != 0) {
        perror("delete");
        return;
    }
}

//---------------------------------------------------------//

void cmd_ls(char *args[]){

    bool mostrar_ocultos = false;

    if(args[1] != NULL && strcmp(args[1],"-la") == 0){
        mostrar_ocultos = true;
    }

    #ifdef _WIN32 

        WIN32_FIND_DATA dados;
        HANDLE dir = FindFirstFile("*", &dados);

        if (dir == INVALID_HANDLE_VALUE) {
            printf("\033[31mErro ao abrir diretorio\033[0m\n");
            return;
        }
        do {

            if (!mostrar_ocultos) {

                if ((dados.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) || dados.cFileName[0] == '.'){
                    continue;
                }
            }
            printf("%s\n", dados.cFileName);        
        } while (FindNextFile(dir, &dados));

        FindClose(dir);

        #else

            DIR *dir = opendir(".");

            if(dir == NULL){
                printf("\033[31mErro ao abrir diretorio\033[0m\n");
                return;
            }

            struct dirent *entrada;

            while((entrada = readdir(dir)) != NULL){

                if(!mostrar_ocultos && entrada->d_name[0] == '.'){
                    continue;
                }
                printf("%s\n", entrada->d_name);
            }
            closedir(dir);
        


    #endif 
}