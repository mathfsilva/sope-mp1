//File.h
#ifndef FILE_H
#define FILE_H

void write_PROC_CREATE(int fd,char *argv[],double time_taken);
void write_PROC_EXIT(int fd,double time_taken,int exit_code);
void write_FILE_MODF(int fd,double time_taken,char*old_mode,char* new_mode,char*file_name);
void write_SIGNAL_RECV(int fd,double time_taken,char *signal);
void write_SIGNAL_SENT(int fd,double time_taken,char *signal);
//void eventHandler(int code, int argc, char *argv[], char*reg,double time_taken);

int getfd(char*reg);



#endif
