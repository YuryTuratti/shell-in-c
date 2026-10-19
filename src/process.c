#include "../include/process.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void cmd_externo(char *args[]){
    
    //cria o processo filho
    pid_t pid = fork();

    //se for menor que 0 houve um erro
    if(pid < 0){
        perror("Fork");
        return;
    }
    // = 0 processo filho criado
    if(pid == 0){
        //se for executado o processo sera executado e voltara para a main 
        execvp(args[0],args);

        //so sera executado se houver erro no execvp()
        perror(args[0]);
        _exit(127);
    }
   // pid -> qual processo filho o pai deve esperar,
   // null -> nao quero analisar como o filho terminou,
   // 0 -> espera normalmente)
    
    if(waitpid(pid,NULL,0) == -1){
        perror("waitpid");
    }

}