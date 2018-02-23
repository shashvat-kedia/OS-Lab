/* Names:
IDs:
Explain your steps as comments in the code itself.
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(){
int fd[2];
pid_t cpid;
pipe(fd); /* create pipe */
if((cpid = fork()) == -1){
perror("fork");
exit(1);
}
if(cpid == 0){
/* child only reads. Close the write end */
close(fd[1]);
/* Open the destination file and read from pipe */
/* Create the destination file if it does not exist */
}
else{
/* parent only writes. Close the read end */
close(...);
/* Open the source file and write to pipe */
/* Print error message if the source file does not exist */
}
/* Remember to close the files */
return 0;
}
