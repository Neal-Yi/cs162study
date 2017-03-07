#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include "tokenizer.h"

<<<<<<< HEAD
=======
/* Convenience macro to silence compiler warnings about unused function parameters. */
#define unused __attribute__((unused))

>>>>>>> a6ef9f69aaa37bd8797091bd4eb338c15b7a7c2d
/* Whether the shell is connected to an actual terminal or not. */
bool shell_is_interactive;

/* File descriptor for the shell input */
int shell_terminal;

/* Terminal mode settings for the shell */
struct termios shell_tmodes;

/* Process group id for the shell */
pid_t shell_pgid;

<<<<<<< HEAD
pid_t subprocess_pgid;

int cmd_exit(struct tokens *tokens);
int cmd_help(struct tokens *tokens);
int cmd_cd(struct tokens *tokens);
int cmd_pwd(struct tokens *tokens);

void singal_callback(int signum);
=======
int cmd_exit(struct tokens *tokens);
int cmd_help(struct tokens *tokens);

>>>>>>> a6ef9f69aaa37bd8797091bd4eb338c15b7a7c2d
/* Built-in command functions take token array (see parse.h) and return int */
typedef int cmd_fun_t(struct tokens *tokens);

/* Built-in command struct and lookup table */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_exit, "exit", "exit the command shell"},
<<<<<<< HEAD
  {cmd_pwd, "pwd", "show the current working directory"},
  {cmd_cd, "cd", "change working directory to specific path"},
};

/* Prints a helpful description for the given command */
int cmd_help(struct tokens *tokens) {
  for (int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
=======
};

/* Prints a helpful description for the given command */
int cmd_help(unused struct tokens *tokens) {
  for (unsigned int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
>>>>>>> a6ef9f69aaa37bd8797091bd4eb338c15b7a7c2d
    printf("%s - %s\n", cmd_table[i].cmd, cmd_table[i].doc);
  return 1;
}

/* Exits this shell */
<<<<<<< HEAD
int cmd_exit(struct tokens *tokens) {
  exit(0);
}

/* Show current working directory*/
int cmd_pwd(struct tokens *tokens){
  char pwd[1024];
  getcwd(pwd, 1024);
  fprintf(stdout, "%s\n", pwd);
  return 1;
}
/* change working directory */
int cmd_cd(struct tokens *tokens){
 int ret =  chdir(tokens_get_token(tokens, 1)); 
 if(ret<0)
{
	fprintf(stderr, "plesae enter a correct path\n"); 
return -1;
}
   return 1;	
}
/* Looks up the built-in command, if it exists. */
int lookup(char cmd[]) {
  for (int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
=======
int cmd_exit(unused struct tokens *tokens) {
  exit(0);
}

/* Looks up the built-in command, if it exists. */
int lookup(char cmd[]) {
  for (unsigned int i = 0; i < sizeof(cmd_table) / sizeof(fun_desc_t); i++)
>>>>>>> a6ef9f69aaa37bd8797091bd4eb338c15b7a7c2d
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0))
      return i;
  return -1;
}

/* Intialization procedures for this shell */
void init_shell() {
  /* Our shell is connected to standard input. */
  shell_terminal = STDIN_FILENO;

  /* Check if we are running interactively */
  shell_is_interactive = isatty(shell_terminal);

  if (shell_is_interactive) {
    /* If the shell is not currently in the foreground, we must pause the shell until it becomes a
     * foreground process. We use SIGTTIN to pause the shell. When the shell gets moved to the
     * foreground, we'll receive a SIGCONT. */
    while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
      kill(-shell_pgid, SIGTTIN);

    /* Saves the shell's process id */
    shell_pgid = getpid();

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);

    /* Save the current termios to a variable, so it can be restored later. */
    tcgetattr(shell_terminal, &shell_tmodes);
  }
}

<<<<<<< HEAD
void signal_callback(int signum){
	kill(-subprocess_pgid, signum);
}
int main(int argc, char *argv[]) {
=======
int main(unused int argc, unused char *argv[]) {
>>>>>>> a6ef9f69aaa37bd8797091bd4eb338c15b7a7c2d
  init_shell();

  static char line[4096];
  int line_num = 0;

  /* Please only print shell prompts when standard input is not a tty */
  if (shell_is_interactive)
    fprintf(stdout, "%d: ", line_num);

  while (fgets(line, 4096, stdin)) {
    /* Split our line into words. */
    struct tokens *tokens = tokenize(line);

    /* Find which built-in function to run. */
    int fundex = lookup(tokens_get_token(tokens, 0));

    if (fundex >= 0) {
      cmd_table[fundex].fun(tokens);
    } else {
<<<<<<< HEAD
	    char *argv[1024];
	    int token_length = tokens_get_length(tokens);
	    char *lastArgvment = tokens_get_token(tokens, token_length - 1);
	    bool isbackground = (lastArgvment[0] == '&');
	    int cpid = fork();
	    if(cpid== 0){

		    int argc = 0;
		    bool isredir_stdin = false, isredir_stdout = false;
		    for(int i = 0; i < token_length  ; i++){
			    argv[argc] = tokens_get_token(tokens, i );
			    if (argv[argc][0] == '<')
			    {
				    int j =0;
				    while(argv[argc][j] != '\0' && argv[argc][j] != '>')j++;
				    if(argv[argc][j] != '\0'){
					    freopen(&argv[argc][j+1], "wb", stdout);
					    argv[argc][j] = '\0';
					    isredir_stdout = true;
				    }
				    freopen(&argv[argc][1], "rb", stdin);
				    isredir_stdin = true;
			    }else if (argv[argc][0] == '>')
			    {   
				    int j =0;
				    while(argv[argc][j] != '\0' && argv[argc][j] != '<')j++;
				    if(argv[argc][j] != '\0'){
					    freopen(&argv[argc][j+1], "rb", stdin);
					    argv[argc][j] = '\0';
					    isredir_stdin = true;
				    }

				    
				    freopen(&argv[argc][1], "wb", stdout);

				    isredir_stdout = true;
				   
			    }else if(argv[argc][0] != '&')argc ++;

		    }
		    argv[argc] = NULL;
		    char path[1024];
		   if(argv[0][0] != '/' && argv[0][0]!='.' &&argv[0][0]!='~')
		   {

			    char *up_env = getenv("PATH");
			    char *s =up_env;
			    while((*up_env)!=0){
				    if(*up_env == ':')
				    {
					    *up_env = 0;
					    strcpy(path, s);
					    strcat(path,"/");
					    strcat(path,argv[0]);
					    char *tmp = argv[0];
					    argv[0] = path;
					    s = up_env + 1;
					    if(execv(path, argv)<0)
					    {
						    argv[0] = tmp;
					    }else break;
				    }
				    up_env++;
			    }
			    if((*up_env) == 0)
				    fprintf(stderr, "no command was found!\n");
		   }else{
			   if(execv(argv[0], argv) < 0)
				   fprintf(stderr, "no command was found!\n");
		   }
      // subprocess should exit;
		   if(isredir_stdin)fclose(stdin);

		   if(isredir_stdout)fclose(stdout);

		   exit(0); 		    
	    }else if(cpid > 0){
		    subprocess_pgid = cpid;
		    setpgid(subprocess_pgid,subprocess_pgid); 
		    signal(SIGINT, signal_callback);
		    if (isbackground)
		    {
			    kill(-subprocess_pgid, SIGCONT);
			    printf("%d\n", subprocess_pgid);
		    }else{
			    int status;
			    wait(&status);
		    }
	    }
=======
      /* REPLACE this to run commands as programs. */
      fprintf(stdout, "This shell doesn't know how to run programs.\n");
>>>>>>> a6ef9f69aaa37bd8797091bd4eb338c15b7a7c2d
    }

    if (shell_is_interactive)
      /* Please only print shell prompts when standard input is not a tty */
      fprintf(stdout, "%d: ", ++line_num);

    /* Clean up memory */
    tokens_destroy(tokens);
  }

  return 0;
}
