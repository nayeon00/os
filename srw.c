#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

sem_t rw_mutex, mutex;
int read_count = 0;

int buffer[BUFFER_SIZE]={0};
char str[10];

void *reader(void* arg){
	sem_wait(&mutex);
	read_count++;
	if (read_count == 1){
		sem_wait(&rw_mutex);
	}
	sem_post(&mutex);
	char rstr[10];
	strcpy(rstr, str);
	printf("Reader : %s\n", rstr);
	sem_wait(&mutex);
	read_count--;
	if (read_count == 0){
		sem_post(&rw_mutex);
	}
	sem_post(&mutex);
}


char word[5][15] = {"pineapple", "strawberry", "watermelon", "blueberry", "orange"};
int c = 0;

void *writer(void* arg){
	sem_wait(&rw_mutex);
	strcpy(str, word[c%5]);
	printf("Writer : %s\n", str);
	c++;
	usleep(1);
	sem_post(&rw_mutex);
}

int main(){
	pthread_t rtid[5];
	pthread_t wtid[5];
	
	sem_init(&rw_mutex, 0, 1);
	sem_init(&mutex, 0, 1);
	
	int j=0;
	while(j<5){
		for(int i=0;i<5;i++){
			pthread_create(&wtid[i], NULL, writer, NULL);
			pthread_create(&rtid[i], NULL, reader, NULL);
		}
	
		for(int i=0;i<5;i++){
			pthread_join(wtid[i], NULL);
			pthread_join(rtid[i], NULL);
		}
		j++;
	}

	return 0;
}
