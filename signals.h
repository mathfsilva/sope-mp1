//Signals.h
#ifndef SIGNALS_H
#define SIGNALS_H

extern pid_t PID_CURRENT_CHILD;
void signals_handler(int signo);
void subscribe_SIGINT();

#endif