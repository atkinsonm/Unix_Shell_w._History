// Michael Meluso
// Programming Project - UNIX Shell and History Feature
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
char *args[MAX_LINE/2 + 1]; /* command line arguments */
int should_run = 1; /* flag to determine when to exit program */
int status;
int pid;
char *current; /* current command being scanned */
char buf[MAX_LINE + 1]; /* command buffer */
int i = 0; // string/history iterator
int ac = 0; /* command counter */
int hcount; /* command decrementer for printing */
char *hist[10][10]; /* command history */
int j = 0; /* extra counter */

for (j; j <= MAX_LINE/2+1; j++) { args[j] = (char *) malloc(10); }

	while (should_run) {
		i = 0; // reset iterator
		printf("osh> ");
		fflush(stdout);
		
		// read entire line
		fgets(buf,MAX_LINE, stdin);
		strcat (buf, "\0"); // add end of string character
 		
		current = strtok(buf, " ");
		while (current != NULL) {
			strcpy(args[i], current);
			i++;
			current = strtok(NULL, " ");
		}

		for (i = 0; args[i] != NULL; i++) {
			while ((args[i])[j] != '\0') { j++; }
			(args[i])[j-1] = '\0';
			(args[i])[j] = 0;
		}

		for (i = 0; args[i] != NULL; i++) {
			hist[ac%10][i] = args[i];
		}

		// exit command?
		if ((args[0])[0] == 'e') {
			if ((args[0])[1] == 'x') {
				if ((args[0])[2] == 'i') {
					if ((args[0])[3] == 't') { should_run = 0; }
				}
			} 
		}
		
		else if ((args[0])[0] == 'h') {
		if ((args[0])[1] == 'i') {
		if ((args[0])[2] == 's') {
		if ((args[0])[3] == 't') {
		if ((args[0])[4] == 'o') {
		if ((args[0])[5] == 'r') {
		if ((args[0])[6] == 'y') { 
			if (ac == 0) {
				printf("\nNo commands in history.\n");	
			}
			else {
			i = ac % 10; // how many commands are in history
			hcount = ac;
			printf("\n");

			for (i; i > 0; i--) {
				printf(hcount + *(hist[i]));
				hcount--;
			}
			}
		}}}}}}}

		else if ((args[0])[0] == '!') {
			if ((args[0])[1] == '!') {
				if (ac == 0) {
					printf("No commands in history.\n");	
				}
				else {
					pid = fork();
					if (pid < 0) {// error
						perror("Fork failed");
						exit(1); 
					}

					else if (pid == 0) { // child process
						execvp(hist[ac%10][0], hist[ac%10]);
					}

					else if (pid > 0) { // parent process
						// wait?
						i = 0; j = 0;
						while (args[i] != 0) {
							while ((args[i])[j] != '\0') {
								if ((args[0])[i] == '&') { wait(&status); }
							}
						}
					}
				}
			}
			else {
				scanf("%c", j);
				pid = fork();
				if (pid < 0) {// error
					perror("Fork failed");
					exit(1); 
				}

				else if (pid == 0) { // child process
					execvp(hist[j][0], hist[ac%10]);
				}

				else if (pid > 0) { // parent process
					// wait?
					i = 0; j = 0;
					while (args[i] != 0) {
						while ((args[i])[j] != '\0') {
							if ((args[0])[i] == '&') { wait(&status); }
						}
					}
				}	
			}
		}
		
		else {
		// create child process
		pid = fork(); 
		
		if (pid < 0) {// error
			perror("Fork failed");
			exit(1); 
		}

		else if (pid == 0) { // child process
			execvp(args[0], args);
		}

		else if (pid > 0) { // parent process
			// wait?
			i = 0;
			//while (*args[i] != "\0")
			wait(&status);
		}		

		ac++; // increment command counter
		}
	}
}
