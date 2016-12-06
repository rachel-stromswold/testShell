#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_READ_SIZE 100

typedef struct CommandStruct{
	int argc;//the number of arguments
	char** argv;
	char* fname;//the name of the command to be executed, should be equivalent to argv[0]
	char background;//1 if the process should be placed in background, 0 otherwise
	struct CommandStruct* pipeDest;//if we are piping this command, what next command should recieve the piped output
} Command;

#endif
