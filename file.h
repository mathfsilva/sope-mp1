//File.h
#ifndef FILE_H
#define FILE_H


#include <sys/types.h>
#include <unistd.h>

int FD_LOG_FILE;
extern pid_t PID_CURRENT_CHILD;

int write_PROC_CREATE(char *argv[]);
int write_PROC_EXIT(int exit_code);
int write_FILE_MODF(char*old_mode,char* new_mode,char*file_name);
int write_SIGNAL_RECV(char *signal);
int write_SIGNAL_SENT(char *signal,pid_t target_pid);
void calculate_time(long double *time_taken);
//void eventHandler(int code, int argc, char *argv[], char*reg,double time_taken);

int getfd(char*reg);



#endif
