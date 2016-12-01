#include "dispatch.h"

Process* first=NULL;Process* last=NULL;

//executes the command indicated by the command structure comm
void dispatch(Command comm){
	pid_t procId;
	procId=fork();
	
	if(procId<0){
		printf("An error has occured forking the process.\n");
	}else if(procId==0){
		if(execvp(comm.fname, comm.argv)==-1)
			printf("An error occured executing the process specified, %s.\n", comm.fname);

		exit(2);
	}else if(procId>0){
		int status=0;
		waitpid(procId, &status, 0);
	}
}

//executes the command indicated by the command structure comm in the background (the user may run other processes while this command finishes execution)
void dispatchBackground(Command comm){
	pid_t procId;
	procId=fork();
	
	if(procId<0){
		printf("An error has occured forking the process.\n");
	}else if(procId==0){
		if(execvp(comm.fname, comm.argv)==-1)
			printf("An error occured executing the process specified.\n");

		exit(2);
	}else if(procId>0){
		(*last).nextProcess=(Process*)malloc(sizeof(Process));
		char* name=(char*)malloc((strlen(comm.fname)+1)*sizeof(char));
		strcpy(name, comm.fname);
		(*((*last).nextProcess)).name=name;
		(*((*last).nextProcess)).status=0;
		(*((*last).nextProcess)).procId=procId;
		(*((*last).nextProcess)).nextProcess=NULL;
		last=(*last).nextProcess;
	}
}
