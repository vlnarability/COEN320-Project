#include <stdlib.h>
#include <stdio.h>

#ifndef FILEREAD_H_
#define FILEREAD_H_

int frequency[5];// command frequency 
//extern boolean command;// check command
typedef enum {F,T} boolean;
extern boolean state;

typedef struct{		//define buffer
	float data[100];			
}DataBuffer;

typedef struct Param{
	int period;//period in microseconds
	int filenum;//1-Fuel_consumption, 2-Engine_speed, 3-Engine_coolant_temperature, 4-Current_Gear, 5-Vehicle_speed
}Param;

struct Param* p[5]; 			//parameters for 5 different producer thread

void* file_read(DataBuffer*);		//csv file reader

//void* commandProcessor(int);    	//command Processor

void commandUpdate();			//generate command update

int getline(char*,int);			//getline implementation for command

#endif /*FILEREAD_H_*/
