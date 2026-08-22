#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

int buscar_home(char **caminho);
void salvar_caminho_orig(char caminho_original[]);
void div_args(char comando[], char *args[]);
void enterpretador(char *args[], char *home, char *caminho_original);
void cmd_pwd(char *args[]);
void cmd_clear(char *args[]);
void cmd_help(char *caminho_original,char *args[]);
void cmd_ls(bool mostrar_ocultos);
void cmd_cd(char *args[], char *home);
void cmd_mkdir(char *args[]);
void cmd_delete(char *args[]);
void cmd_exit(char *args[]);
void cmd_externo(char *args[]);