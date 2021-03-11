//File.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include<time.h>
#include "file.h"
extern clock_t start;

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
    global_fd= open(reg, O_WRONLY|O_SYNC|O_APPEND,0600);
}

double calculate_time(){
    clock_t end=clock()-start;
    double time_taken=((double)end) / (CLOCKS_PER_SEC / 1000); // in miliseconds
    return time_taken;
}

void write_PROC_CREATE(char *argv[]){
    double time_taken=calculate_time();
    char t[sizeof(time_taken)]; 
    char const *msg = " PROCESS WITH PID ";
    char const *msg2 = " PROC_CREAT ";
    static int size = sizeof(getpid())/sizeof(char);
    char pid[size];
    //printf("\n%d %d", getpid(), size);
    snprintf(pid, size, "%d\n", getpid());
    snprintf(t,9,"%f",time_taken);
    write(global_fd,t,sizeof(t)-1);
    write(global_fd, msg, strlen(msg));
    write(global_fd, pid,  sizeof(pid)-1);
    write(global_fd,msg2,strlen(msg2));
    for(int j=0;argv[j]!=NULL;j++){
        write(global_fd," ",sizeof(" ")-1);
        write(global_fd,argv[j],strlen(argv[j]));
    }
    write(global_fd,"\n",sizeof("\n")-1);
}

void write_PROC_EXIT(int exit_code){
    double time_taken=calculate_time();
    char t[sizeof(time_taken)]; 
    char exit[1]={'0'};
    exit[0]=exit_code +'0';
    char const *msg = " PROCESS WITH PID ";
    char const *final= " PROC_EXIT WITH EXIT STATUS ";
    static size_t size = sizeof(getpid())/sizeof(char);
    char pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t,9,"%f",time_taken);
    write(global_fd,t,sizeof(t)-1);
    write(global_fd, msg, strlen(msg));
    write(global_fd, pid, sizeof(pid)-1);
    write(global_fd, final, strlen(final));
    write(global_fd,exit,sizeof(exit));
    close(global_fd);
}


void write_FILE_MODF(char*old_mode,char* new_mode,char*file_name){ 
    double time_taken=calculate_time();
    char t[sizeof(time_taken)];
    static size_t size = sizeof(getpid())/sizeof(char);
    char const *msg=" FILE_MODF ";
    char const *msg1 = " PROCESS WITH PID ";
    char const *point=" : ";
    char pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t,9,"%f",time_taken); 
    write(global_fd,t,sizeof(t)-1);
    write(global_fd,msg1,strlen(msg1));
    write(global_fd,pid,sizeof(pid)-1);
    write(global_fd,msg,strlen(msg));
    write(global_fd,file_name,strlen(file_name));
    write(global_fd,point,strlen(point));
    write(global_fd,old_mode,strlen(old_mode));
    write(global_fd,point,strlen(point));
    write(global_fd,new_mode,strlen(new_mode));
    write(global_fd,"\n",sizeof("\n")-1);
}

void write_SIGNAL_RECV(char *signal){
    double time_taken=calculate_time();
    char t[sizeof(time_taken)];
    char const *msg=" SIGNAL_RECV ";
    snprintf(t,9,"%f",time_taken); 
    write(global_fd,t,sizeof(t)-1);
    write(global_fd,msg,strlen(msg));
    write(global_fd,signal,strlen(signal));
    write(global_fd,"\n",sizeof("\n")-1);
}

void write_SIGNAL_SENT(char *signal){
    double time_taken=calculate_time();
    char t[sizeof(time_taken)];
    static size_t size = sizeof(getpid())/sizeof(char);
    char const *msg=" SIGNAL_SENT ";
    snprintf(t,9,"%f",time_taken); 
    char pid[size];
    snprintf(pid, size, "%d", getpid());
    write(global_fd,t,sizeof(t)-1);
    write(global_fd,msg,strlen(msg));
    write(global_fd,signal,strlen(signal));
    write(global_fd," : ",sizeof(" : ")-1);
    write(global_fd,pid,sizeof(pid)-1);
    write(global_fd,"\n",sizeof("\n")-1);
}