#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <math.h>


int MAX_LINE = 80;
int BUFFER_SIZE = 1024;
int LAST_CMD = -1;
int MAX = 512;
int count = 0;

FILE *filePtr;

typedef struct list{
  char cmd[80];
  int num;
  struct list *left;
  struct list *right;
}node;

node *head = NULL;
node *last = NULL;

void save_cmd(char line[MAX_LINE]){
  count++;
  if(head != NULL){
    node *temp = (node *) malloc(sizeof(node));
    head->right = temp;
    temp->right = NULL;
    temp->left = head;
    strcpy(temp->cmd,line);
    temp->num = count;
    head = temp;
  }
  else{
    head = (node *) malloc(sizeof(node));
    strcpy(head->cmd,line);
    head->num = count;
    head->left = NULL;
    head->right = NULL;
    last = head;
  }
}

void read_file(){
  filePtr = fopen("/Users/shashvatkedia/Desktop/Shell/hist.txt","r");
  count = 0;
  char cmd[MAX_LINE];
  if(filePtr){
    while(fgets(cmd,sizeof(cmd),filePtr) != NULL){
      cmd[strlen(cmd)-1]='\0';
      save_cmd(cmd);
    }
  }
  fclose(filePtr);
}

void write_file(){
  filePtr = fopen("/Users/shashvatkedia/Desktop/Shell/hist.txt","a");
  fprintf(filePtr,"%s \n",head->cmd);
  fclose(filePtr);
}

void show_history(){
  if(count < 10){
    node* temp = head;
    while(temp->left){
      printf("%d %s\n",temp->num,temp->cmd);
      temp = temp->left;
    }
    printf("%d %s\n",temp->num,temp->cmd);
  }
  else{
    node* temp = head;
    for(int i = 0;i <= 9;i++){
      if(temp){
      printf("\n%d %s",temp->num,temp->cmd);
      temp = temp->left;
    }
  }
  }
}

char* output_command(int state){
  if(state == 0){
    if(count == 0){
      printf("No command present in history\n");
      return NULL;
    }
    else{
    return head->cmd;
  }
}
else{
  if(count < state){
    printf("No command present in history\n");
    return NULL;
  }
  else{
    node* temp = head;
    while(temp->left){
      if(temp->num == state){
        return temp->cmd;
      }
      temp = temp->left;
    }
    if(temp->num == state){
      return temp->cmd;
    }
  }
}
return NULL;
}

void execute(char** args,int amp_flag){
  pid_t pid;
  pid = fork();
  if(pid == 0){
    execvp(args[0],args);
    printf("Invalid\n");
    exit(0);
  }
  else if(pid > 0 && amp_flag == 0){
    waitpid(pid,NULL,0);
  }
}

int main(void){
read_file();
int should_run = 1;
int amp_flag = 0,count = 0;
char line[MAX_LINE];
char* args[MAX_LINE/2 + 1];
while (should_run) {
printf("shashvat>");
fgets(line,MAX_LINE,stdin);
line[strlen(line) - 1] = '\0';
if(!strcmp(line,"history")){
  show_history();
  save_cmd(line);
  write_file();
  continue;
}
else if(!strcmp(line,"!!")){
  char* cmd = output_command(0);
  if(cmd != NULL){
    strcpy(line,cmd);
    printf("Executing   %s\n",cmd);
    if(!strcmp(cmd,"history")){
      show_history();
      save_cmd(line);
      write_file();
      continue;
    }
    else if(!strcmp(cmd,"quit")){
      should_run = 0;
      save_cmd(line);
      write_file();
      continue;
    }
    save_cmd(cmd);
  }
  else{
    continue;
  }
}
else if(line[0] == '!'){
  int num = atoi(line+1);
  if(num != 0){
    char* cmd = output_command(num);
    if(cmd != NULL){
      strcpy(line,cmd);
      printf("Executing  %s\n",cmd);
      if(!strcmp(cmd,"history")){
        show_history();
        save_cmd(line);
        write_file();
        continue;
      }
      else if(!strcmp(cmd,"quit")){
        should_run = 0;
        save_cmd(line);
        write_file();
        continue;
      }
      save_cmd(cmd);
    }
    else{
      continue;
    }
  }
}
else if(!strcmp(line,"quit")){
  should_run = 0;
  save_cmd(line);
  write_file();
  continue;
}
else{
  save_cmd(line);
}
fflush(stdout);
char* token = strtok(line," ");
while(token != NULL){
  args[count] = token;
  token = strtok(NULL," ");
  count++;
}
args[count] = NULL;
if(!strcmp(args[count - 1],"&")){
  amp_flag = 1;
  fflush(stdout);
  args[count - 1] = NULL;
}
count = 0;
write_file();
execute(args,amp_flag);
fflush(stdout);
}
return 0;
}
