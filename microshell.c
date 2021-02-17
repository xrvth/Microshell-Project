#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

int externalexecution(char* command1, char** arguments)
{
  pid_t pid = fork();

  if (pid == 0)
    {
      execv(command1, arguments);
      exit(127);
    }
  else
    {
      waitpid(pid, 0, 0);
    }

  return 0;
}

int doesthefileexist(const char* file)
{
  struct stat status;
  int exists = stat(file, &status);

  if(exists == 0)
    return 1;
  else
    return 0;
} /*knowledge comes from: https://www.zentut.com/c-tutorial/c-file-exists/ */

void cyan ()
{
  printf("\033[1;36m");
}

void yellow ()
{
  printf("\033[1;33m");
}

void magenta ()
{
  printf("\033[1;35m");
}

void resetcolor ()
{
  printf("\033[0m");
} /*knowledge comes from: http://web.theurbanpenguin.com/adding-color-to-your-output-from-c/ */

int main(int argc,char** argv)
{
  char command_base[100], command[100], user_input[100], history[100][100];
  char cwd[PATH_MAX];
  int helper, i = 0, j, k, history_length[100], exists;

  magenta();
  printf("[");
  cyan();
  printf("%s", getcwd(cwd, sizeof(cwd)));
  magenta();
  printf("]");
  yellow();
  printf(" $ ");
  resetcolor();
  printf("Hello! This is Julia's microshell. Please input some command! :) \n");

  do{
    helper = -1;

    magenta();
    printf("[");
    cyan();
    printf("%s", getcwd(cwd, sizeof(cwd)));
    magenta();
    printf("]");
    yellow();
    printf(" $ ");
    resetcolor();
    fgets(command_base, sizeof(command_base), stdin);

    if (strncmp(command_base, "history", 7))
      {
        for(j = 0 ; j < strlen(command_base); j++)
          {
            history[i][j] = command_base[j];
          }
        history_length[i] = strlen(command_base);
        i++;
      }

    sscanf(command_base, "%s %s", command, user_input);

    if(!strncmp(command, "cd", 2))
      {
        helper = 0;
      }
    if(!strncmp(command, "./", 2))
      {
        exists = doesthefileexist(command);
        if(exists)
        {
          helper = 1;
        }
      }
    if(!strncmp(command, "help", 4))
      {
        helper = 2;
      }
    if(!strncmp(command, "history", 7))
      {
        helper = 3;
      }
    if(!strncmp(command, "clean", 5))
      {
        helper = 4;
      }

    switch(helper)
      {
      case 0:
        if(!strncmp(user_input, "", 1))
          {
            chdir("~");break;
          }
        else
          {
            chdir(user_input);break;
          }

      case 1:
        {
          externalexecution(command, argv);
          printf("\n");break;
        }

      case 2:
        printf("cd - changes the current working directory \n./name_of_a_program - opens an external program \nhistory - shows the history of previously typed commands \nclean - cleans the history \nhelp - shows all commands and their use \nexit - exits the microshell \nJulia Ziemba © 2020 \n");break;

      case 3:
        for (j = 0; j < i; j++)
          {
            for (k = 0; k < history_length[j] ; k++)
              {
                printf("%c", history[j][k]);
              }
          }
        break;

      case 4:
        memset(history, 0, sizeof(history));
        memset(history_length, 0, sizeof(history_length));
        i = 0;
        break;

      default:
        if(!strncmp(command, "exit", 4))
          {
            break;
          }
        else
          {
            magenta();
            printf("[");
            cyan();
            printf("%s", getcwd(cwd, sizeof(cwd)));
            magenta();
            printf("]");
            yellow();
            printf(" $ ");
            resetcolor();
            printf("Sorry! But I cannot execute this command :(\n");break;
          }
      }

      }while(strncmp(command, "exit", 4));

}
