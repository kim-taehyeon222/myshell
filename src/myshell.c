#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ls_command.h"

#define BUF_SIZE 1024
#define COUNT_SIZE 16

#define MAX_LINE 80
#define MAX_ARGS 10

int main() {
  
  char input[MAX_LINE];
  char* token;
  char* argv[MAX_ARGS];

  while (1) {

    printf("myshell> ");
    fgets(input, MAX_LINE, stdin);
    
    int i = 0;
    token = strtok(input, " \n\t");
    while (token != NULL) {
      argv[i++] = token;
      token = strtok(NULL, " \n\t");
    }
    argv[i] = NULL;

    if (argv[0] == NULL) { // empty command
      continue;
    }

    if (strcmp(argv[0], "exit") == 0) { // exit
      printf("Goodbye\n");
      exit(0);

    } else if (strcmp(argv[0], "cd") == 0) { // cd
      chdir(argv[1]);

    } else if (strcmp(argv[0], "pwd") == 0) { // pwd
      getcwd(input, MAX_LINE);
      printf("%s\n", input);
    } else if (strcmp(argv[0], "ls") == 0) { // ls
      my_ls();
    } else if (strcmp(argv[0], "cat") == 0) { // cat
      int fd;
      int length;
      char buf[BUF_SIZE];

      fd = open(argv[1], O_RDONLY);
      while ((length = read(fd, buf, COUNT_SIZE)) > 0) {
        write(1, buf, length);
      }
      close(fd);
    } else {
      //실행파일 access
      if (access(argv[0], X_OK) == 0) { // execv
        printf("execute %s\n", argv[0]);
        pid_t pid;
        int status;
        pid = fork();

        if (pid == 0) {
          execv(argv[0], argv);
        } else {
          wait(&status);
        }

      } else {
        printf("command not found: %s\n", argv[0]);
      }
    }
  }

  return 0;
}