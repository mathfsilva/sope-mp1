//File.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "file.h"


#define atoa(x) #x

void eventHandler(int code, int argc, char *argv[], char*reg){
    switch (code){
        case 0:
            char msg[] = "PROCESS CREATED WITH PID ";
            static int size = sizeof(getpid())/sizeof(char);
            char pid[size];
            printf("\n%d %d", getpid(), size);
            snprintf(pid, size, "%d\n", getpid());
            int fd = open(reg, O_CREAT | O_TRUNC | O_WRONLY, 0600);
            write(fd, msg, sizeof(msg));
            write(fd, pid, size);
            close(fd);
            break;
    }
}
