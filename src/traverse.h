#ifndef TRAVERSE_H_
#define TRAVERSE_H_

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

#include "./xmod.h"

pid_t PID_CURRENT_CHILD;
bool IMPOSSIBLE;
int traverse(int argc, char *argv[],options opts,int no_options);

#endif // TRAVERSE_H_
