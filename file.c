//File.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "file.h"

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

void write_PROC_CREATE(int fd,char *argv[],double time_taken){
    char t[sizeof(time_taken)]; 
    char const *msg = " PROCESS WITH PID ";
    char const *msg2 = " PROC_CREAT ";
    static int size = sizeof(getpid())/sizeof(char);
    char pid[size];
    //printf("\n%d %d", getpid(), size);
    snprintf(pid, size, "%d\n", getpid());
    snprintf(t,9,"%f",time_taken);
    write(fd,t,sizeof(t)-1);
    write(fd, msg, strlen(msg));
    write(fd, pid,  sizeof(pid)-1);
    write(fd,msg2,strlen(msg2));
    for(int j=0;argv[j]!=NULL;j++){
        write(fd," ",sizeof(" ")-1);
        write(fd,argv[j],strlen(argv[j]));
    }
    write(fd,"\n",sizeof("\n")-1);
}

void write_PROC_EXIT(int fd,double time_taken,int exit_code){
    char t[sizeof(time_taken)]; 
    char exit[1]={'0'};
    exit[0]=exit_code +'0';
    char const *msg = " PROCESS WITH PID ";
    char const *final= " PROC_EXIT WITH EXIT STATUS ";
    static size_t size = sizeof(getpid())/sizeof(char);
    char pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t,9,"%f",time_taken);
    write(fd,t,sizeof(t)-1);
    write(fd, msg, strlen(msg));
    write(fd, pid, sizeof(pid)-1);
    write(fd, final, strlen(final));
    write(fd,exit,sizeof(exit));
    close(fd);
}


void write_FILE_MODF(int fd,double time_taken,char*old_mode,char* new_mode,char*file_name){ 
    char t[sizeof(time_taken)];
    static size_t size = sizeof(getpid())/sizeof(char);
    char const *msg=" FILE_MODF ";
    char const *msg1 = " PROCESS WITH PID ";
    char const *point=" : ";
    char pid[size];
    snprintf(pid, size, "%d", getpid());
    snprintf(t,9,"%f",time_taken); 
    write(fd,t,sizeof(t)-1);
    write(fd,msg1,strlen(msg1));
    write(fd,pid,sizeof(pid)-1);
    write(fd,msg,strlen(msg));
    write(fd,file_name,strlen(file_name));
    write(fd,point,strlen(point));
    write(fd,old_mode,strlen(old_mode));
    write(fd,point,strlen(point));
    write(fd,new_mode,strlen(new_mode));
    write(fd,"\n",sizeof("\n")-1);


}
