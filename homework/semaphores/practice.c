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
union semun 
{
	int val;               //value for SETVAL
	struct semid_ds *buf;  //buffer for IPC_STAT, IPC_SET
	unsigned short *array; //array for GETALL, SETALL
	struct seminfo *__buf; //buffer for IPC_INFO
}arg;
/*********************************************************************************/
void SemaphoreStat(struct semid_ds *buff);
/*********************************************************************************/
const char *file_name = "semkeyfile.txt";
/** Beginning of main() body of program ******************************************/
int main(void)
{
	int semid;
	key_t key;
	int err, fd;

	/** open file for producing the key by calling ftok() *******************/
	fd = open(file_name, O_RDWR| O_CREAT| O_TRUNC);
	//printf("file desciptor %d\n", fd);

	/** allocate space for semaphore set "struct semid_ds" *********/
	arg.buf=malloc(sizeof(struct semid_ds));

	/** ftok converts a pathname and a project id to a System V IPC key **/
	//key=ftok(file_name, 15);

	/** output results of ftok() **/
	//printf("Created key=%d.\n", key);
	printf("errno=%d: %s.\n", errno, strerror(errno));

	/** create semaphore with 3 semaphores in set **/
	semid=semget(IPC_PRIVATE, NUMBER_OF_SEMAPHORES, 0666);

	/** output results of creating semaphore with semget() **/
	printf("Created semaphore with semid=%d.\n", semid);
	printf("errno=%d: %s.\n", errno, strerror(errno));

	/** retrieve the semid_ds structure stored in kernel, storing it in the 
	    structure pointed to by arg.buf (getter function) **/
	err=semctl(semid, NUMBER_OF_SEMAPHORES, IPC_STAT, arg.buf);

	//printf("arg.val=%d\n", arg.val);
	printf("Number of semaphores in the set should be %d, and it is %lu\n", NUMBER_OF_SEMAPHORES, arg.buf->sem_nsems);
	
	//SemaphoreStat(arg.buf);

	return 0;
}
