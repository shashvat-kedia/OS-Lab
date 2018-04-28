/*
Name: Shashvat Kedia
Roll No.: 1610110347

Name: Mudit Saxena
Roll No.: 1610110222
*/

#include "initialize.h"

void parseCSV(FILE *file){
	int i = 0,ind = 0;
	char in, val[MAX_DIGITS + 1];
	in = fgetc(file);
	length = (in != EOF) ? 1:0;
	while(in != EOF){
		if (in == ',')
			length++;
		in = fgetc(file);
	}
	arr = (int *) malloc (length * sizeof(int));
	if (length < 1)
		return;
	fseek(file, 0, SEEK_SET);
	in = ' ';
	while(in != EOF){
		in = fgetc(file);
		if (in == ',' || in == EOF){
			val[i] = '\0';
			arr[ind++] = atoi(val);
			i = 0;
		}
		else
			val[i++] = in;
	}

}

int getIndex(int frames[], int frame_length ,int val){
	int i;
	for (i=0; i < frame_length; i++){
		if(frames[i] == val){
			return i;
		}
	}
	return -1;
}

void initialize_arr(int frames[], int frame_length){
	int i;
	for (i=0; i < frame_length; i++){
		frames[i] = -1;
	}
}

int algoFIFO(int frames[], int frame_size){
	int replace = 0, faults = 0, i;
	initialize_arr(frames, frame_size);
	for (i=0 ; i < length; i++) {
		if(getIndex(frames, frame_size, arr[i]) == -1){
			if(faults < frame_size) {
				frames[faults] = arr[i];
			}
			else {
				frames[replace] = arr[i];
				replace = (replace + 1) % frame_size;
			}
			faults++;
		}
	}
	return faults;
}

int algoOPT(int frames[], int frame_size) {
	int mark_arr[frame_size];
	int faults = 0, replace = -1, i, j, marked, tmp;
	initialize_arr(frames, frame_size);
	for (i=0 ; i < length; i++) {
		if(getIndex(frames, frame_size, arr[i]) == -1) {
			if(faults < frame_size) {
				frames[faults] = arr[i];
			}
			else {
				marked = 0;
				initialize_arr(mark_arr, frame_size);
				for(j = i + 1; j < length; j++) {
					tmp = getIndex(frames, frame_size, arr[j]);
					if (tmp > -1) {
						if (marked == frame_size - 1) {
							break;
						}
						if (mark_arr[tmp] != 0){
							mark_arr[tmp] = 0;
							marked++;
						}
					}
				}
				tmp = getIndex(mark_arr, frame_size, -1);
				frames[tmp] = arr[i];
			}
			faults++;
		}
	}
	return faults;
}

int algoLRU(int frames[], int frame_size){
	int mark_arr[frame_size];
	int faults = 0, i, j, tmp, min;
	initialize_arr(frames, frame_size);
	for (i=0 ; i < length; i++) {
		tmp = getIndex(frames, frame_size, arr[i]);
		if(tmp == -1){
			if(faults < frame_size) {
				frames[faults] = arr[i];
				mark_arr[faults] = i;
			}
			else {
				min = INT_MAX;
				for(j = 0; j < frame_size; j++){
					if (mark_arr[j] < min){
						min = mark_arr[j];
						tmp = j;
					}
				}
				frames[tmp] = arr[i];
				mark_arr[tmp] = i;
			}
			faults++;
		}
		else{
			mark_arr[tmp] = i;
		}
	}
	return faults;
}

int main(int argc, char *argv[]){
	int frameSizes[] = {1, 4, 6, 10};
	int numOfFrameSizes = 4;
	int i, algoFaults[3];
	FILE *file_read;

	if (argc < 2) {
		puts("Invalid command");
		exit(1);
	}

	file_read = fopen(argv[1], "r");
	if (file_read == NULL){
		perror("Source");
		exit(1);
	}
	parseCSV(file_read);
	fclose(file_read);

	for(i=0; i < numOfFrameSizes; i++){
		printf("\nFrames: %d\n",frameSizes[i]);
		framesArr = (int *) malloc(frameSizes[i] * sizeof(int));
		algoFaults[0] = algoFIFO(framesArr, frameSizes[i]);
		algoFaults[1] = algoOPT(framesArr, frameSizes[i]);
		algoFaults[2] = algoLRU(framesArr, frameSizes[i]);
		printf("FIFO %d\nOPT %d\nLRU %d\n",algoFaults[0],algoFaults[1],algoFaults[2]);
		free(framesArr);
	}

	free(arr);

	return 0;
}
