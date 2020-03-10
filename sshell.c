/**
 * Example program demonstrating UNIX pipes.
 *
 * Figures 3.25 & 3.26
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

int main(void)
{
	char write_msg[BUFFER_SIZE];
	char read_msg[BUFFER_SIZE];
	char exitCondition1[2] = "q\n";
	char exitCondition2[2] = "quit\n";
	pid_t pid;
	int fd[2];

	if (pipe(fd) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if (pid > 0) {
		close(fd[READ_END]);
		printf("osh> ");

		while(strcmp(&exitCondition1,write_msg) != 0 &&
	          strcmp(&exitCondition2,write_msg) != 0) {

			fgets(write_msg, BUFFER_SIZE, stdin);
			write(fd[WRITE_END], write_msg, strlen(write_msg)+1);

		}

		close(fd[WRITE_END]);
	}
	else {
		close(fd[WRITE_END]);

		while (1) {
			read(fd[READ_END], read_msg, BUFFER_SIZE);

			if(strcmp(&exitCondition1,read_msg) == 0
		       strcmp(&exitCondition2,read_msg) == 0) {
				break;
			}

			system(read_msg);
			printf("osh> ");
			fflush(stdout);
		}

		close(fd[READ_END]);
	}

	return 0;
}
