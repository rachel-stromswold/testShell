#include "dispatch.h"

Process* first=NULL;Process* last=NULL;

//lists the most recent num jobs sent through the shell background. if num=-1 then all jobs are listed
void listJobs(int num){
	if(num<0){
		Process* proc=first;
		while(proc!=NULL){
			printf("Command%d with PID %d: STATUS %d\n", (*proc).number, (*proc).procId, (*proc).status);
			proc=(*proc).nextProcess;
		}
	}else{
		Process* proc=last;
		while(num>0 && proc!=NULL){
			printf("Command%d with PID %d: STATUS %d\n", (*proc).number, (*proc).procId, (*proc).status);
			proc=(*proc).prevProcess;
			num--;
		}
	}
}

void moveToForeground(int pid){
	Process* proc=first;
	char validPid=0;//0 indicates that the input pid is not owned by the shell
	while(proc!=NULL){
		if((*proc).procId==pid){
			validPid=1;
		}
		proc=(*proc).nextProcess;
	}

	if(validPid){
		int status=0;
		waitpid(pid, &status, 0);
	}
}

//executes the command indicated by the command structure comm
void dispatch(Command comm){
	pid_t procId;
	procId=fork();
	
	if(procId<0){
		printf("An error has occured forking the process.\n");
	}else if(procId==0){
		if(execvp(comm.fname, comm.argv)==-1)
			printf("An error occured executing the process specified, %s, argc: %i.\n", comm.fname, comm.argc);

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
		fclose(stdout);//close the normal stdout
		fclose(stderr);
		if(execvp(comm.fname, comm.argv)==-1)
			printf("An error occured executing the process specified.\n");

		exit(2);
	}else if(procId>0){
		//if we haven't already created the first background job, then we will make this the first background job
		if(last==NULL || first==NULL){
			first=(Process*)malloc(sizeof(Process));	
			last=first;
			char* name=(char*)malloc((strlen(comm.fname)+1)*sizeof(char));
			strcpy(name, comm.fname);
			(*first).name=name;
			(*first).status=0;
			(*first).procId=procId;
			(*first).number=(*last).number+1;
			(*first).nextProcess=NULL;
			(*first).prevProcess=last;
		//otherwise make sure that the current last job points to the new job
		}else{
			(*last).nextProcess=(Process*)malloc(sizeof(Process));
			(*(*last).nextProcess).prevProcess=last;
			last=(*last).nextProcess;
			char* name=(char*)malloc((strlen(comm.fname)+1)*sizeof(char));
			strcpy(name, comm.fname);
			(*last).name=name;
			(*last).status=0;
			(*last).procId=procId;
			(*last).number=(*last).number+1;
			(*last).nextProcess=NULL;	
		}
	}
}
