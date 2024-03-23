#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_COMMAND_SIZE 512

int main() {
  char command[MAX_COMMAND_SIZE];
  char *args[MAX_COMMAND_SIZE / 2 + 1]; // Argumentos do comando
  int should_run = 1;

  while (should_run) {
    printf("> ");
    fflush(stdout);

    // Ler o comando do usuário
    fgets(command, MAX_COMMAND_SIZE, stdin);
    command[strcspn(command, "\n")] = 0; // Remover a nova linha

    // Se o comando for "exit", sair do shell
    if (strcmp(command, "exit") == 0) {
      should_run = 0;
      continue;
    }

    // Se o comando for "cd", mudar o diretório"
    if (strcmp(args[0], "cd") == 0) {
      if (chdir(args[1]) != 0) {
        perror("Erro ao mudar de diretório");
      }
      continue;
    }

    // Se o comando for "ln", cria um link para um arquivo.
    if (strcmp(args[0], "ln") == 0) {
      if (link(args[1], args[2]) == -1) {
        perror("Erro ao criar link");
      }
      continue;
    }

    // Se o comando for "rm", remove um arquivo.
    if (strcmp(args[0], "rm") == 0) {
      if (unlink(args[1]) == -1) {
        perror("Erro ao remover arquivo");
      }
      continue;
    }

    // Dividir o comando em argumentos
    char *token = strtok(command, " ");
    int i = 0;
    while (token != NULL) {
      args[i] = token;
      i++;
      token = strtok(NULL, " ");
    }
    args[i] = NULL; // Finalizar a lista de argumentos com NULL

    // Criar um novo processo
    pid_t pid = fork();
    if (pid == 0) {
      // Executar o comando no processo filho
      if (execvp(args[0], args) == -1) {
        perror("Erro ao executar o comando");
      }
      exit(EXIT_FAILURE);
    } else if (pid < 0) {
      // Erro ao criar o processo
      perror("Erro ao criar o processo");
    } else {
      // Esperar o processo filho terminar
      wait(NULL);
    }
  }

  return 0;
}
