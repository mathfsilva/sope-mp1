//Signals.h
#ifndef SIGNALS_H
#define SIGNALS_H

extern pid_t PID_CURRENT_CHILD;
int subscribe_SIGINT();
int subscribe_osignals();

#endif