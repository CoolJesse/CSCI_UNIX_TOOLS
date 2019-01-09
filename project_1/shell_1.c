#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define MAXLINE 200

#define MAXARG 20

extern char **environ;

static void execute3(int, char *[]);
static bool getargs(int *, char *[], int, bool *);
void set(int, char *[]);
void asg(int, char *[]);
void display_status(pid_t, int );

int main(void)
{
	char *argv[MAXARG];
	int argc;
	bool eof;

	while(true)
	{	printf("@ ");
		if(getargs(&argc, argv, MAXARG, &eof) && argc > 0)
		{	if(strchr(argv[0], '=') != NULL)
				asg(argc, argv);
			else if(strcmp(argv[0], "set") == 0)
				set(argc, argv);
			else
				execute3(argc, argv);
			
		}

		if(eof)
			exit(EXIT_SUCCESS);
	}			
}

static void execute3(int argc, char *argv[])
{	
	pid_t pid;
	int status;

	switch(pid = fork())
	{	case -1: /* Parent (error)*/
			printf("Parent error, error number: %d %s\n", errno, strerror(errno));
		case 0: /* child */
			execvp(argv[0], argv);
			printf("Child error, error number: %d %s\n", errno, strerror(errno));
		default: /* parent */
			if(waitpid(pid, &status, 0) != -1);
				display_status(pid, status);
				
			
	}
	return;
}

static bool getargs(int *argcp, char *argv[], int max, bool *eofp)
{
	static char cmd[MAXLINE];
	char *cmdp;
	int i;

	*eofp = false;
	if (fgets(cmd, sizeof(cmd), stdin) == NULL)
	{	if(ferror(stdin))
			fprintf(stderr, "fgets failed with error number %d %s\n", errno, strerror(errno));
		*eofp = true;
		return false;
	}

	if(strchr(cmd, '\n') == NULL)
	{	while(true)
		{	switch(getchar())
			{	case '\n':
					break;
				case EOF:
					if(ferror(stdin))
						fprintf(stderr, "ferror number %d %s\n", errno, strerror(errno));
				default:
					continue;
			}
			break;
		}
		printf("Line too long -- command ignored\n");
		return false;
	}
	cmdp = cmd; 
	for(i = 0; i < max; i++)
	{	if( (argv[i] = strtok(cmdp, "\t\n") ) == NULL )
			break;
		cmdp = NULL; /*tell strtok to keep going*/
	}
	
	if(i >= max)
	{	printf("Too many args -- command ignored\n");
		return false;
	}

	*argcp = 1;
	return true;	
}

void set(int argc, char *argv[])
{
	int i;
	
	if(argc != 1)
		printf("Extra args\n");
	else
		for(i = 0; environ[i] != NULL; i++)
			printf("%s\n", environ[i]);
}

void asg(int argc, char *argv[])
{
	char *name, *val;
	if(argc != 1)
		printf("Extra args\n");
	else
	{	name = strtok(argv[0], "=");
		val = strtok(NULL, ""); /*Get all that's left*/
		if(name == NULL || val == NULL)
			printf("Bad command\n");
		else
			if(setenv(name, val, true) != 0)
				printf("Error setting environment, error number: %d %s\n", errno, strerror(errno));
	}
	return;

}

void display_status(pid_t pid, int status)
{
	if(pid != 0)
		printf("Process %ld: ", (long)pid);
	if(WIFEXITED(status))
		printf("Exit value %d\n", WEXITSTATUS(status));
	else
		if(WCOREDUMP(status))
			printf(" - core dump");
		if(WIFSTOPPED(status))
			printf(" (stopped)");
}
