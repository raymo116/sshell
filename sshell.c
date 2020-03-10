/**
 * Based off an example program demonstrating UNIX pipes by
 * Silberschatz, Galvin, and Gagne
 *
 * @name: sshell.c
 * @author: Matt Raymond
 * @email: raymo116@mail.chapman.edu
 * @id: 2270559
 * @date: 03/10/2020
 * @version: 1.0
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define READ_END	0
#define WRITE_END	1

/*
* This program will emulate a terminal by creating a child process, taking in
* terminal commands through the parent process, piping them to the child
* process, and executing them via the child process. The user can exit by
* typing "q" or "quit"
*/
int main(void)
{
	// Creates the arrays to hold the IPC
	char write_msg[BUFFER_SIZE];
	char read_msg[BUFFER_SIZE];

	// Exit conditions
	char exitCondition[3] = "q\n";
	char exitCondition2[6] = "quit\n";

	pid_t pid;
	int fd[2];

	// Error handling for pipe creation
	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	//Fork
	pid = fork();

	// Error handling for fork creation
	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	// Splits according to processes
	if (pid > 0) { // Parent
		// Close unneeded pipe openings
		close(fd[READ_END]);

		// terminal name
		printf("osh> ");

		// Continues until exit condition is hit
		while((strcmp(&exitCondition,write_msg) != 0) &&
		(strcmp(&exitCondition2,write_msg) != 0)) {
			// Get input
			fgets(write_msg, BUFFER_SIZE, stdin);

			// Sends information to child process
			write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		}

		// Close the other side of the pipe
		close(fd[WRITE_END]);
	}
	else { // Child
		// Close unnecessary end of pipe
		close(fd[WRITE_END]);

		// Continues until exit condition is hit
		while(1) {
			// Get input
			read(fd[READ_END], read_msg, BUFFER_SIZE);

			if((strcmp(&exitCondition,read_msg) != 0) &&
			(strcmp(&exitCondition2,read_msg) != 0)) {
				// Executes the command
				system(read_msg);

				// prints the terminal name
				printf("osh> ");
				// Necessary if not using new line
				fflush(stdout);
			}
			else {
				break;
			}
		}

		// Close other side of pipe
		close(fd[READ_END]);
	}

	return 0;
}
