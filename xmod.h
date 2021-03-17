#ifndef XMOD_H_
#define XMOD_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/time.h>


int nftot, nfmod;
char *global_file_path;


struct timeval START_TIME;
int MODE;

typedef struct 
{
    bool r;
    bool w;
    bool x;
}perm_mode;

typedef struct{
    bool v;
    bool c;
    bool R;
}options;

int xmod(int argc, char *argv[],options opts,int no_options);

#endif // XMOD_H_
