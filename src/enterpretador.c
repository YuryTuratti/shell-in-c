#include "../include/enterpretador.h"
#include "../include/builtins.h"
#include "../include/process.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void enterpretador(char *args[],char *home){
    
    if(args[0] == NULL)
        return;
    
    if(executar_builtins(args)){
        return;
    }

    cmd_externo(args);
}