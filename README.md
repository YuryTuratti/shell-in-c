# Shell do Yury

Um shell desenvolvido em **C**, criado do zero para estudar conceitos de sistemas operacionais, manipulação de processos, parsing de comandos, gerenciamento de diretórios e desenvolvimento multiplataforma.

> 🚧 **Projeto em desenvolvimento**

## Sobre o projeto

O **Shell do Yury** é um shell de terminal desenvolvido do zero utilizando a linguagem C.

O objetivo é entender, na prática, como um shell funciona internamente: como comandos são recebidos, separados, interpretados e executados pelo sistema operacional.

O projeto também busca oferecer suporte a diferentes sistemas operacionais, abstraindo diferenças entre **Windows, Linux e macOS**.

## Funcionalidades

Atualmente, o Shell do Yury possui:

* [x] Parser de argumentos utilizando `strtok()`
* [x] `cd`
* [x] `cd ~`
* [x] `pwd`
* [x] `clear`
* [x] `help`
* [x] `mkdir`
* [x] `delete`
* [x] `exit`
* [x] Identificação do sistema operacional
* [x] Abstração de operações específicas do sistema operacional
* [x] Busca automática da pasta `HOME`
* [x] Arquivo externo para documentação dos comandos

## Comandos disponíveis

### `cd`

Altera o diretório atual.

```bash
cd diretorio
```

Exemplo:

```bash
cd projetos
```

### `cd ~`

Vai para a pasta `HOME` do usuário.

```bash
cd ~
```

### `pwd`

Mostra o diretório atual.

```bash
pwd
```

### `mkdir`

Cria um novo diretório.

```bash
mkdir nome_da_pasta
```

### `delete`

Remove um arquivo ou diretório.

```bash
delete nome
```

> ⚠️ O comando ainda está em desenvolvimento. Utilize com cuidado.

### `clear`

Limpa o terminal.

```bash
clear
```

O comando utilizado internamente é adaptado de acordo com o sistema operacional.

### `help`

Mostra os comandos disponíveis no Shell do Yury.

```bash
help
```

A documentação é carregada a partir de um arquivo externo.

### `exit`

Encerra o Shell do Yury.

```bash
exit
```

## Arquitetura

O projeto possui uma camada de abstração para lidar com diferenças entre sistemas operacionais.

Por exemplo:

```c
#ifdef _WIN32

#define CHANGE_DIR(path) _chdir(path)
#define CREATE_DIR(path) _mkdir(path)
#define CLEAR_SHELL "cls"

#else

#define CHANGE_DIR(path) chdir(path)
#define CREATE_DIR(path) mkdir(path, 0755)
#define CLEAR_SHELL "clear"

#endif
```

Assim, o restante do shell pode utilizar:

```c
CHANGE_DIR(path);
```

sem precisar conhecer diretamente a implementação específica de cada sistema operacional.

### Fluxo atual

```text
                  Usuário
                     │
                     ▼
              Comando digitado
                     │
                     ▼
                 div_args()
                     │
                     ▼
             Array de argumentos
                     │
                     ▼
              enterpretador()
                     │
          ┌──────────┴──────────┐
          │                     │
          ▼                     ▼
   Comandos internos      Comandos externos
          │                (em desenvolvimento)
          ▼
   cd / pwd / mkdir
   clear / help
   delete / exit
```

## Tecnologias

* C
* GCC
* APIs POSIX
* API do Windows
* Manipulação de arquivos
* Manipulação de diretórios
* Parsing de strings
* Variáveis de ambiente

## Estrutura do projeto

```text
Shell-do-Yury/
├── shell.h
├── shell.c
├── help.txt
├── main.c
└── README.md
```

A estrutura poderá ser modificada conforme novas funcionalidades forem implementadas.

## Compilação

### Linux / macOS

Utilizando GCC:

```bash
gcc main.c shell.c -o shell
```

Executar:

```bash
./shell
```

### Windows

Utilizando MinGW:

```bash
gcc main.c shell.c -o shell.exe
```

Executar:

```bash
shell.exe
```

## Roadmap

O objetivo é evoluir o Shell do Yury para um shell mais completo.

### Execução de processos

* [ ] Executar comandos externos
* [ ] `fork()`
* [ ] `execvp()`
* [ ] `waitpid()`
* [ ] Gerenciamento de processos
* [ ] Tratamento de sinais

### Parsing

* [ ] Suporte a argumentos com espaços
* [ ] Aspas duplas
* [ ] Aspas simples
* [ ] Variáveis de ambiente
* [ ] Melhor tratamento de erros

### Redirecionamento

* [ ] `>`
* [ ] `>>`
* [ ] `<`

### Pipes

* [ ] `|`
* [ ] Múltiplos pipes

### Recursos do Shell

* [ ] Histórico de comandos
* [ ] Aliases
* [ ] Sistema de configuração
* [ ] Autocomplete
* [ ] Melhor suporte multiplataforma

## Objetivo

O projeto está sendo desenvolvido principalmente como estudo prático de:

* Sistemas Operacionais
* Linguagem C
* Gerenciamento de processos
* Manipulação de memória
* Manipulação de arquivos
* Parsing
* APIs de sistemas operacionais
* Desenvolvimento multiplataforma

A implementação está sendo construída gradualmente, buscando compreender o funcionamento de cada componente do shell em vez de utilizar uma implementação pronta.

## Autor

**Yury Turatti**

## Formação em andamento

**Sistemas de Informação - UFU**

Projeto desenvolvido para estudos de C, Sistemas Operacionais e desenvolvimento de software multiplataforma.
