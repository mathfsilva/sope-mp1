#ifndef XMOD_H
#define XMOD_H

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

#endif
