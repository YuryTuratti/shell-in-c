#include "shell.h"

#define TAMANHO_PATH 1024
#define MAX_ARGS 64

//---------------------------------------------------------//

// descobrir qual é o sistema operacional do usuário
#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
    #include <windows.h>    
    #define ENV_HOME "USERPROFILE"
    #define GET_CURRENT_DIR(buffer, size) _getcwd(buffer, size)
    #define CREATE_DIR(path) _mkdir(pasth)
    #define CHANGE_DIR(path) _chdir(path)
    #define DELETE_DIR(path) _rmdir(path)
    #define DELETE_FILE "rm \"%s\""
    #define CLEAR_SHELL "cls"
#else
    #include <dirent.h>
    #define ENV_HOME "HOME"
    #define GET_CURRENT_DIR(buffer, size) getcwd(buffer, size)
    #define CHANGE_DIR(path) chdir(path)
    #define CREATE_DIR(path) mkdir(path,0755)
    #define DELETE_DIR(path) rmdir(path)
    #define DELETE_FILE "rm \"%s\""
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

void cmd_cd(char *args[], char *home){

    if(strcmp(args[0], "cd") == 0){

        //precisa existir exatamente um argumento
        if(args[1] != NULL && args[2] == NULL){
            
            if(strcmp(args[1], "~") == 0){
                if(CHANGE_DIR(home) == 0){
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

void cmd_clear(char *args[]){
    if(args[1] == NULL){
        system(CLEAR_SHELL);
    }
    else{
        printf("\033[31mArgumento nao permitido\033[0m\n");
    }
}
//---------------------------------------------------------//

void cmd_help(char *caminho_original,char *args[]){

    if(args[1] != NULL){
        printf("\033[31mArgumento nao permitido\033[0m\n");
        return;
    }
    else{
        char caminho_help[TAMANHO_PATH];
        
        snprintf(caminho_help, sizeof(caminho_help),"%s/help.text",caminho_original);
        
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
        printf("\033[31Argumento nao permitido\033[0m\n");
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

void cmd_ls(bool mostrar_ocultos){
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

void cmd_externo(char *args[]){
    
    printf("teste\n");
    return;
}

//---------------------------------------------------------//

void enterpretador(char *args[],char *home,char *caminho_original){

    if(args[0] == NULL)
        return;
    
    if(strcmp(args[0], "cd") == 0){
        cmd_cd(args,home);
    }
    else if(strcmp(args[0], "clear") == 0){
        cmd_clear(args);
    }
    else if(strcmp(args[0], "help") == 0){
        cmd_help(caminho_original,args);
    }
    else if(strcmp(args[0], "pwd") == 0){
        cmd_pwd(args);
    }
    else if(strcmp(args[0], "mkdir") == 0){
       cmd_mkdir(args);
    }
    else if(strcmp(args[0], "exit") == 0){
        cmd_exit(args);
    }
    else if(strcmp(args[0], "delete") == 0){
        cmd_delete(args);
    }
    else if(strcmp(args[0],"ls") == 0){
        if(args[1] == NULL){
            cmd_ls(false);
        }
        else if(strcmp(args[1], "-la") == 0 && args[2] == NULL){
            cmd_ls(true);
        }
        else{
            printf("\033[31mArgumento nao permitido\033[0m\n");
        }
    }

    else{
        cmd_externo(args);
    }
}