#include "io.h"

extern void listJobs(int num);
extern void dispatch(Command comm, int* p_fds);
extern void dispatchBackground(Command comm);
extern void moveToForeground(int pid);

void printCommand(Command c){
	printf("argc: %d, ", c.argc);
	for(int i=0; c.argv[i]!=NULL;i++){
		printf("%s,", *(c.argv+i));
	}printf("\n");
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

//fills data within a single command, mostly used to simplify the internal structure of the processString command, this will NOT properly handle pipes, for that use processString()
//this does nothing to pipeDest
void fillSingleCommand(char* command, Command* comm){
	int commandLen=strlen(command);
	char* commandDup;
	if(command[commandLen-1]==' '){//if there is a space at the end of the string then we should eliminate it before processing
		commandLen-=1;
	}
	if(command[0]==' '){
		commandLen-=1;
		commandDup=(char*)malloc(sizeof(char)*(commandLen+1));
		strncpy(commandDup, command+1, commandLen);
	}else{
		commandDup=(char*)malloc(sizeof(char)*(commandLen+1));
		strncpy(commandDup, command, commandLen);
	}
	commandDup[commandLen]=0;//ensure that our string is null terminated

	(*comm).argc=countOccurances(commandDup, " ")+1;//plus one because of fencepost rule
	if(commandDup[commandLen-1]=='&'){
		(*comm).background=1;
		(*comm).argc-=1;
	}

	(*comm).argv=(char**)malloc(((*comm).argc+1)*sizeof(char*));

	char* token=strtok(commandDup, " ");int i=0;
	while(token!=NULL){
		(*comm).argv[i]=token;
		i++;
		token=strtok(NULL, " ");
	}
	(*comm).argv[(*comm).argc]=NULL;
	(*comm).fname=(*comm).argv[0];

	//don't free because the command internally uses each of these individual pieces
	//free(commandDup);
}

//returns an easily parsable command from an input string
Command processString(char* input){
	int inputLen=strlen(input);
	Command ret;
	Command* bottom=&ret;
	ret.background=0;//by default make the process not run in the background

	if(inputLen<=0){
		//printf("Error, improper input command.");
		ret.fname="";
	}else{
		//go through every command divided by a pipe and assign the proper command
		char* commands=strtok(input, "|");	
		while(commands!=NULL){
			fillSingleCommand(commands, bottom);
			
			commands=strtok(NULL, "|");
			if(commands!=NULL){//if there are any more commands in the pipe stream, then make sure we get it on the next pass
				(*bottom).pipeDest=(Command*)malloc(sizeof(Command));
				bottom=(*bottom).pipeDest;
			}else{//otherwise ensure that the current command does not have a child
				(*bottom).pipeDest=NULL;
			}
		}
	}

	return ret;
}

//processes an input string and sends it to be dispatched or executes custom built in commands specifically designed for this shell
//returns 0=normal quit signal
//	  1=continue running (no problems)
//	  2=fatal error (quit)
int execute(char* input){
	input[strlen(input)-1]=0;
	//printf("%s\n", input);
	Command comm=processString(input);
	printCommand(comm);

	if(strcmp(comm.fname, "quit")==0){
		//freeCommand(comm);
		return 0;
	}else if(strcmp(comm.fname, "listjobs")==0){
		if(comm.argc==1)
			listJobs(-1);
		else
			listJobs(atoi(comm.argv[1]));
	}else if(strcmp(comm.fname, "foreground")==0 || strcmp(comm.fname, "fg")==0){
		if(comm.argc==1)
			printf("Error, you must supply 1 argument (pid) for this command, for example: \"fg 2093\".");
		else
			moveToForeground(atoi(comm.argv[1]));
	}else{//if it isn't any of the recognized built in commands, then do something else
		if(comm.background){
			dispatchBackground(comm);
		}else{
			dispatch(comm, NULL);
		}
	}

	//because of the way strtok works, we don't have to worry about memory leaks, the memory that holds the strings will be overwritten on the next call

	return 1;
}
