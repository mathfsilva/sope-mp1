//File.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "file.h"

void eventHandler(int code, int argc, char *argv[], char*reg){
    int fd = open(reg, O_CREAT | O_TRUNC | O_WRONLY, 0600);
    switch (code){
        case 0: {
            char msg[] = "PROCESS CREATED WITH PID ";
            static int size = sizeof(getpid())/sizeof(char);
            char pid[size];
            printf("\n%d %d", getpid(), size);
            snprintf(pid, size, "%d\n", getpid());
            write(fd, msg, sizeof(msg));
            write(fd, pid, size);
            break;
        }
        case 1: {
            char msg[] = "PROCESS WITH PID ";
            char final[] = " TERMINATED\n";
            static size_t size = sizeof(getpid())/sizeof(char);
            char pid[size];
            snprintf(pid, size, "%d", getpid());
            write(fd, msg, sizeof(msg));
            write(fd, pid, sizeof(pid));
            write(fd, final, sizeof(final));
            break;
        }
    }
}
