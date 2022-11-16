/*
 * builtin.c : check for shell built-in commands
 * structure of file is
 * 1. definition of builtin functions
 * 2. lookup-table
 * 3. definition of is_builtin and do_builtin
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include "shell.h"

/****************************************************************************/
/* builtin function definitions                                             */
/****************************************************************************/

/* "builtin" command tells whether a command is builtin or not. */
static void bi_builtin(char ** argv) {
  if (argv[1])
     printf("%s is %s a built in feature\n", argv[1], is_builtin(argv[1]) ? "" : "not");  
}


/* "cd" command.  Why does this have to be a shell builtin command? */
static void bi_cd(char **argv) {
   char *path = argv[1] ? argv[1] : getenv("HOME") ;
   if (chdir(path) == -1)
      perror("cd");
}


/* "echo" command.  Does not print final <CR> if "-n" encountered. */
static void bi_echo(char **argv) {
  int nlflag = 1;	/* Cleared upon finding "-n" in argv[1]. */
  int i;

  if ((argv[1]) && (!strcmp(argv[1], "-n"))) {
    nlflag = 0;
    argv++;
  }

  for (i = 1; argv[i] != NULL; i++) {
    fputs(argv[i], stdout);
    putchar(' ');
  }

  if (nlflag)
    putchar('\n');
}


/* "hostname" command. */
static void bi_hostname(char ** argv) {
    static struct utsname my_info;
    uname(&my_info);
    puts(my_info.nodename); 
}


/* "id" command shows user and group of this process. */
static void bi_id(char ** argv) {
   int id = getuid();
   struct passwd *pw_info = getpwuid(id);
   struct group *grp_info = getgrgid(pw_info->pw_gid);
   printf("Userid = %d (%s), Groupid = %d (%s)\n",id,pw_info->pw_name,pw_info->pw_gid,grp_info->gr_name); 
}


/* "pwd" command. */
static void bi_pwd(char ** argv) {
    static char buf[4096];
    printf("%s\n",getcwd(buf,4096));
}


/* quit/exit/logout/bye command. */
static void bi_quit(char **argv) {
  exit(0);
}


/****************************************************************************/
/* lookup table                                                             */
/****************************************************************************/

static struct cmd {
  char * keyword;		/* When this field is argv[0] ... */
  void (* do_it)(char **);	/* ... this function is executed. */
} inbuilts[] = {
  { "builtin", 	bi_builtin},	/* List of (argv[0], function) pairs. */
  { "cd", bi_cd },
  { "hostname", bi_hostname },
  { "pwd", bi_pwd },
  { "id", bi_id },
  { "echo", 	bi_echo},
  { "quit",	bi_quit},
  { "exit",	bi_quit},
  { "bye",	bi_quit},
  { "logout",	bi_quit},
  {  NULL,	NULL}		/* NULL terminated. */
};

/****************************************************************************/
/* is_builtin and do_builtin                                                */
/****************************************************************************/

static struct cmd * this; /* close coupling between is_builtin & do_builtin */

/* Check to see if command is in the inbuilts table above.
Hold handle to it if it is. */
int is_builtin(char *cmd) {
  struct cmd *tableCommand;

  for (tableCommand = inbuilts ; tableCommand->keyword != NULL; tableCommand++)
    if (strcmp(tableCommand->keyword,cmd) == 0) {
      this = tableCommand;
      return 1;
    }
  return 0;
}


/* Execute the function corresponding to the builtin cmd found by is_builtin. */
int do_builtin(char **argv) {
  this->do_it(argv);
}
