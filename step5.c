#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LSH_TOK_BUFSIZE 64
#define DELIM " \t\r\n\a"


int ecd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */


/*
  Builtin function implementations.
*/
int ecd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, " expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
    
  }
  return 1;
}



int eexit(char **args)
{
  return 0;
}


int start(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("execution error");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("error");
  } else {
    // Parent process
    wait(NULL);
  }

  return 1;
}

int execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  if(strncmp(args[0],"cd",strlen("cd"))==0)
  {return ecd(args);}
  /*for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }*/
  if(strncmp(args[0],"exit",strlen("exit"))==0)
  {return eexit(args);}

  return start(args);
}



char **split(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, p = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *tok;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  tok = strtok(line, DELIM);
  while (tok != NULL) {
    tokens[p] = tok;
    p++;

    if (p >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    tok = strtok(NULL, DELIM);
  }
  tokens[p] = NULL;
  return tokens;
}


char *readline(void)
{
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}


void lsh_loop(void)
{
  char *line;
  char **args;
  //char *cmd="cmd";
  int status=1;

  while (status) {
    char s[100];
    printf("cmd %s~",getcwd(s, 100));
    line = readline();
    args = split(line);
    //if(strncmp(args[0],"cd",strlen("cd"))==0)
    
    status = execute(args);
    

    free(line);
    free(args);
  } //while (status);
}



int main(int argc, char **argv)
{
  lsh_loop();
  return EXIT_SUCCESS;
}

