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
#include <time.h>

#include "file.h"
#include "signals.h"
#include "macros.h"

int calculate_mode(perm_mode mode)
{
    int val = 0;
    val += mode.r ? 4 : 0;
    val += mode.w ? 2 : 0;
    val += mode.x ? 1 : 0;
    return val;
}

char *getoldmode(char *p, char *f)
{
    char *oldmode_str = (char *)malloc(3);
    struct stat fs;
    stat(f, &fs); //Gets current permission

    //User Permission

    perm_mode old_mode_u;
    perm_mode old_mode_g;
    perm_mode old_mode_o;

    old_mode_u.r = fs.st_mode & S_IRUSR;
    old_mode_u.w = fs.st_mode & S_IWUSR;
    old_mode_u.x = fs.st_mode & S_IXUSR;

    old_mode_g.r = fs.st_mode & S_IRGRP;
    old_mode_g.w = fs.st_mode & S_IWGRP;
    old_mode_g.x = fs.st_mode & S_IXGRP;

    old_mode_o.r = fs.st_mode & S_IROTH;
    old_mode_o.w = fs.st_mode & S_IWOTH;
    old_mode_o.x = fs.st_mode & S_IXOTH;

    int old_u = calculate_mode(old_mode_u);
    int old_g = calculate_mode(old_mode_g);
    int old_o = calculate_mode(old_mode_o);

    oldmode_str[0] = old_u + '0';
    oldmode_str[1] = old_g + '0';
    oldmode_str[2] = old_o + '0';

    return oldmode_str;
}

char *parse(char *p, char *f, perm_mode *mode_u, perm_mode *mode_g, perm_mode *mode_o)
{
    int size = strlen(p);

    if (size > 5){
        perror("Wrong input for mode\n");
        exit(-1); //TODO change this
    }

    char *oldmode_str = (char *)malloc(3);
    struct stat fs;
    stat(f, &fs); //Gets current permission

    //User Permission

    mode_u->r = fs.st_mode & S_IRUSR;
    mode_u->w = fs.st_mode & S_IWUSR;
    mode_u->x = fs.st_mode & S_IXUSR;

    mode_g->r = fs.st_mode & S_IRGRP;
    mode_g->w = fs.st_mode & S_IWGRP;
    mode_g->x = fs.st_mode & S_IXGRP;

    mode_o->r = fs.st_mode & S_IROTH;
    mode_o->w = fs.st_mode & S_IWOTH;
    mode_o->x = fs.st_mode & S_IXOTH;

    int old_u = calculate_mode(*mode_u);
    int old_g = calculate_mode(*mode_g);
    int old_o = calculate_mode(*mode_o);

    oldmode_str[0] = old_u + '0';
    oldmode_str[1] = old_g + '0';
    oldmode_str[2] = old_o + '0';

    //Let's consider letters option first (u+w)
    //User

    bool add_or_equal = p[1] == '+' || p[1] == '=';

    perm_mode *modes[3];
    
    switch (p[0]){
        case 'u':
            modes[0] = mode_u;
            break;
        case 'g':
            modes[0] = mode_g;
            break;
        case 'o':
            modes[0] = mode_o;
            break;
        case 'a':
            modes[0] =  mode_u;
            modes[1] =  mode_g;
            modes[2] =  mode_o;
        default:
        //TODO error
            break;
    }

    for (int i = 0; i < (p[0] == 'a' ? 3 : 1); i++){
        if(p[1] == '='){
            modes[i]->r = 0;
            modes[i]->w = 0;
            modes[i]->x = 0;
        }

        int r = 0, w = 0, x = 0; 

        for(int j = 2; j < size; j++){
            switch (p[j])
            {
                case 'r':
                    r++;
                    modes[i]->r = add_or_equal;
                    break;
                case 'w':
                    w++;
                    modes[i]->w = add_or_equal;
                    break;
                case 'x':
                    x++;
                    modes[i]->x = add_or_equal;
                    break;
                default:
                //TODO error
                break;
            }
        }

        if(r > 1 || w > 1 || x > 1){
            perror("Gave the same mode more than once");
            return NULL; //TODO change this
        }
    }

    return oldmode_str;
}

void print_options(options opts){
    printf("Option v: %d\n", opts.v);
    printf("Option c: %d\n", opts.c);
    printf("Option R: %d\n", opts.R);
}

int get_options(int argc, char *argv[], options* opts){

    int i;

    int ret = 0;

    if(argc < 3){ //not needed
        perror("Too few args\n");
        return -1;
    }

    for(i = 1; argv[i][0] == '-'; i++){
        if (!(i < argc-2)){
            perror("Too few args\n");
            return -1;
        }
        //for(int j = 1; argv[i][j] != '\0'; j++){
            int j = 1;
            switch (argv[i][j])
            {
            case 'c':
                if(opts->c){
                    perror("Invalid, repeated options\n");
                    return -1;
                }
                else{
                    ret++;
                    opts->c = 1;
                }
                break;
            case 'v':
                if(opts->v){
                    perror("Invalid, repeated options\n");
                    return -1;
                }
                else{
                    ret++;
                    opts->v = 1;
                }
                break;
            case 'R':
                if(opts->R){
                    perror("Invalid, repeated options\n");
                    return -1;
                }
                else{
                    ret++;
                    opts->R = 1;
                }
                break;
            default:
                perror("Invalid options\n");
                return -1;
                break;
            }
            if(argv[i][j+1] != '\0'){
                perror("Invalid options\n");
                return -1;
            }
        //}
    }    

    printf("Done reading options\n");

    return ret;
}

int xmod(int argc, char *argv[])
{

    char*file_name;
    int mode;
    char mode_str[3] = {'0', '0', '0'};
    perm_mode mode_u, mode_g, mode_o;
    char *oldmode;

    options opts;

    opts.c = 0;
    opts.v = 0;
    opts.R = 0;

    struct stat st_buf;
    stat (argv[argc - 1], &st_buf);

    if (S_ISREG (st_buf.st_mode)) {
        printf ("%s is a regular file.\n", argv[argc - 1]);
    } else if (S_ISDIR (st_buf.st_mode)) {
        printf ("%s is a directory.\n", argv[argc - 1]);
    }

    int no_options = get_options(argc, argv, &opts);

    if (no_options == -1){
        perror("Error occurred while reading options\n");
        write_PROC_EXIT(1);
        return 1;
    }

    print_options(opts);

    //Turn mode (when written in digits) to an octal number in order to call chmod function
    if (isdigit(argv[1+no_options][0]))
    {

        mode = strtol(argv[1+no_options], 0, 8);
        oldmode = getoldmode(argv[1+no_options], argv[2+no_options]);
        
        if (oldmode == NULL) return 1;
    }
    else
    {
        /*We need to know the current permissions in place because here we 
       are just adding or deleting permissions to certain people, we
       don't change the current permissions for the other people.
       */
        oldmode = parse(argv[1+no_options], argv[2+no_options], &mode_u,&mode_g,&mode_o);
        
        if (oldmode == NULL) return 1;

        int modeu = calculate_mode(mode_u);
        int modeg = calculate_mode(mode_g);
        int modeo = calculate_mode(mode_o);
        mode_str[0] = modeu + '0';
        mode_str[1] = modeg + '0';
        mode_str[2] = modeo + '0';

        mode = strtol(mode_str, 0, 8);
    }

    if (chmod(argv[argc - 1], mode) < 0)
    {
        printf("ERROR");
    }
    else{ //FILE_MODF here (reason why went to get oldmode)
        if(mode_str[0]!='0'){
           if(strcmp(mode_str,oldmode)!=0){ //Think we only need to write if they are different
                file_name=argv[argc-1];
                write_FILE_MODF(oldmode,mode_str,file_name);
            }
        }
        else{
        if(strcmp(argv[1+no_options],oldmode)!=0){ //Think we only need to write if they are different
            file_name=argv[argc-1];
            write_FILE_MODF(oldmode,argv[1+no_options],file_name);
        }
        }
    }
    return 0;
}

bool aretheyequal(char *env, char const *arg)
{
    for (int i = 0; arg[i] != '\0'; i++)
    {
        if (arg[i] != env[i])
        {
            return false;
        }
    }
    //printf("Holo\n");
    return true;
}

char *checkLog(char *envp[])
{
    printf("Hi\n");
    //Check if LOG_FILENAME was defined by user
    for (int j = 0; envp[j] != NULL; j++)
    {
        //printf("%s\n",envp[j]);
        if (aretheyequal(envp[j], "LOG_FILENAME"))
        {
            char *reg = getenv("LOG_FILENAME");
            //printf("%s\n",reg);

            int fd;
            //char const *text1 = "Holo Pat"; //Experiment

            if (access(reg, F_OK) == 0)
            { //When file exists->Truncate it
                printf("File exists\n");
                fd = open(reg, O_CREAT | O_TRUNC | O_WRONLY, 0600);
                //write(fd, text1, 8);
                close(fd);
            }
            else
            {
                //If file doesn't exist->Create a new one
                printf("File doesn't exist\n");
                //printf("%s\n",reg);
                fd = open(reg, O_CREAT | O_EXCL, 0644);
                //write(fd, text1, 8);
                close(fd);
            }
            return reg;
        }
    }
    return NULL;
}

void checkSymlink(int argc, char *argv[])
{
    //Symlink check.
    struct stat buffer;
    lstat(argv[argc - 1], &buffer);
    if (S_ISLNK(buffer.st_mode))
    {
        char buf[PATH_MAX]; /* PATH_MAX includes the \0 so +1 is not required */
        char *res = realpath(argv[argc - 1], buf);

        if (res == NULL)
        {
            perror("Couldn't find the real path of a symlink.");
        }
        else
        {
            //printf("Hihi, it worked!!!!\n");
            memcpy(argv[argc - 1], buf, strlen(buf));
            //printf("NOVO CAMINHO: %s\n", argv[argc - 1]);
        }
    }
}

int main(int argc, char *argv[], char *envp[])
{
    START_TIME = clock();
    char *reg = checkLog(envp);
    getfd(reg);
    //It's gonna have a PROC_CREAT here (only PROC_CREAT right now-->because we only have one process)
    //eventHandler(0, argc, argv, reg,time_taken);
    write_PROC_CREATE(argv);
    subscribe_SIGINT(); //Ctrl+C interruption
    printf("Back\n");
    //sleep(4);
    printf("Awake\n");

    if (argc < 3)
    { //chmod options permissions file_name
        /*If no options are specified, chmod modifies the permissions of the file 
                specified by file name to the permissions specified by permissions.
                So it's possible to have only 3 arguments--->xmod, permissions, file_name*/
        printf("Not enough arguments\n");
        write_PROC_EXIT(1);
        //PROC_EXIT here
        return 1;
    }
    
    checkSymlink(argc, argv);

    if (xmod(argc, argv))
    {
        return 1;
    }

    write_PROC_EXIT(0);
    //Should have a PROC_EXIT here
    //eventHandler(1, argc, argv, reg,time_taken);
    return 0;
}