/*
 * Author : Alaa Wahba
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv){
 int num = atoi(argv[1]);
  pid_t pid;
  pid = fork();
 
  if(pid == 0){
  	// Child process 
  	printf("Child process\n");

  	if(num < 0 )
  		{
  		printf("Error negative number!\n");
  		}
  	else{
  		printf("%d" , num);
  		while(num != 1){
  			if(num %2 == 0 ) // even
  			{
  				num = num/2;
  			}
  			else{
  				num = 3*num +1 ;
  			}
  			printf(",  %d" , num);
  		}
  		printf("\n");
  	
  	}
    
  }
  else{
  	wait(NULL);
  	printf("Parent process\n");
  } 
   
  return 0;
}


