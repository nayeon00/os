#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

sem_t mutex, empty, full;

int in=0;
int out=0;
int buffer[BUFFER_SIZE]={0};
int counter=0;

void *producer(void* arg){
	int nextProduced = 1;
	int i=0;
	while(i<10){
		while(counter == BUFFER_SIZE)
			;
		sem_wait(&empty);
		sem_wait(&mutex);
		buffer[in] = nextProduced;
		printf("buffer[%d] 생산\n", in);
		in = (in+1) % BUFFER_SIZE;
		nextProduced++;
		usleep(1);
		counter++;
		i++;
		sem_post(&mutex);
		sem_post(&full);
	}
}

void *consumer(void* arg){
	int nextConsumed;
	int i=0;
	while(i<10){
		while(counter == 0)
			;
		sem_wait(&full);
		sem_wait(&mutex);
		nextConsumed = buffer[out];
		printf("buffer[%d] 소비\n", out);
		out = (out+1) % BUFFER_SIZE;
		counter--;
		i++;
		sem_post(&mutex);
		sem_post(&empty);
	}
}

int main(){
	pthread_t ptid[2];
	pthread_t ctid[2];
	
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	
	
	for(int i=0;i<2;i++){
		pthread_create(&ptid[i], NULL, producer, NULL);
		pthread_create(&ctid[i], NULL, consumer, NULL);
	}
	
	for(int i=0;i<2;i++){
		pthread_join(ptid[i], NULL);
		pthread_join(ctid[i], NULL);
	}

	return 0;
}
