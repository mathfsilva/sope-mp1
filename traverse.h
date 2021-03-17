#ifndef TRAVERSE_H
#define TRAVERSE_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <locale.h>
#include <ftw.h>
#include <stdbool.h>

#include "traverse.h"
#include "file.h"
#include "xmod.h"


pid_t PID_CURRENT_CHILD;
bool IMPOSSIBLE;
int traverse(int argc, char *argv[],options opts,int no_options);

#endif
