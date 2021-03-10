#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>

#include "file.h"
#include "macros.h"

int calculate_mode(perm_mode mode){
    int val = 0;
    val += mode.r ? 4 : 0;
    val += mode.w ? 2 : 0;
    val += mode.x ? 1 : 0;
    return val;
}

double calculate_time(clock_t start){
    clock_t end=clock()-start;
    double time_taken=((double)end) / (CLOCKS_PER_SEC / 1000); // in miliseconds
    return time_taken;
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

char *parse(char *p, char *f, perm_mode* mode_u, perm_mode* mode_g, perm_mode* mode_o)
{
    int size = strlen(p);
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

    perm_mode *temp_mode;

    bool add_or_equal = p[1] == '+' || p[1] == '=';

    if (p[0] != 'a'){

        switch (p[0]){
            case 'u':
                temp_mode = mode_u;
                break;
            case 'g':
                temp_mode = mode_g;
                break;
            case 'o':
                temp_mode = mode_o;
                break;
            default:
            //TODO error
                break;
        }

        for(int i = 2; i < size; i++){
            switch (p[i])
            {
                case 'r':
                    temp_mode->r = add_or_equal;
                    break;
                case 'w':
                    temp_mode->w = add_or_equal;
                    break;
                case 'x':
                    temp_mode->x = add_or_equal;
                    break;
                default:
                //TODO error
                    break;
            }
        }
    }
    else{
        for(int i = 2; i < size; i++){
            switch (p[i])
            {
                case 'r':
                    mode_u->r = add_or_equal;
                    mode_g->r = add_or_equal;
                    mode_o->r= add_or_equal;
                    break;
                case 'w':
                    mode_u->w = add_or_equal;
                    mode_g->w = add_or_equal;
                    mode_o->w = add_or_equal;
                    break;
                case 'x':
                    mode_u->x = add_or_equal;
                    mode_g->x = add_or_equal;
                    mode_o->x = add_or_equal;
                    break;
                default:
                //TODO error
                    break;
            }
        }
    }
    return oldmode_str;
}

int xmod(int argc, char *argv[],int fd,clock_t start)
{

    char *options;
    char*file_name;
    int mode;
    char mode_str[3] = {'0', '0', '0'};
    perm_mode mode_u,mode_g,mode_o;
    char *oldmode;
    if (argv[1][0] == '-' && ((argv[1][1] == 'c') | (argv[1][1] == 'R') | (argv[1][1] == 'v')))
    { //Or argc==4
        options = argv[1];
        printf("%s\n", options);
    }

    //Turn mode (when written in digits) to an octal number in order to call chmod function
    else if (isdigit(argv[1][0]))
    {
        mode = strtol(argv[1], 0, 8);
        oldmode = getoldmode(argv[1], argv[2]);
        
        
        if (chmod(argv[argc - 1], mode) < 0)
        {
            printf("ERROR");
        }
        else{ //FILE_MODF here (reason why went to get oldmode)
            if(strcmp(argv[1],oldmode)!=0){ //Think we only need to write if they are different
            file_name=argv[argc-1];
            double time_taken=calculate_time(start);
            write_FILE_MODF(fd,time_taken,oldmode,argv[1],file_name);
        }
        }
    }
    else
    {
        /*We need to know the current permissions in place because here we 
       are just adding or deleting permissions to certain people, we
       don't change the current permissions for the other people.
       */
        oldmode = parse(argv[1], argv[2], &mode_u,&mode_g,&mode_o);

        int modeu=calculate_mode(mode_u);
        int modeg=calculate_mode(mode_g);
        int modeo=calculate_mode(mode_o);
        mode_str[0] = modeu + '0';
        mode_str[1] = modeg + '0';
        mode_str[2] = modeo + '0';

        mode = strtol(mode_str, 0, 8);
        printf("%s\n", mode_str);

        if (chmod(argv[argc - 1], mode) < 0)
        {
            printf("ERROR");
        }
         else{ //FILE_MODF here (reason why went to get oldmode)
            if(strcmp(mode_str,oldmode)!=0){ //Think we only need to write if they are different
            file_name=argv[argc-1];
            double time_taken=calculate_time(start);
            write_FILE_MODF(fd,time_taken,oldmode,mode_str,file_name);
        }
         }
    }

    if (argc == 4)
    { //In case we have options
        if (isdigit(argv[2][0]))
        {
            mode = strtol(argv[2], 0, 8);
            oldmode = getoldmode(argv[2], argv[3]);

            if (chmod(argv[argc - 1], mode) < 0)
            {
                printf("ERROR");
            }
             else{ //FILE_MODF here (reason why went to get oldmode)
            if(strcmp(argv[2],oldmode)!=0){ //Think we only need to write if they are different
            file_name=argv[argc-1];
            double time_taken=calculate_time(start);
            write_FILE_MODF(fd,time_taken,oldmode,argv[2],file_name);
        }
             }
        }
        else
        {
            oldmode = parse(argv[1], argv[2], &mode_u,&mode_g,&mode_o);

        int modeu=calculate_mode(mode_u);
        int modeg=calculate_mode(mode_g);
        int modeo=calculate_mode(mode_o);
        mode_str[0] = modeu + '0';
        mode_str[1] = modeg + '0';
        mode_str[2] = modeo + '0';

            mode = strtol(mode_str, 0, 8);
            printf("%s\n", mode_str);

            if (chmod(argv[argc - 1], mode) < 0)
            {
                printf("ERROR");
            }
             else{ //FILE_MODF here (reason why went to get oldmode)
            if(strcmp(mode_str,oldmode)!=0){ //Think we only need to write if they are different
            file_name=argv[argc-1];
            double time_taken=calculate_time(start);
            write_FILE_MODF(fd,time_taken,oldmode,mode_str,file_name);
        }
             }
        }
    }
    return 0;
}

bool aretheyequal(char *env,char const *arg){
    for(int i = 0; arg[i] != '\0'; i++){
        if(arg[i] != env[i]){
            return false;
        }
    }
    //printf("Holo\n");
    return true;
}

char *checkLog(char *envp[])
{
     //Check if LOG_FILENAME was defined by user
    for(int j=0;envp[j]!=NULL;j++){
        //printf("%s\n",envp[j]);
        if(aretheyequal(envp[j],"LOG_FILENAME")){
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
    
    clock_t start;
    double time_taken;
    start = clock();
    char *reg=checkLog(envp);
    int fd = open(reg, O_WRONLY|O_SYNC|O_APPEND,0600);
    time_taken=calculate_time(start);
    //It's gonna have a PROC_CREAT here (only PROC_CREAT right now-->because we only have one process)
    //eventHandler(0, argc, argv, reg,time_taken);
    write_PROC_CREATE(fd,argv,time_taken);




    if (argc < 3)
    {   //chmod options permissions file_name
        /*If no options are specified, chmod modifies the permissions of the file 
                specified by file name to the permissions specified by permissions.
                So it's possible to have only 3 arguments--->xmod, permissions, file_name*/
        printf("Not enough arguments\n");
        time_taken=calculate_time(start);
        write_PROC_EXIT(fd,time_taken,1);
        //PROC_EXIT here
        return 1;
    }

    
    checkSymlink(argc, argv);

    if (xmod(argc, argv,fd,start))
    {
        return 1;
    }

    time_taken=calculate_time(start);
    write_PROC_EXIT(fd,time_taken,0);
    //Should have a PROC_EXIT here
    //eventHandler(1, argc, argv, reg,time_taken);
    return 0;
}