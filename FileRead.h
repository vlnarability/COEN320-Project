#define _GNU_SOURCE
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <termios.h>

#ifndef THREADFUNCTION_H_
#define THREADFUNCTION_H_

#define SEM_PRODUCER_FNAME "/producerSem"
#define SEM_CONSUMER_FNAME "/consumerSem"

#define MAX_LINE_SIZE 9999
#define MAX_SHEET_SIZE 9999

typedef enum { F, T } boolean;
extern boolean state;
int frequency[5];// command frequency 
//extern boolean command;// check command

typedef struct {		//define buffer
	float data[100];
}DataBuffer;

typedef struct Param {
	int period;//period in microseconds
	int filenum;//1-Fuel_consumption, 2-Engine_speed, 3-Engine_coolant_temperature, 4-Current_Gear, 5-Vehicle_speed
}Param;

struct Param* p[5]; //an array of 5 Param elements

void* file_read(DataBuffer*);		//read function

//void* commandProcessor(int);    //command Processor

void commandUpdate();

int getline(char*, int);			//get command
#endif /*THREADFUNCTION_H_*/
