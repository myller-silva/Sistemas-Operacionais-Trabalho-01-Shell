#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 100
#define MAX_ARGS 10

void change_directory(const char *path) {
  if (chdir(path) == -1) {
    perror("Erro ao executar cmd");
    exit(EXIT_FAILURE);
  }
}

int main() {
  char cmd[MAX_CMD_LEN];
  char *args[MAX_ARGS];

  while (1) {
    printf("Shell> ");
    fflush(stdout);

    fgets(cmd, MAX_CMD_LEN, stdin);
    cmd[strcspn(cmd, "\n")] = '\0';

    char *token = strtok(cmd, " ");
    int i = 0;
    while (token != NULL && i < MAX_ARGS - 1) {
      args[i] = token;
      token = strtok(NULL, " ");
      i++;
    }
    args[i] = NULL;

    if (strcmp(args[0], "cd") == 0) {
      if (i < 2) {
        printf("Uso: cd <diretório>\n");
      } else {
        change_directory(args[1]);
      }
      continue;
    }

    if (strcmp(args[0], "exit") == 0) {
      printf("Saindo do shell...\n");
      break;
    }

    pid_t pid = fork();

    if (pid < 0) {
      perror("Erro ao criar o processo filho");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      execvp(args[0], args);
      perror("Erro ao executar o comando");
      exit(EXIT_FAILURE);
    } else {
      int status;
      waitpid(pid, &status, 0);
    }
  }

  return 0;
}
