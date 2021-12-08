#include "FileRead.h" 

void* file_read(DataBuffer* buffer) {
    char file_name[100];
    int i = 0;			//row
    int j = 0;			//column
    char line[10000];	//define max line size
    char* result = NULL;
    
    FILE* myfile;
    strcpy(file_name, "320data_3.csv");		//file name in vm tmp
    myfile = fopen(file_name, "r");			//create file read
    
    if (!myfile) {
        fprintf(stderr, "cannot open %s for reading\n", file_name);
        exit(-1);
    }

    while (fgets(line, 10000, myfile) != NULL) {
        result = strtok(line, ",");				//csv file seperate by ","
        while (result != NULL) {
            if (i != 0) {
                buffer->data[j] = atof(result);     
            }
            j++;					//next column
            result = strtok(NULL, ",");
            if (i == 1) {			//ignore the line of text(1 row)
                state = T;			
            }
        }
        i++;    		//move to the next row
        j = 0;  		//reset to the first column
        delay(1000);    //provide data every 1 second
    }
    
    fclose(myfile);		//close file read
}



