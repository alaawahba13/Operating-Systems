/*
 * Author : Alaa Wahba
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
int main(int argc, char** argv){

const int SIZE  =4096; 
const char *name = "OS";
// shared memory file descriptor
int shm_fd;

// Create shared memory


 int num = atoi(argv[1]);
  pid_t pid;
  pid = fork();
 
  if(pid == 0){
  	// Child process 
  	printf("Child process\n");
  	
  	shm_fd = shm_open(name , O_CREAT | O_RDWR ,0666);
	ftruncate(shm_fd, SIZE);
	 int* ptr = (int *)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd,0);

	int count =1 ;
  	if(num < 0 )
  		{
  		printf("Error negative number!\n");
  		}
  	else{
  	  while(num != 1){
  		*(ptr+ count) = num; 
  		count++;
  			if(num %2 == 0 ) // even
  			{
  				num = num/2;
  			}
  			else{
  				num = 3*num +1 ;
  			}
  			
  			
  		}	
  	}
  	*ptr =  count;
    
  }
  else{
  	wait(NULL);
  	printf("Parent process\n");
  	shm_fd = shm_open(name , O_CREAT | O_RDWR ,0666);
  	 int* ptr = (int *)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd,0);
  	 int count = *ptr;
  	 for(int i=1 ; i<count ; i++){
  	  	printf("%d, " , *(ptr+i)); 
  	 }
  	 printf("1\n");
  	 shm_unlink(name); 
  } 
   
  return 0;
}


