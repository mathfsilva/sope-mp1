//File.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include<time.h>
#include "file.h"
extern clock_t START_TIME;

/*
void eventHandler(int code, int argc, char *argv[], char*reg,double time_taken){
    int fd = open(reg, O_WRONLY|O_SYNC|O_APPEND,0600);
    switch (code){
        case 0: {
            //printf("I'm here\n");
            char t[sizeof(time_taken)]; 
            char const *msg = " PROCESS WITH PID ";
            static int size = sizeof(getpid())/sizeof(char);
            char pid[size];
            //printf("\n%d %d", getpid(), size);
            snprintf(pid, size, "%d\n", getpid());
            snprintf(t,9,"%f",time_taken);
            write(fd,t,sizeof(t)-1);
            write(fd, msg, strlen(msg));
            write(fd, pid,  sizeof(pid)-1);
            for(int j=0;argv[j]!=NULL;j++){
                write(fd," ",sizeof(" ")-1);
                write(fd,argv[j],strlen(argv[j]));
            }
            write(fd,"\n",sizeof("\n")-1);
            break;
        }
        case 1: {
            char t[sizeof(time_taken)]; 
            char const *msg = " PROCESS WITH PID ";
            char const *final= " TERMINATED\n";
            static size_t size = sizeof(getpid())/sizeof(char);
            char pid[size];
            snprintf(pid, size, "%d", getpid());
            snprintf(t,9,"%f",time_taken);
            write(fd,t,sizeof(t)-1);
            write(fd, msg, strlen(msg));
            write(fd, pid, sizeof(pid)-1);
            write(fd, final, strlen(final));
            close(fd);
            break;
        }
    }
}
*/


void getfd(char*reg){
    FD_LOG_FILE= open(reg, O_WRONLY|O_SYNC|O_APPEND,0600);
}

double calculate_time(){
    clock_t end=clock()-START_TIME;
    double time_taken=((double)end) / (CLOCKS_PER_SEC / 1000); // in miliseconds
    return time_taken;
}

void write_PROC_CREATE(char *argv[]){
    double time_taken=calculate_time();
    char t[sizeof(time_taken)]; 
    char const *msg2 = "PROC_CREAT";
    static int size = sizeof(getpid())/sizeof(char);
    char pid[size];
    snprintf(pid, size, "%d\n", getpid());
    snprintf(t,9,"%f",time_taken);

    size_t size2=sizeof(char*)+sizeof(time_taken)+size+strlen(msg2);
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
    double time_taken=calculate_time();
    char t[sizeof(time_taken)]; 
    char exit[sizeof(exit_code)];
    sprintf(exit,"%d",exit_code);
    char const *final= "PROC_EXIT";
    static size_t size = sizeof(getpid())/sizeof(char);
    char pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t,9,"%f",time_taken);

    size_t size2=sizeof(time_taken)+sizeof(exit_code)+strlen(final)+size;
    char *str_final=(char*)malloc(size2*2);
    strcat(str_final,t);
    strcat(str_final," ; ");
    strcat(str_final,pid);
    strcat(str_final," ; ");
    strcat(str_final,final);
    strcat(str_final," ; ");
    strcat(str_final,exit);
    strcat(str_final,"\n");    
    write(FD_LOG_FILE,str_final,strlen(str_final));
    close(FD_LOG_FILE);
    free(str_final);
}


void write_FILE_MODF(char*old_mode,char* new_mode,char*file_name){ 
    double time_taken=calculate_time();
    char t[sizeof(time_taken)];
    static size_t size = sizeof(getpid())/sizeof(char);
    char const *msg="FILE_MODF";
    char const *point=" : ";
    char pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t,9,"%f",time_taken); 

    size_t size2=sizeof(time_taken)+size+strlen(msg)+strlen(point);
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
    double time_taken=calculate_time();
    char t[sizeof(time_taken)];
    char const *msg="SIGNAL_RECV";
    snprintf(t,9,"%f",time_taken); 
    static size_t size = sizeof(getpid())/sizeof(char);

    char pid[size];

    snprintf(pid, size, "%d", getpid());
    size_t size2= sizeof(time_taken)+strlen(msg)+strlen(signal)+size;
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
    double time_taken=calculate_time();
    char t[sizeof(time_taken)];
    static size_t size = sizeof(getpid())/sizeof(char);
    char const *msg="SIGNAL_SENT";
    snprintf(t,9,"%f",time_taken); 
    char pid[size];
    char t_pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t_pid,size,"%d",target_pid);
    size_t size2= sizeof(time_taken)+strlen(msg)+strlen(signal)+size*2;
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