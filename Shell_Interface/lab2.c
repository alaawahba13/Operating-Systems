/*
 * AUTHOR : Alaa Wahba 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/stat.h>
#define MAX_LINE 80


int read_command(char command[], char history[]){
	// read command from stdin with size MAX_LINE	
	fgets(command, MAX_LINE +1 , stdin);
	command[strlen(command)-1] = '\0';
	//the history feature 
	if(strcmp(command, "!!") == 0){
		if(strlen(history) !=0){	
			// print the last command if it exists and copy it to the current command
			printf("%s\n", history);
			strcpy(command, history);
		}
		else {
			printf("No previous commands\n");
			return -1;	
			}
	}else if (strcmp(command, "exit") == 0){
		return 0;

	}
	if(strlen(command) > 0){
		// copy the current command into last command 
		strcpy(history, command);
	}
return 1;		
}

int parse_Input(char *args[] , char command[]){
	int count =0 ; 
	// Parse the string command into separate chars using the space delimitor 
	char *token = strtok(command, " ");
	while(token != NULL){
		args[count++] = token;
		token = strtok(NULL, " ");
	}	
	args[count] = token;
	return count;

}

void exec_Child(char *args[], int len){
	pid_t pid;
	int wait_flag =0;
	// Check if child process has to run concurently 
	if(*args[len-1] == '&'){
		wait_flag = 1 ;
		args[len-1] = NULL;
		len--;
	}
	
	pid = fork();
	if(pid ==0 ){ //Child process
	// Check for Redirecting input and output
		if(len >=3){
			if(strcmp(args[len-2], ">") ==0){	// redirect the output of command into a file
				args[len-2] = NULL;
				int fd_out = open(args[len - 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				dup2(fd_out, STDOUT_FILENO);
				close(fd_out);
			}else if(strcmp(args[len-2], "<") ==0){  // redirect the input to a command from a file
				args[len-2] = NULL;
				int fd_in = open(args[len - 1 ] , O_RDONLY);
				dup2(fd_in, STDIN_FILENO);
				close(fd_in);
			} 
			
		}
	   execvp(args[0], args);
	}
	else if (pid > 0)
	{
	 // Parent process
		if(!wait_flag)	
	   		wait(NULL);	
	}
	else
	{ 	
	// didn't fork a process
		printf("\n fork ERROR");
	}

}

//----------------Commuincation via a pipe--------------------

int check_pipe(char *args[], int len){
	for(int i=0; i<len; i++)
		if(strcmp(args[i], "|") ==0)
			return i;
 return -1;
}

void exec_pipe(char *args[] , char *args_pipe[], int flag){
	pid_t pid;
	int fd[2]; 
	pipe(fd);
	
	pid = fork();
	
	if(pid ==0 ){	// Child process 
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		 execvp(args[0], args);
		exit(1);	
	}
	else if(pid >0) {	// Parent process
		pid =fork();
		if(pid ==0 ){	// Child process 
			dup2(fd[0], STDIN_FILENO);
			close(fd[1]);
			close(fd[0]);
			 execvp(args_pipe[0], args_pipe);
			exit(1);	
		}
		else if(pid >0) {	// Parent process
			int status;
			close(fd[0]);
			close(fd[1]);
			if(flag ==0)
				waitpid(pid, &status, 0);
	
		}
	
	}

}

void exec_(char *args[], int len){
	int flag=0;
	int isPipe = check_pipe(args, len);
	if(isPipe == -1 ) {
		exec_Child(args,len);
		return;
	}else{
		char *args_pipe[MAX_LINE /2 +1];
		int i;
		for(i=0; i<len-isPipe-1; i++)
			args_pipe[i] = strdup(args[isPipe +i +1]);
		args_pipe[i] = NULL;	
		if(*args_pipe[i-1] == '&'){
			flag =1;
			args_pipe[i-1] = NULL;
		}
		args[isPipe]= NULL;
		exec_pipe(args, args_pipe, flag);	
	}






}

int main(void){
	char *args[MAX_LINE/2 +1];
	char command[MAX_LINE];
	char history[MAX_LINE];
	int should_run =1 ;
	

	while(should_run){
		int len;
		printf("osh>");
		fflush(stdout);
		int ret_value = read_command(command, history);
		if(ret_value == 1){
			// excute the command 
			len = parse_Input(args, command);
			exec_(args,len);
		
		}
		else if(ret_value ==0)	
			// exit
			should_run =0; 
	 
	}
return 0;
}






















