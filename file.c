//File.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "file.h"

#define atoa(x) #x

void eventHandler(int code, int argc, char *argv[], int fd){
    switch (code){
        case 0:
            char msg[] = "PROCESS CREATED WITH PID ";
            static size_t size = sizeof(getpid())/sizeof(char);
            char pid[size];
            printf("\n%d %d", getpid(), size);
            snprintf(pid, size, "%d", getpid());
            write(fd, msg, sizeof(msg));
            write(fd, pid, sizeof(pid));
            break;
    }
}
