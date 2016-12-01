#include "dispatch.h"

Process* first=NULL;Process* last=NULL;

//lists the most recent num jobs sent through the shell background. if num=-1 then all jobs are listed
void listJobs(int num){
	if(num<0){
		Process* proc=first;
		while(proc!=NULL){
			printf("Command%d with PID %d: STATUS %d\n", (*proc).number, (*proc).procId, (*proc).status);
		}
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
		if(execvp(comm.fname, comm.argv)==-1)
			printf("An error occured executing the process specified.\n");

		exit(2);
	}else if(procId>0){
		Process np;
		//if we haven't already created the first background job, then we will make this the first background job
		if(last==NULL || first==NULL){
			first=(Process*)malloc(sizeof(Process));
			np=(*first);//this is just a convenient identifier for the new process
			last=first;
		//otherwise make sure that the current last job points to the new job
		}else{
			(*last).nextProcess=(Process*)malloc(sizeof(Process));
			np=(*((*last).nextProcess));//this is just a convenient identifier for the new process	
			last=(*last).nextProcess;
		}

		char* name=(char*)malloc((strlen(comm.fname)+1)*sizeof(char));
		strcpy(name, comm.fname);
		np.name=name;
		np.status=0;
		np.procId=procId;
		np.number=(*last).number+1;
		np.nextProcess=NULL;
		np.prevProcess=last;
	}
}
