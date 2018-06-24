/* p3helper.c
 * Nash Granstrom
 * John Carroll
 * Program 3 assignment
 * CS570
 * San Diego State University
 * Spring 2018

   */
#include "p3.h"
#define COURSEID "570"

/* You may put declarations/definitions here.
   In particular, you will probably want access to information
   about the job (for details see the assignment and the documentation
   in p3robot.c):
     */

extern int nrRobots;
extern int seed;
extern int width;
extern int quota;

int fd;
int count;
char semaphoreMutx[SEMNAMESIZE];
sem_t *pmutx; /* semaphore guarding access to shared data */

/* General documentation for the following functions is in p3.h
   Here you supply the code, and internal documentation:
   */
void initStudentStuff(void) {

sprintf(semaphoreMutx,"/%s%ldmutx",COURSEID,(long)getuid());

 if((pmutx = sem_open(semaphoreMutx,O_RDWR|O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,1)) == SEM_FAILED){
 	
	CHK((int)(pmutx = sem_open(semaphoreMutx,O_RDWR)));
	CHK(fd = open("countfile",O_RDWR));
 }
 else{
 	CHK(fd = open("countfile",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR));
  	count = 0;
	CHK(sem_wait(pmutx)); //wait for files to be initialized
  	CHK(lseek(fd,0,SEEK_SET));
  	assert(sizeof(count) == write(fd,&count,sizeof(count))); //write count to file
	CHK(sem_post(pmutx)); //files are now initialized
	
}



	 
/*
	sprintf(semaphoreMutx,"%s%ldmutx",COURSEID,(long)getuid());

	CHK(fd = open("countfile",O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR));
  	count = 0;
	CHK(lseek(fd,0,SEEK_SET));
  	assert(sizeof(count) == write(fd,&count,sizeof(count))); //write count to file

	CHK(SEM_FAILED != (pmutx = sem_open(semaphoreMutx,O_RDWR|O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,1)));
*/
}

/* In this braindamaged version of placeWidget, the widget builders don't
   coordinate at all, and merely print a random pattern. You should replace
   this code with something that fully follows the p3 specification. */

void placeWidget(int n) {
	CHK(sem_wait(pmutx));
	CHK(lseek(fd,0,SEEK_SET));
  	assert(sizeof(count) == read(fd,&count,sizeof(count)));
  	count++;
	/* end critical section */
	int tmp = count;

	if(tmp == ((nrRobots * quota))){
  		printeger(n);
		printf("F\n");
		fflush(stdout);
		CHK(close(fd));
		CHK(unlink("countfile"));
    		CHK(sem_close(pmutx));
    		CHK(sem_unlink(semaphoreMutx));
		exit(0);
  	}
  	else{
  		if(!(tmp % width)){
    			printeger(n);
  			printf("N\n");
  			fflush(stdout);
  		}		
		else{
			printeger(n);
			fflush(stdout);
		}
  	}

	count = tmp;
	CHK(lseek(fd,0,SEEK_SET));
	assert(sizeof(count) == write(fd,&count,sizeof(count)));
	CHK(sem_post(pmutx));
	
}


