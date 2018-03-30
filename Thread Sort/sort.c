#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int *unsorted;
int *sorted;

typedef struct{
	int start;
	int end;
	}parameters;

void arrprint(int arr[], int start, int end){
	int i;
	for (i=start;i<end;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
}

void sort(void *param){
	parameters *data = (parameters *)param;
	int i,k;
	for (i=data->start;i<data->end;i++){
		for (k=data->start;k<(data->end)-1;k++){
			int temp;
			if (unsorted[k]>unsorted[k+1]){
				temp = unsorted[k];
				unsorted[k]=unsorted[k+1];
				unsorted[k+1]=temp;
			}
		}
	}
	//arrprint(unsorted,data->start,data->end);
}

int main(int argc,char *argv[]){
	if(argc!=2){
		printf("Aborting! Incorrect number of parameters entered\n");
	}
	else{
		
		pthread_t t1;
		pthread_t t2;
		
		char temp;
		char v[14];
		int cc=0,values,track=0,index=0,l=0,s1=0,s2,f;
		FILE *srcfile;
		srcfile=fopen(argv[1],"r");
		temp = fgetc(srcfile);
		while(temp!=EOF){
			if (temp==',')
				cc++;
			temp = fgetc(srcfile);
		}
		values=cc+1;
		unsorted = (int *)malloc(sizeof(int) * values);
		sorted = (int *)malloc(sizeof(int) * values);
		fseek(srcfile,0,SEEK_SET);
		v[track] = fgetc(srcfile);
		while(v[track]!=EOF){
			if (v[track]==','){
				v[track]='\0';
				unsorted[index++]=atoi(v);
				for (f=0;f<14;f++){
					v[f]=0;
				}
				track=-1;
			}
			v[++track]=fgetc(srcfile);
		}
		unsorted[values-1]=atoi(v);
		parameters *data1 = (parameters *)malloc(sizeof(parameters));
		data1->start = 0;
		data1->end = values/2;
		pthread_create(&t1,NULL,(void *)sort,(void *)data1);
		parameters *data2 = (parameters *)malloc(sizeof(parameters));
		data2->start = values/2;
		data2->end = values;
		pthread_create(&t2,NULL,(void *)sort,(void *)data2);
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		s2=values/2;
		while(l<values){
			if (s1<values/2 && (unsorted[s1]<=unsorted[s2]||s2==values)){
				sorted[l++]=unsorted[s1++];
			}
			else if (s2<values && (unsorted[s2]<=unsorted[s1]||s1==values/2)){
				sorted[l++]=unsorted[s2++];
			}
		}
		printf("Sorted Left array\n");
		printf("\n");
		arrprint(sorted,0,values/2);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("Sorted Right array\n");
		printf("\n");
		arrprint(sorted,values/2,values);
		printf("\n");
		printf("\n");
		printf("\n");
		printf("Sorted Array:\n");
		printf("\n");
		arrprint(sorted,0,values);
	}

}
