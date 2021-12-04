#include "FileRead.h" 

void* file_read(DataBuffer* buffer) {
    char file_name[100];
    int i = -1;			//row, ignore row 1 text
    int j = 0;			//column
    char line[MAX_LINE_SIZE];
    char* result = NULL;
    FILE* myfile;

    strcpy(file_name, "320data_3.csv");		//file name in vm tmp
    myfile = fopen(file_name, "r");
    if (!myfile) {
        fprintf(stderr, "failed to open %s for reading\n", file_name);
        exit(-1);
    }

    while (fgets(line, MAX_LINE_SIZE, myfile) != NULL) {
        result = strtok(line, ",");
        while (result != NULL) {
            if (i != -1) {
                buffer->data[j] = atof(result);
            }
            j++;
            result = strtok(NULL, ",");
            if (i == 0) {
                state = T;
            }
        }
        i++;    		//move to the next row
        j = 0;  		//reset to the first column
        delay(1000);    //provide data every 1 second
    }
    //close file
    fclose(myfile);
}



