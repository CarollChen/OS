#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

/* the maximum time (in seconds) to sleep */
#define MAX_SLEEP_TIME 3
/* number of potential students */ 
#define NUM_OF_STUDENTS 4
#define NUM_OF_HELPS 2
/* number of available seats */ 
#define NUM_OF_SEATS 2

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex;
/* semaphore declarations */
sem_t students_sem; /* ta waits for a student to show up, student notifies ta his/her arrival*/ 
sem_t ta_sem; /* student waits for ta to help, ta notifies student he/she is ready to help*/
/* the number of waiting students */
int waiting_students;
int seed;
void *Ta(void *param){
	int totalHelpers = 8;
	while(totalHelpers){
		struct timeval now;
		struct timespec timeout;
		
		gettimeofday(&now);
		timeout.tv_sec = now.tv_sec + rand_r(&seed)%MAX_SLEEP_TIME;
		timeout.tv_nsec = now.tv_usec * 1000;

		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		int programTime = rand_r(&seed);
		int sleepTime = programTime%MAX_SLEEP_TIME+1;
		int sval = 0;
		sem_getvalue(&students_sem, &sval);
		printf("Helping student for %d seconds, # of waiting students = %d \n",sleepTime,2-sval);
		//check if there are student needs help
		totalHelpers--;
		pthread_mutex_unlock(&mutex);
	}
		pthread_exit(0);
}

void *Student(void *param){
	int restHelp = NUM_OF_HELPS;
	int id = *( (int *) param);
	
	 while(restHelp){
		int programTime = rand_r(&seed);
		int sleepTime = programTime%MAX_SLEEP_TIME+1;
		printf("Student #%d programming for %d seconds \n",id-1, sleepTime);
		sleep(sleepTime);
		
		int *seats = (int*)malloc(sizeof(int));
		if(!sem_trywait(&students_sem)){
			int sval = 0;
			sem_getvalue(&students_sem, &sval);
			pthread_mutex_lock(&mutex);
	 		printf("Student # %d takes a seat, # of waiting students = %d \n",id-1,2-sval);
			pthread_mutex_unlock(&mutex);
			sem_wait(&ta_sem);
			pthread_mutex_lock(&mutex);
			printf("Student # %d receiving help \n",id-1);
			sem_post(&students_sem);
			pthread_cond_broadcast(&cond);
			pthread_mutex_unlock(&mutex);
			sleep(3);
			restHelp--;
	 		sem_post(&ta_sem);
	 		
	 	}
	 	else {
	 		//else continue
			printf("Student # %d will try later\n",id-1);
			sleep(2);
	 		continue;
	 	}
	 }

	pthread_exit(0);
}

int main(){
	seed = rand();
	printf("CS149 SleepingTA from Jingyuan Chen\n");
	pthread_mutex_init((&mutex),NULL);
	pthread_attr_t attr;
	sem_init(&students_sem, 0, 2); 
	sem_init(&ta_sem, 0, 1); 
	pthread_t *studentPool = (pthread_t *)malloc(sizeof(pthread_t)*NUM_OF_STUDENTS);
	pthread_t ta;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&ta, &attr, Ta, NULL);

	int i=0;

	int *ids = (int *)malloc(sizeof(int)*NUM_OF_STUDENTS);

	for(;i<NUM_OF_STUDENTS;i++){
		*(ids+i) = i+1;
		pthread_create((studentPool+i), &attr, Student, (void*) (ids+i));
	}

	void *rv;

	for(i=0;i<NUM_OF_STUDENTS;i++){
		pthread_join(*(studentPool+i),&rv);
	}
	pthread_join(ta,&rv);

	return 0;
}
