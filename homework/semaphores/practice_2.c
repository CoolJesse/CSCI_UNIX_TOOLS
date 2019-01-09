#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define NUMBER_OF_SEMAPHORES  3

/** create datatype for semctl() to use, passed as fourth arguement to function **/
static union semun 
{
	int val;               //value for SETVAL
	struct semid_ds *buf;  //buffer for IPC_STAT, IPC_SET
	unsigned short *array; //array for GETALL, SETALL
	struct seminfo *__buf; //buffer for IPC_INFO
}arg;
/*********************************************************************************/
void SemaphoreStat(struct semid_ds *buff);
/*********************************************************************************/
const char *file_name = "file.txt";
/** Beginning of main() body of program ******************************************/
int main(void)
{
	int pid;
	int i, j;
	int err, fd;
	ssize_t numBytes;	
	int semid; //semid of semaphore set
	key_t key = IPC_PRIVATE; //key passed to semget()
	int semflg=IPC_CREAT | 0666; //semflg to pass to semget()
	int nsems=1; //number of semaphores per set to pass to semget()
	int nops; //number of operations to do

	/** ptr to operations to perform **/
	struct sembuf *semoparray = (struct sembuf *)malloc(2*sizeof(struct sembuf));

	/** open file for producing the key by calling ftok() *******************/
	fd = open(file_name, O_RDWR| O_CREAT| O_APPEND);
	//printf("file desciptor %d\n", fd);

	/** allocate memory for array of three elements and populate array **/	
	char **childInput=calloc(3, 20*sizeof(char));
	
	childInput[0]="Bart Simpson\n";
	childInput[1]="Lisa Simpson\n";
	childInput[2]="Maggie Simpson\n";

	/** allocate memory for array of three elements and populate array **/	
	char **parentInput=calloc(3, 20*sizeof(char));
	
	parentInput[0]="Homer Simpson\n";
	parentInput[1]="Marge Simpson\n";
	parentInput[2]="Grandpa Simpson\n";

	/** setting up the semaphore set **/
	fprintf(stderr, "Calling semget() with key=%x, nsems=%d, semflg=%o.\n", key, nsems, semflg);

	/** create semaphore **/
	if( (semid=semget(key, nsems, semflg) ) == -1)
	{
		perror("Call to semget() failed.");
		exit(1);
	}

	else
		(void) fprintf(stderr, "Call to semget succeeded: semid=%d\n", semid);

	/** union semun buf attribute points semid_ds object**/
	arg.buf=malloc(sizeof(struct semid_ds));

	/** union semun val attribute equal to 1, used for SETVAL command with semctl() **/
	arg.val = 1;
	
	/** Set the value of semval specified by arg.val **/
	err=semctl(semid, 0, SETVAL, arg);

	/** print the results of semctl() **/
	printf("Semaphore 0 has semval=err=%d.\n", err);
	printf("Semaphore 0 has semval=arg.val=%d.\n", arg.val);

	if( (pid=fork())<0 )
	{
		perror("Call to fork() failed.\n");
		exit(1);
	}
	
	/** Child ***********************************************************************/
	if( (pid==0) )
	{
		i=0;
		/** There is 1 semaphore in set, to aquire/release 3 times **/
		while(i<3)
		{
			/** test the semaphore **/
			if(err>0)
			{
				/* Set arguments for 1st call to semop() on the set*/
				/* 1. First argument: semid */
				/* 2. Second argument: semoparray[nopa] */
				/* Operation -1: "acquire resource" */

				/** Semaphore # in set=only use 1 "track": 1 semaphore is semaphore set **/
				semoparray[0].sem_num=0;

				/** Operation -1="DECREMENT SEMAPHORE: resource in use" **/
				semoparray[0].sem_op=-1;

				/** Flag="take off semaphore asynchronous" **/
				semoparray[0].sem_flg=SEM_UNDO

				nops=1;
				fprintf(stderr, "\nsemop:CHILD calling semop(%d, &semoparray, %d) with: ", semid, nops );

				for(j=0; j<nops; j++)
				{
					fprintf(stderr, "\n\tsemoparray[%d].sem_num=%d, ", j, semoparray[j].sem_num);
					fprintf(stderr, "sem_op=%d, ",semoparray[j].sem_op);
					fprintf(stderr, "sem_flg=%o\n", semoparray[j].sem_flg);
				}

				if((j=semop(semid, semoparray, nops)) == -1)
					perror("Call to semop() failed.");
				
				else
				{
					err=semctl(semid, 0, GETVAL, NULL);
					printf("After DECREMENT CHILD: semval=%d\n", err);
					fprintf(stderr, "\n\tChild process taking control of track: %d/3 times\n", i+1 );
					numBytes=write(fd, childInput[i], strlen(childInput[i]));
					sleep(5);
				}

				/** Set argument for 2nd call to semop() on the set **/
				/* 1. First argument: semid */
				/* 2. Second argument: semoparray[nops] */
				/* Operation 1: "release the resource" */
				semoparray[0].sem_num=0;
				semoparray[]

				/** Second semop() call to release the resource **/
				if()
				else()

			else if(err==0)
			{
				fprintf(stderr, "Waiting for resource to be released by Parent.\n");
				sleep(5);
			}
			
			i++
		}
		/** end while() **********************************************/
	}
	/** Parent *********************************************************************/
	else
	{
	}
	return 0;
}
