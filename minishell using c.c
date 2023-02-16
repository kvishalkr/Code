/* This program is for a minishell in linux to run
some of the basic commands used in it.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ  0
#define WRITE 1


/* The array below will hold the arguments: args[0] is the command. */
char* args[512];
pid_t pid;
int command_pipe[2];  //for pipe handling..


 int command(int input, int first, int last)
{
	int pipettes[2];

	// Invoke pipe
	pipe( pipettes );
	pid = fork();

	 //	STDIN -->  --> STDOUT


	if (pid == 0) {
		if (first == 1 && last == 0 && input == 0) {
			// First command
			dup2( pipettes[WRITE], STDOUT_FILENO );
		} else if (first == 0 && last == 0 && input != 0) {
			// Middle command
			dup2(input, STDIN_FILENO);
			dup2(pipettes[WRITE], STDOUT_FILENO);
		} else {
			// Last command
			dup2( input, STDIN_FILENO );
		}

		if (execvp( args[0], args) == -1)
			exit(EXIT_FAILURE); // If child fails
	}

	if (input != 0)
		close(input);

	close(pipettes[WRITE]);

	// If it's the last command, nothing more needs to be read
	if (last == 1)
		close(pipettes[READ]);

	return pipettes[READ];
}

//Final cleanup , "wait" for the process to terminate

void cleanup(int n)
                                       // n is the no of times command was invoked.
{
	int i;
	for (i = 0; i < n; ++i)
		wait(NULL);
}

void renames(char* ad1, char* par)
{

		if (strcmp(ad1, par)==0)
		{
			printf("The source file %s and the destination file %s can not be the same.\n", ad1, par);
		}
		else
		{
			if (access(ad1, F_OK)<0)                                                                     // renaming a file.
			{
				printf("The source file %s does not exist.\n", ad1);
			}
			else
			{
				if (rename(ad1, par)<0)
				{
					printf("cannot rename file from %s to file %s\n", ad1, par);
				}
			}
		}
		return;
}


void changedir(char* adt)
{

		if (chdir(adt)<0)
		{
			if (errno==ENOTDIR||errno==ENOENT)
			{                                                           // This changes the directory.
				printf("The directory %s does not exist\n", adt);
			}
		}

	return;
}














 int run(char* cmd, int input, int first, int last);
 char line[1000];
 int n = 0;                                          // refers to no of calls to the command.

int main()
{

	while (1) {
		//Print the command prompt
		printf("Minishell:~$> ");


		                                                          // read the command line given.
		if (!fgets(line, 1000, stdin))
			return 0;

		int input = 0;
		int first = 1;

		char* cmd = line;
		char* next = strchr(cmd, '|'); // finding first pipe


		while (next != NULL) {
			                                          // next pointing to pipe | .
			*next = '\0';
			input = run(cmd, input, first, 0);



		if(!(strcmp(args[0], "ren"))){
            renames(args[1],args[2]);
		}

        if(!(strcmp(args[0],"cd"))){
            changedir(args[1]);

            if(!(strcmp(args[1],".."))){
              printf("Current directory is /home/User\n");
            }
            else{
                printf("Current directory is /home/User/%s\n",args[1]);
            }
        }


			cmd = next + 1;
			next = strchr(cmd, '|');                        // finding next pipe
			first = 0;
		}
		input = run(cmd, input, first, 1);


		if(!(strcmp(args[0], "ren"))){
            renames(args[1],args[2]);
		}

		if(!(strcmp(args[0],"cd"))){
            changedir(args[1]);

             if(!(strcmp(args[1],".."))){
              printf("Current directory is /home/User\n");                                   //printing the directory .
            }
            else{
                printf("Current directory is /home/User/%s\n",args[1]);
            }

        }



		cleanup(n);
		n = 0;
	}
	return 0;
}

void split(char* cmd);

int run(char* cmd, int input, int first, int last)
{
	split(cmd);
	if (args[0] != NULL) {
		if (strcmp(args[0], "exit") == 0)
			exit(0);
		n += 1;
		return command(input, first, last);
	}
	return 0;
}

char* whitespace(char* s)
{
	while (isspace(*s)) ++s;
	return s;
}

void split(char* cmd)
{
	cmd = whitespace(cmd);
	char* next = strchr(cmd, ' ');
	int i = 0;

	while(next != NULL) {
		next[0] = '\0';
		args[i] = cmd;
		++i;
		cmd = whitespace(next + 1);
		next = strchr(cmd, ' ');
	}

	if (cmd[0] != '\0') {
		args[i] = cmd;
		next = strchr(cmd, '\n');
		next[0] = '\0';
		++i;
	}

	args[i] = NULL;
}




