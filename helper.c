#ifndef HELPER_C_
#define HELPER_C_

#include <stdio.h>
#include <time.h>
#include <sys/wait.h>

// Copied from Assignment-1 PDF
void sleepForMs(long long delayInMs)
{
	const long long NS_PER_MS = 1000 * 1000;
	const long long NS_PER_SECOND = 1000000000;

	long long delayNs = delayInMs * NS_PER_MS;
	int seconds = delayNs / NS_PER_SECOND;
	int nanoseconds = delayNs % NS_PER_SECOND;

	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *)NULL);
}

// Copied from Assignment-1 PDF
void runCommand(char *command, char *output)
{
	// Execute the shell command (output into pipe)
	FILE *pipe = popen(command, "r");

	char buffer[1024];
	while (!feof(pipe) && !ferror(pipe))
	{
		if (fgets(buffer, sizeof(buffer), pipe) == NULL)
			break;
		// printf("--> %s", buffer); // Uncomment for debugging
	}

	// Get the exit code from the pipe; non-zero is an error:
	int exitCode = WEXITSTATUS(pclose(pipe));
	if (exitCode != 0)
	{
		perror("Unable to execute command:");
		printf("    command:    %s\n", command);
		printf("    exit code:  %d\n", exitCode);
	}

	int wc = sprintf(output, "%s", buffer);
	output[wc - 1] = 0;
}
#endif
