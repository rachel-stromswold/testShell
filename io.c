#include "io.h"

extern void dispatch(Command comm);
extern void dispatchBackground(Command comm);

void freeCommand(Command c){
	//Command c=(*comm);
	free(c.fname);
	for(int i=0; c.argv[i]!=NULL;i++){
		free(c.argv+i);
	}
	free(c.argv);
}

void printCommand(Command c){
	for(int i=0; c.argv[i]!=NULL;i++){
		printf("%s", *(c.argv+i));
	}
}

//returns the number of times that any of the characters in delimiters occurs in the source string src
//both strings MUST be null terminated
int countOccurances(char* src, char* delimiters){
	int ret=0;
	char* i=src;char* j=delimiters;
	for(i=src;(*i)!=0;i++){
		for(j=delimiters;(*j)!=0;j++){
			if((*i)==(*j)){
				ret++;
				break;
			}
		}
	}

	return ret;
}

//returns an easily parsable command from an input string
Command processString(char* input){
	int inputLen=strlen(input);
	Command ret;
	ret.background=0;//by default make the process not run in the background

	if(inputLen<=0){
		//printf("Error, improper input command.");
		ret.fname="";
	}else{
		ret.argc=countOccurances(input, " ")+1;//plus one because of fencepost rule
		if(input[inputLen-1]=='&'){
			ret.background=1;
			ret.argc-=1;
		}

		ret.argv=(char**)malloc((ret.argc+1)*sizeof(char*));

		char* token=strtok(input, " ");int i=0;
		while(token!=NULL){	
			ret.argv[i]=token;
			printf("token %d: %s\n", i, token);
			i++;
			token=strtok(NULL, " ");
		}
		ret.argv[ret.argc]=NULL;
		ret.fname=ret.argv[0];
	}

	return ret;
}

//processes an input string and sends it to be dispatched or executes custom built in commands specifically designed for this shell
//returns 0=normal quit signal
//	  1=continue running (no problems)
//	  2=fatal error (quit)
int execute(char* input){
	printf("%s\n", input);
	Command comm=processString(input);
	printCommand(comm);

	if(strcmp(comm.fname, "quit")==0){
		freeCommand(comm);
		return 0;
	}else{//if it isn't any of the recognized built in commands, then do something else
		if(comm.background){
			dispatchBackground(comm);
		}else{
			dispatch(comm);
		}
	}

	//freeCommand(comm);

	return 1;
}
