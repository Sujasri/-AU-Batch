#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#define MAX_JOB 10
pthread_t tid[2];
int count = 0;
int head =0;
int tail = 0;
int total=0;
int wait_time=0;
int turn_time=0;
/*Lock and condition variables*/
pthread_mutex_t cmd_queue_lock;  
pthread_cond_t cmd_buf_not_full; 
pthread_cond_t cmd_buf_not_empty; 

struct jobQueue
{  
   int arg1,arg2;
   int burst_time,priority;
   char arr_time[30];
   char name[20];
   char exec_time[30];
   char comp_time[30];
   int num;
  
};
struct jobQueue job[MAX_JOB-1];
void *scheduling();
void *dispatching();
void input_help();
void input_run(char input_cmd[]);
void fcfs();
void sjf();
void priority();
void perfEvaluate();
int splitColon(char time[]);
void input_help()
{
	printf("\nrun <jobname> <argument1> <argument2> <burst_time> <priority>: submit a job named <jobname>,\n\t\t execution time is <burst_time>,\n\t\t priority is <priority>.");
	printf("\nlist: display the job status");
	printf("\nfcfs: change the scheduling policy to FCFS");
	printf("\nsjf: change the scheduling policy to SJF");
	printf("\npriority: change the scheduling policy to priority");
	printf("\nquit: exit AUBatch");
				
}
void input_run(char input_cmd[])
{
	char cmd[50];
	strcpy(cmd,input_cmd);
	char *token = strtok(cmd, " ");
	int j=1;
	while (token != NULL)
	{
		if (j==2)
			sscanf(token, "%s", &job[head].name);
		if (j==3)
			sscanf(token, "%d", &job[head].arg1);
		if (j==4)
			sscanf(token, "%d", &job[head].arg2);
		if (j==5)
			sscanf(token, "%d", &job[head].burst_time);
		if (j==6)
			sscanf(token, "%d", &job[head].priority);
		j++;
		token = strtok(NULL, " ");
	}
	if(j!=7){
		printf("\nWrong command");
		
	}
	time_t T=time(NULL);
	struct tm tm = *localtime(&T);
	sprintf(job[head].arr_time,"%d:%d:%d",tm.tm_hour, tm.tm_min, tm.tm_sec);
	job[head].num = head+1;
			count++;
			head++;
			total++;

}
void fcfs(){
	int min_burst,min_arg1,min_arg2,min_priority,min_num;
	char min_name[20],min_time[30];
	int i,j;
	   for (i = 1; i < count; i++)
	   {   min_num = job[i].num;
		   min_burst = job[i].burst_time;
		   min_arg1 = job[i].arg1;
		   min_arg2 = job[i].arg2;
		   min_priority = job[i].priority;
		   strcpy(min_name , job[i].name);
		   strcpy(min_time ,job[i].arr_time);
		   j = i-1; 
		   while ((j >= 0) && (job[j].num > min_num))
		   {
			   job[j+1].num = job[j].num;  
			   job[j+1].burst_time = job[j].burst_time;
			   job[j+1].arg1 = job[j].arg1;
			   job[j+1].arg2 = job[j].arg2;
			   job[j+1].priority = job[j].priority;
			   strcpy(job[j+1].arr_time ,job[j].arr_time);
			   strcpy(job[j+1].name ,job[j].name);
			   j = j-1;
		   }
		   job[j+1].num = min_num;
		   job[j+1].burst_time = min_burst;
		   job[j+1].arg1 = min_arg1; 
		   job[j+1].arg2 = min_arg2;
		   job[j+1].priority = min_priority;
		   strcpy(job[j+1].arr_time ,min_time);
		   strcpy(job[j+1].name , min_name);
	   }
	   
	
	printf("\nScheduling policy is switched to FCFS. All the %d waiting jobs have been rescheduled.",count);
}
void sjf(){
	int min_burst,min_arg1,min_arg2,min_priority;
	char min_name[20],min_time[30];
	int i,j;
	   for (i = 1; i < count; i++)
	   {
		   min_burst = job[i].burst_time;
		   min_arg1 = job[i].arg1;
		   min_arg2 = job[i].arg2;
		   min_priority = job[i].priority;
		   strcpy(min_name , job[i].name);
		   strcpy(min_time ,job[i].arr_time);
		   j = i-1; 
		   while ((j >= 0) && (job[j].burst_time > min_burst))
		   {
			   job[j+1].burst_time = job[j].burst_time;
			   job[j+1].arg1 = job[j].arg1;
			   job[j+1].arg2 = job[j].arg2;
			   job[j+1].priority = job[j].priority;
			   strcpy(job[j+1].arr_time ,job[j].arr_time);
			   strcpy(job[j+1].name ,job[j].name);
			   j = j-1;
		   }
		   job[j+1].burst_time = min_burst;
		   job[j+1].arg1 = min_arg1; 
		   job[j+1].arg2 = min_arg2;
		   job[j+1].priority = min_priority;
		   strcpy(job[j+1].arr_time ,min_time);
		   strcpy(job[j+1].name , min_name);
	   }
	   
	   printf("\nScheduling policy is switched to SJF. All the %d waiting jobs have been rescheduled.",count);
	  
	}
void priority(){
	int max_priority,max_arg1,max_arg2,max_burst_time;
	char max_name[20],max_time[30];
	int i,j;
	   for (i =1 ; i < count; i++)
	   {
		   max_priority = job[i].priority;
		   max_arg1 = job[i].arg1;
		   max_arg2 = job[i].arg2;
		   max_burst_time = job[i].burst_time;
		   strcpy(max_name,job[i].name);
		   strcpy(max_time ,job[i].arr_time);
		   j = i-1; 
		   while ((j >= 0) && (job[j].priority < max_priority))
		   {
			   job[j+1].priority = job[j].priority;
			   job[j+1].burst_time = job[j].burst_time;
			   job[j+1].arg1 = job[j].arg1;
			   job[j+1].arg2 = job[j].arg2;
			   stpcpy(job[j+1].arr_time,job[j].arr_time);
			   strcpy(job[j+1].name , job[j].name);
			   j = j-1;
		   }
		   job[j+1].priority = max_priority;
		   job[j+1].burst_time = max_burst_time;
		   job[j+1].arg1 = max_arg1; 
		   job[j+1].arg2 = max_arg2;
		   strcpy(job[j+1].arr_time, max_time);
		   stpcpy(job[j+1].name,max_name);
}
	printf("\nScheduling policy is switched to Priority. All the %d waiting jobs have been rescheduled.",count);
		
}
void perfEvaluate(char arr_time[],char exec_time[],char comp_time[]){
	
	int arr;int exec;int comp;
	arr = splitColon(arr_time);
	exec= splitColon(exec_time);
	comp = splitColon(comp_time);
	wait_time = abs(wait_time+(exec-arr));
	turn_time = abs(turn_time + wait_time+(comp-exec));
	
}
int splitColon(char time[]){
	char t[30]="";
	char *token = strtok(time, ":");
	int stoi;
	while (token != NULL)
	{
		strcat(t,token);
		token = strtok(NULL, ":");
	}
	sscanf(t, "%d", &stoi);
	return stoi;
}
int main()
{
	int err1,err2,j;
	printf("Welcome to Sujasri's batch job scheduler version 1.0\nType 'help' to find more about AUBatch commands.");
	err1=pthread_create(&(tid[1]),NULL,scheduling,NULL);
	if(err1 != 0)
		printf("\n Cannot create thread : [%s]",strerror(err1));
	err2=pthread_create(&(tid[2]),NULL,dispatching,NULL);
	if(err2 != 0)
		printf("\n Cannot create thread : [%s]",strerror(err2));
	pthread_mutex_init(&cmd_queue_lock, NULL);
	pthread_cond_init(&cmd_buf_not_full, NULL);
	pthread_cond_init(&cmd_buf_not_empty, NULL);
	pthread_join(tid[1],NULL);
	pthread_join(tid[2],NULL);
	return 0;
}

/*Scheduler thread*/
void *scheduling()
{
	int i,j;
	float avg_turntime,avg_waittime,thro;
	char *input_cmd1;
	char input_cmd[50];
    size_t input_size = 32;
	input_cmd1 = (char *)malloc(input_size * sizeof(char));
		for(i=0;i<MAX_JOB;i++){
		pthread_mutex_lock(&cmd_queue_lock);
        //printf("\nScheduler: count = %d", count);
        while (count == MAX_JOB) {
			/*Waits until the buffer is not full*/
            pthread_cond_wait(&cmd_buf_not_full, &cmd_queue_lock);
        }
		pthread_mutex_unlock(&cmd_queue_lock);
		printf("\n>");		
		getline(&input_cmd1,&input_size,stdin);
		
		strncpy(input_cmd,input_cmd1,strlen(input_cmd1)-1);
		if(strcmp(input_cmd,"help")==0){
			input_help();
		}
	
		else if(strstr(input_cmd,"run")){				
			pthread_mutex_lock(&cmd_queue_lock); 
			input_run(input_cmd);
			if (head == MAX_JOB)
				head = 0;
			
        pthread_cond_signal(&cmd_buf_not_empty);  
        
        pthread_mutex_unlock(&cmd_queue_lock);
		}
		else if(strcmp(input_cmd,"list")==0){
			pthread_mutex_lock(&cmd_queue_lock);
			printf("\nName\tCPU_Time\tPriority\tArrival_time\tProgress");
			for(j=0;j<count;j++){
				printf("\n%s\t%d\t\t%d\t\t%s\tTo be executed",job[j].name,job[j].burst_time,job[j].priority,job[j].arr_time);			
			}	
			pthread_mutex_unlock(&cmd_queue_lock);
		}
		else if(strcmp(input_cmd,"fcfs")==0){
			fcfs();
		}
		else if(strcmp(input_cmd,"sjf")==0){
			pthread_mutex_lock(&cmd_queue_lock);
			sjf();
			pthread_mutex_unlock(&cmd_queue_lock);
		}
		else if(strcmp(input_cmd,"priority")==0){
			pthread_mutex_lock(&cmd_queue_lock);
			priority();
			pthread_mutex_unlock(&cmd_queue_lock);
		}
		else if(strcmp(input_cmd,"quit")==0){
		avg_turntime = turn_time/total;
		avg_waittime = wait_time/total;
		thro = 1.0/avg_turntime;
		printf("\nTotal jobs submitted : %d",total);
		printf("\nAverage waiting time : %f",avg_waittime);
		printf("\nAverage turn around time : %f",avg_turntime);
		printf("\nThroughput : %f",thro);
		exit(0);	
		}
		else{
			
			printf("\n Type help to get the list of commands");
		}
		memset(input_cmd,0,sizeof(input_cmd));
	}
	
	return NULL;
}
/*Dispatcher thread*/
void *dispatching()
{
    
	int i;
	char arg1[20],arg2[20];
	
	 for (i = 0; i <total+1 ; i++) {
		sleep(20); 
        pthread_mutex_lock(&cmd_queue_lock);
        
        //printf("\nDispatcher: count = %d", count);
		
        while (count == 0) {
			/*Waits until the buffer has atleast one job*/
            pthread_cond_wait(&cmd_buf_not_empty, &cmd_queue_lock);
			
        }
        count--;
        
        sprintf(arg1,"%d",job[tail].arg1);
		sprintf(arg2,"%d",job[tail].arg2);
		time_t T1=time(NULL);
		struct tm tm1 = *localtime(&T1);
		sprintf(job[tail].exec_time,"%d:%d:%d",tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
		pid_t forked = fork();
		if (forked==0){
			
			execv("./process",(char*[]){"./process",arg1,arg2,NULL});

		}
		time_t T3=time(NULL);
		sleep(3);
		struct tm tm2 = *localtime(&T3);
		sprintf(job[tail].comp_time,"%d:%d:%d",tm2.tm_hour, tm2.tm_min, tm2.tm_sec);
		
		
		perfEvaluate(job[tail].arr_time,job[tail].exec_time,job[tail].comp_time);		
		
        tail++;
        if (tail == MAX_JOB){
			tail = 0; 
			}

      pthread_cond_signal(&cmd_buf_not_full);
       
      pthread_mutex_unlock(&cmd_queue_lock); 
        
       
	 }
      return NULL;   
}

