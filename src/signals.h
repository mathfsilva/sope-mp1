#ifndef SIGNALS_H_
#define SIGNALS_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int processGroup;
void setProcessGroup();
void nothing(int signo);
extern pid_t PID_CURRENT_CHILD;
int subscribe_SIGINT();
int subscribe_osignals();

#endif // SIGNALS_H_
