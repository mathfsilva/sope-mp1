//File.h
#ifndef FILE_H
#define FILE_H

void write_PROC_CREATE(int fd,char *argv[],double time_taken);
void write_PROC_EXIT(int fd,double time_taken,int exit_code);
//void eventHandler(int code, int argc, char *argv[], char*reg,double time_taken);




#endif
