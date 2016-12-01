#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "io.h"

extern int execute(char* input);

int main(){
	int status=1;
	char* input=(char*)malloc(MAX_READ_SIZE*sizeof(char));

	while(status==1){
		printf("cs350sh> ");
		fgets(input, MAX_READ_SIZE, stdin);
		status=execute(input);
	}

	return 0;
}
