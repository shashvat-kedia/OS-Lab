/* Names:Shashvat Kedia
IDs:1610110347
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(int argc , char* argsv[]){
  int fd[2];
  pid_t cpid;
  char readBuffer[100];
  char writeBuffer[100];
  int read_counter;
  int targetfile;
  FILE *targetFilePtr;

  pipe(fd); /* create pipe */

  if(argc < 2){
    printf("Enter atleast 2 parameters\n");
    exit(1);
  }
  if(argc == 2){
      targetfile = open("/Users/shashvatkedia/Desktop/default.txt" , 0666);
  }
  int openfile = open(argsv[1] ,0);

  if(openfile == -1){
    printf("Error in opening file\n");
    exit(1);
  }

  if(targetfile == -1 && argc == 2){
    targetFilePtr = fopen("/Users/shashvatkedia/Desktop/default.txt","w");
    fclose(targetFilePtr);
    targetfile = open("/Users/shashvatkedia/Desktop/default.txt" , 0666);
  }
  else if(targetfile == -1 && argc == 3){
    targetFilePtr = fopen(argsv[2],"w");
    fclose(targetFilePtr);
    targetfile = open(argsv[2] , 0666);
  }

  cpid = fork();

  if((cpid = fork()) == -1){
    perror("fork");
    exit(1);
  }
  if(cpid == 0){
    /* child only reads. Close the write end */
    close(fd[1]);
    read(fd[0] , readBuffer , sizeof(readBuffer));
    //printf("the recived string is %s\n",readBuffer );

    write(targetfile , readBuffer , strlen(readBuffer));

    /* Open the destination file and read from pipe */
    /* Create the destination file if it does not exist */
  }
  else{
    /* parent only writes. Close the read end */
    close(fd[0]);

    while(read_counter=read(openfile , readBuffer , sizeof(readBuffer))>0){
      write(fd[1] , readBuffer , sizeof(readBuffer));

    }
    close(fd[1]);
    /* Open the source file and write to pipe */
    /* Print error message if the source file does not exist */
  }
  /* Remember to close the files */
  return 0;
}
