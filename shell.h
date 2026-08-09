#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>

void salvar_caminho_orig(char caminho_original[]);
void div_args(char comando[], char *args[]);
int buscar_home(char **caminho);
void mostrar_pwd(void);
void limpar_shell();
void mostrar_help(char *caminho_original);
void criar_pasta();
void apagar_arquivo(char *nome);
void enterpretador(char *args[], char *home, char *caminho_original);