#ifndef DISPATCH_H
#define DISPATCH_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#include "io.h"

//This stores the information about one process, processes are stored together in a linked list
typedef struct ProcessStruct{
	pid_t procId;
	int status;
	int number;
	char* name;
	struct ProcessStruct* nextProcess;
	struct ProcessStruct* prevProcess;
} Process;

#endif
