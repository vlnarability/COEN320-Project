#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include "FileRead.h" //contains thread functions

#define _GNU_SOURCE

sem_t sem_cons;				//consumer semaphore
int status;					//status for function calls
int update[5];
float data[5];
struct timeval tv;

boolean state = F;
boolean command = F;
DataBuffer* buf;

void* consumerThread();
void* producerThread(Param*);

static void task_body(void) {   	//Timer is taking from the tutorial provided code
	static int cycles = 0;
	static uint64_t start;
	uint64_t current;
	struct timespec tv;
	
	if (start == 0) {
		clock_gettime(CLOCK_MONOTONIC, &tv);
		start = tv.tv_sec * 1000 + tv.tv_nsec / 1000000;
	}
	
	clock_gettime(CLOCK_MONOTONIC, &tv);
	current = tv.tv_sec * 1000 + tv.tv_nsec / 1000000;
	
	if (cycles > 0) {
		fprintf(stderr, "Ave interval between instances: %f millisecons\n",
			(double)(current-start)/cycles);
	}
	
	cycles++;
}

int main(void)
{
	int itr;
	int i = 0;
	pthread_t producer_id[5], consumer_id, command_id;

	sem_init(&sem_cons, 0, 1);			//initial consumer semaphore 

	for (itr = 0; itr < 5; itr++) {		//allocate memory
		p[itr] = malloc(sizeof(Param));
		p[itr]->filenum = itr;
	}
 
	for (i = 0; i < 5; i++) { 			//inital update period for all variable
		p[i]->period = 5000;
		frequency[i] = 5000;
	}
	
	for (itr = 0; itr < 5; itr++) {		//initial set update for all files to 0 (no update in consumer thread)
		update[itr] = 0;
	}

	status = pthread_create(NULL, NULL, (void*)consumerThread, NULL);			//create consumer thread
	
	if (status != 0) {
			printf("Consumer thread creation failed.");
			exit(-1);
		}
		
		
	for (itr = 0; itr < sizeof(producer_id) / sizeof(producer_id[0]); itr++) {	//create producer thread
			status = pthread_create(&producer_id[itr], NULL, (void*)producerThread, (void*)p[itr]);
		
		if (status != 0) {
			printf("Producer thread creation failed. Failure Number: %d\n", itr);
			exit(-1);
		}
	/*
	status = pthread_create(&command_id, NULL,(void*)commandProcessor,(void*)itr);//create command thread
		
	if(status != 0){
		printf("Thread command creation failed. Failure Number: %d\n", itr);
		exit(-1);
		}
		*/
	}
	
	buf = (DataBuffer*)malloc(sizeof(DataBuffer));    //allocate memory
	pthread_create(NULL, NULL, (void*)file_read, (void*)buf);  //file read as a seperate thread
	
	while (1) {
		commandUpdate();					//continous recieve command 
	}
	
	pthread_join(consumer_id, NULL);		//finish all thread
	pthread_join(producer_id[0], NULL);		
	pthread_join(producer_id[1], NULL);
	pthread_join(producer_id[2], NULL);
	pthread_join(producer_id[3], NULL);
	pthread_join(producer_id[4], NULL);
	sem_destroy(&sem_cons);
	
	
	return EXIT_SUCCESS;
}

int getline(char* s, int lim) {		//implement getline function
	int c, i;
	for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
		s[i] = c;
	if (c == '\n') {
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}

void commandUpdate() {				//generate command input function(comman in form of "filenumber period(in millisecond)") 
	char* temp;						//for example enter "1 1000" change the first producer thread(Fuel_consumption)display period to 1 second
	char* buffer;
	size_t bufsize = 32;
	int newfreq;
	int i = 0;


	buffer = (char*)malloc(bufsize * sizeof(char));
	if (buffer == NULL) {
		perror("Unable to allocate buffer");
		exit(1);
	}

	getline(buffer, bufsize);

	if (sizeof(buffer[0]) < 1) {
		return;
	}

	printf("buffer: %c\n", buffer[0]);

	temp = buffer;
	while (*temp != ' ') {
		temp++;
	}
	temp++;

	newfreq = atoi(temp);

	//file number + " " + new period (in millisecond)
	switch (buffer[0]) {			//1-Fuel_consumption, 2-Engine_speed, 3-Engine_coolant_temperature, 4-Current_Gear, 5-Vehicle_speed, 8-update all
	case '1':						//updata Fuel_consumption
		frequency[0] = newfreq;
		printf("Fuel_consumption frequency changed to %d\n", frequency[0]);
		break;

	case '2':						//updata Engine_speed
		frequency[1] = newfreq;
		printf("Engine_speed frequency changed to %d\n", frequency[1]);
		break;

	case '3':						//updata Engine_coolant_temperature
		frequency[2] = newfreq;
		printf("Engine_coolant_temperature frequency changed to %d\n", frequency[2]);
		break;

	case '4':						//updata Current_Gear
		frequency[3] = newfreq;
		printf("Current_Gear frequency changed to %d\n", frequency[3]);
		break;

	case '5':						//updata Vehicle_speed
		frequency[4] = newfreq;
		printf("Vehicle_speed frequency changed to %d\n", frequency[4]);
		break;

	case '8':						// update all
		frequency[0] = newfreq;
		frequency[1] = newfreq;
		frequency[2] = newfreq;
		frequency[3] = newfreq;
		frequency[4] = newfreq;
		printf("all frequency changed %d\n", newfreq);
	default:
		break;
	}

}

void* producerThread(Param* param) {
	int period;
	int filenum;
	struct timespec start, finish;
	int elapsed;
	
	period = param->period;
	filenum = param->filenum;
	printf("producer thread %d creadted\n", filenum);
	
	while (1) {
		if (command) {
			// do nothing
		}
		else if (state == T) {
			sem_wait(&sem_cons);		//update of sem_cons to avoid thread problem
			
			gettimeofday(&tv, NULL);
			printf("current time in second:%d\n", tv.tv_sec);
			clock_gettime(CLOCK_MONOTONIC, &start);				//clock start

			//if(command){// if user change frequency
			period = frequency[filenum];						//update the period for current producer thread
			//}

			data[filenum] = buf->data[filenum];					//update new data
			update[filenum] = 1;								//set update to 1 -> consumer thread will display data

			printf("file: %d ,period in millisecond: %d \n", filenum + 1, period);

			clock_gettime(CLOCK_MONOTONIC, &finish);			//clock finish

			elapsed = (finish.tv_sec - start.tv_sec);

			sem_post(&sem_cons);
			delay(period);			//set the period for different producer thread
		}
	}
}

void* consumerThread() {
	int itr;
	printf("consumer threat start\n");
	
	while (1) {
		if (command) {
			//do nothing
		}
		else {
			for (itr = 0; itr < 5; itr++) {		//check for all producer thread
				if (update[itr] == 1) {			//print data if current producer thread has update value equal to 1
					printf("senser read data: %.1f , from file %d .\n", data[itr], itr + 1);
					update[itr] = 0;
				}
			}
		}

	}
	return(NULL);
}
/*
void* commandProcessor(int f_no){
	int id = f_no;
	int f_p;
	printf("command thread %d is running",id );
	while(1){


		ch = readch();
		printf("event : \n%d\n", ch);
		if(readch() ==10){
			command =T;
		}
		if(command){
			printf("sem_prod -1 id:%d\n",id);
			sem_wait(&sem_prod);
			command_counter++;
			printf("id%d enter your frequenccy: ",id);
			scanf("%d", f_p);
			printf("frequency entered: %d", f_p);
			frequency[f_no] = f_p;
			sem_post(&sem_prod);
			printf("sem_prod +1 :%d\n",id);
		}else{
			kbhit();
		}

		if(command_counter == 5){
			command = F;
		}

	}
	close_keyboard();
}
*/





