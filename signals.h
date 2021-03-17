//Signals.h
#ifndef SIGNALS_H
#define SIGNALS_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "traverse.h"
#include "xmod.h"

extern pid_t PID_CURRENT_CHILD;
int subscribe_SIGINT();
int subscribe_osignals();

#endif