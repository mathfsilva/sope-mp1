//File.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include<sys/time.h>
#include "file.h"
extern struct timeval START_TIME;


void getfd(char*reg){
    FD_LOG_FILE= open(reg, O_WRONLY|O_SYNC|O_APPEND,0600);
}

long double calculate_time(){
    struct timeval end;
    gettimeofday(&end, NULL);
    struct timeval temp_diff;

  

  temp_diff.tv_sec =end.tv_sec -START_TIME.tv_sec ;
  temp_diff.tv_usec=end.tv_usec-START_TIME.tv_usec;

  /* Using while instead of if below makes the code slightly more robust. */

  while(temp_diff.tv_usec<0)
  {
    temp_diff.tv_usec+=1000000;
    temp_diff.tv_sec -=1;
  }

  return (1000000LL*temp_diff.tv_sec+temp_diff.tv_usec);

} 

void write_PROC_CREATE(char *argv[]){
    long double time_taken=calculate_time();
    time_taken/=1000;
    static int sizet=32;
    char t[sizet]; 
    char const *msg2 = "PROC_CREAT";
    static int size =20;
    char pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t,sizet,"%Lf",time_taken);

    size_t size2=sizeof(char*)+sizet+size+strlen(msg2);
    char *str_final=(char*)malloc(size2*2);
    strcat(str_final,t);
    strcat(str_final," ; ");
    strcat(str_final,pid);
    strcat(str_final," ; ");
    strcat(str_final,msg2);
    strcat(str_final," ; ");
    for(int j=0;argv[j]!=NULL;j++){
        strcat(str_final,argv[j]);
        strcat(str_final," ");
    }
    strcat(str_final,"\n");
    write(FD_LOG_FILE,str_final,strlen(str_final));
    free(str_final);
}

void write_PROC_EXIT(int exit_code){
    long double time_taken=calculate_time();
    time_taken/=1000;
    static size_t sizet=32;
    static size_t sizee=1;
    char t[sizet]; 
    char exit[sizee];
    sprintf(exit,"%d",exit_code);
    char const *final= "PROC_EXIT";
    static size_t size = 20;
    char pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t,sizet,"%Lf",time_taken);

    size_t size2=sizet+sizee+strlen(final)+size;
    char *str_final=(char*)malloc(size2*4);
    /*strcat(str_final,t);
    strcat(str_final," ; ");
    strcat(str_final,pid);
    strcat(str_final," ; ");
    strcat(str_final,final);
    strcat(str_final," ; ");
    strcat(str_final,exit);
    strcat(str_final,"\n"); */  

    snprintf(str_final,sizet,"%s",t);
    snprintf(str_final,strlen(" ; "),"%s"," ; ");
    snprintf(str_final,size,"%s",pid);
    snprintf(str_final,strlen(" ; "),"%s"," ; ");
    snprintf(str_final,strlen(final),"%s",final);
        snprintf(str_final,strlen(" ; "),"%s"," ; ");
    snprintf(str_final,strlen(exit),"%s",exit);
    snprintf(str_final,strlen("\n"),"%s","\n");
     write(FD_LOG_FILE,str_final,strlen(str_final));
    close(FD_LOG_FILE);
    free(str_final);
}


void write_FILE_MODF(char*old_mode,char* new_mode,char*file_name){ 
    long double time_taken=calculate_time();
    time_taken/=1000;
    static size_t sizet=32;
    char t[sizet];
    static size_t size = 20;
    char const *msg="FILE_MODF";
    char const *point=" : ";
    char pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t,sizet,"%Lf",time_taken); 

    size_t size2=sizet+size+strlen(msg)+strlen(point);
    char* str_final=(char*)malloc(size2*2);

    strcat(str_final,t);
    strcat(str_final, " ; ");
    strcat(str_final,pid);
    strcat(str_final, " ; ");
    strcat(str_final,msg);
    strcat(str_final," ; ");
    strcat(str_final,file_name);
    strcat(str_final,point);
    strcat(str_final,old_mode);
    strcat(str_final,point);
    strcat(str_final,new_mode);
    strcat(str_final,"\n");
    write(FD_LOG_FILE,str_final,strlen(str_final));
    free(str_final);
}

void write_SIGNAL_RECV(char *signal){
    long double time_taken=calculate_time();
    time_taken/=1000;
    static size_t sizet=32;
    char t[sizet];
    char const *msg="SIGNAL_RECV";
    snprintf(t,sizet,"%Lf",time_taken); 
    static size_t size = 20;

    char pid[size];

    snprintf(pid, size, "%d", getpid());
    size_t size2= sizet+strlen(msg)+strlen(signal)+size;
    char *str_final=(char*)malloc(size2*2);
    strcat(str_final,t);
    strcat(str_final," ; ");
    strcat(str_final,pid);
    strcat(str_final," ; ");
    strcat(str_final,msg);
    strcat(str_final," ; ");
    strcat(str_final,signal);
    strcat(str_final,"\n");
    write(FD_LOG_FILE,str_final,strlen(str_final));
    free(str_final);
}

void write_SIGNAL_SENT(char *signal,pid_t target_pid){
    long double time_taken=calculate_time();
    time_taken/=1000;
    static size_t sizet=32;
    char t[sizet];
    static size_t size = 20;
    char const *msg="SIGNAL_SENT";
    snprintf(t,sizet,"%Lf",time_taken); 
    char pid[size];
    char t_pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t_pid,size,"%d",target_pid);
    size_t size2= sizet+strlen(msg)+strlen(signal)+size*2;
    char *str_final=(char*)malloc(size2*2);

    strcat(str_final,t);
    strcat(str_final," ; ");
    strcat(str_final,pid);
    strcat(str_final," ; ");
    strcat(str_final,msg);
    strcat(str_final," ; ");
    strcat(str_final,signal);
    strcat(str_final," : ");
    strcat(str_final,t_pid);
    strcat(str_final,"\n");
    write(FD_LOG_FILE,str_final,strlen(str_final));
    free(str_final);

}