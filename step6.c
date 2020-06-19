#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#define LSH_TOK_BUFSIZE 64
#define DELIM " \t\r\n\a"




char *readline(void)
{
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
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



int lsh_exit(char **args)
{
  return 0;
}

int redirect(char *line)
{
    if(strstr(line,"<")!=NULL)
    {
       if(strstr(line,"<<")!=NULL)
       {
           return 2;
       }
        return 1; 
    }
    else if(strstr(line,">")!=NULL)
    {
        if(strstr(line,">>")!=NULL)
        {
            return 4;
        }
        return 3;
    }
    else
    {
        return -1;
    }   
}

char ** redirects(char *line)//to split the line into arguments to be executed and the file name
{
    int bufsize = LSH_TOK_BUFSIZE, p = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *tok;
  #define DELIMR "<>"

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  tok = strtok(line, DELIMR);
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

    tok = strtok(NULL, DELIMR);
  }
  tokens[p] = NULL;
  return tokens;
}

void output(char **args,char *fn,int t)
{
    int out = open(fn, O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == out) { perror("opening cout.log"); return 255; }

    int err = open("cerr.log", O_RDWR|O_CREAT|O_APPEND, 0600);
    if (-1 == err) { perror("opening cerr.log"); return 255; }

    int save_out = dup(fileno(stdout));
    int save_err = dup(fileno(stderr));

    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr") ; return 255; }

    // puts("doing an ls or something now");
    if (execvp(args[0], args) == -1) {
      perror("rsh");
    }
    exit(EXIT_FAILURE);
    fflush(stdout); close(out);
    fflush(stderr); close(err);

    dup2(save_out, fileno(stdout));
    dup2(save_err, fileno(stderr));

    close(save_out);
    close(save_err);
    
}

void input(char **args,char *fn,int t)
{
    int in=open(fn, O_RDONLY);
    if (in == -1)
    {
      perror("opening cout.log");
     }

    if (-1 == dup2(in, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }
    //if (-1 == dup2(err, fileno(stderr))) { perror("cannot redirect stderr") ; return 255; }
    if ((execvp(args[0], args) == -1)) {
      perror("rsh");
    }
 

   /* cat < infile */

   
}


int start(char **args,char *fn, int t)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if(t==3||t==4)
    {
        output(args,fn,t);
    }
    else if(t==1)
    {
        input(args,fn,t);
    }
    else if (execvp(args[0], args) == -1) 
    {
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

int execute(char **args,char *fn,int t)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  if(strncmp(args[0],"cd",strlen("cd"))==0)
  {return ecd(args);}
  
  

  return start(args,fn,t);
}








void lsh_loop(void)
{
  char *line;//to store what to execute before tokenising
  char **args;// to store the instructions after tokenising
  char **tempr;//to store the string tokenised with<,>
  char *fn; //to store the location
  //char *cmd="cmd";
  int status=1;
  int t;

  while (status) {
    char s[100];
    printf("cmd %s~",getcwd(s, 100));
    line = readline();
    t=redirect(line); // checking if there is < or > in the command
    if(t==-1)
    {
        args = split(line);
        //printf("%d",redirect(line));
    }
    else
    {
        
        tempr=redirects(line);
        args=split(tempr[0]);
        fn=tempr[1];
        printf("%d \n",t);
        printf("%s \n",tempr[1]);
    }
    
    
    //args = split(line);
    status = execute(args,fn,t);

    free(line);
    free(args);
  } //while (status);
}



int main(int argc, char **argv)
{
  lsh_loop();
  return EXIT_SUCCESS;
}

