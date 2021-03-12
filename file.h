//File.h
#ifndef FILE_H
#define FILE_H


#include <sys/types.h>
#include <unistd.h>

int FD_LOG_FILE;

void write_PROC_CREATE(char *argv[]);
void write_PROC_EXIT(int exit_code);
void write_FILE_MODF(char*old_mode,char* new_mode,char*file_name);
void write_SIGNAL_RECV(char *signal);
void write_SIGNAL_SENT(char *signal,pid_t target_pid);
double calculate_time();
//void eventHandler(int code, int argc, char *argv[], char*reg,double time_taken);

void getfd(char*reg);



#endif
