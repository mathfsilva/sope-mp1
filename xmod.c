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

#include "file.h"
#include "signals.h"
#include "traverse.h"
#include "xmod.h"

void calculate_mode(perm_mode mode, int *val)
{
    *val += mode.r ? 4 : 0;
    *val += mode.w ? 2 : 0;
    *val += mode.x ? 1 : 0;
}

void getnewmodeletters(char *p, char *newml)
{
    for (int i = 1; i < 4; i++)
    {
        int val = p[i] - '0';

        if ((val - 4) >= 0)
            strcat(newml, "r");
        else
            strcat(newml, "-");

        val -= 4;

        if ((val - 2) >= 0)
            strcat(newml, "w");
        else
            strcat(newml, "-");

        val -= 2;

        if ((val - 1) >= 0)
            strcat(newml, "x");
        else
            strcat(newml, "-");
    }
}

int getoldmodeletters(char *p, char *f, char *oldml)
{
    struct stat fs;
    if (stat(f, &fs) == -1)
    {
        return 1;
    }

    if (fs.st_mode & S_IRUSR)
    {
        strcat(oldml, "r");
    }
    else
    {
        strcat(oldml, "-");
    }

    if (fs.st_mode & S_IWUSR)
    {
        strcat(oldml, "w");
    }
    else
    {
        strcat(oldml, "-");
    }

    if (fs.st_mode & S_IXUSR)
    {
        strcat(oldml, "x");
    }
    else
    {
        strcat(oldml, "-");
    }

    if (fs.st_mode & S_IRGRP)
    {
        strcat(oldml, "r");
    }
    else
    {
        strcat(oldml, "-");
    }

    if (fs.st_mode & S_IWGRP)
    {
        strcat(oldml, "w");
    }
    else
    {
        strcat(oldml, "-");
    }

    if (fs.st_mode & S_IXGRP)
    {
        strcat(oldml, "x");
    }
    else
    {
        strcat(oldml, "-");
    }

    if (fs.st_mode & S_IROTH)
    {
        strcat(oldml, "r");
    }
    else
    {
        strcat(oldml, "-");
    }

    if (fs.st_mode & S_IWOTH)
    {
        strcat(oldml, "w");
    }
    else
    {
        strcat(oldml, "-");
    }

    if (fs.st_mode & S_IXOTH)
    {
        strcat(oldml, "x");
    }
    else
    {
        strcat(oldml, "-");
    }
    return 0;
}

int getoldmode(char *p, char *f, char *oldmode_str)
{
    struct stat fs;
    if (stat(f, &fs) == -1)
    {
        return 1;
    } //Gets current permission

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

    int old_u = 0, old_g = 0, old_o = 0;
    calculate_mode(old_mode_u, &old_u);
    calculate_mode(old_mode_g, &old_g);
    calculate_mode(old_mode_o, &old_o);

    oldmode_str[0] = '0';
    oldmode_str[1] = old_u + '0';
    oldmode_str[2] = old_g + '0';
    oldmode_str[3] = old_o + '0';
    return 0;
}

int parse(char *p, char *f, perm_mode *mode_u, perm_mode *mode_g, perm_mode *mode_o, char *oldmode_str)
{
    int size = strlen(p);

    if (size > 5)
    {
        perror("Wrong input for mode\n");
        return 1;
    }

    struct stat fs;
    if (stat(f, &fs) == -1)
    {
        return 1;
    } //Gets current permission

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

    int old_u = 0, old_g = 0, old_o = 0;
    calculate_mode(*mode_u, &old_u);
    calculate_mode(*mode_g, &old_g);
    calculate_mode(*mode_o, &old_o);

    oldmode_str[0] = '0';
    oldmode_str[1] = old_u + '0';
    oldmode_str[2] = old_g + '0';
    oldmode_str[3] = old_o + '0';

    //Let's consider letters option first (u+w)
    //User

    bool add_or_equal = p[1] == '+' || p[1] == '=';

    perm_mode *modes[3];

    switch (p[0])
    {
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
        modes[0] = mode_u;
        modes[1] = mode_g;
        modes[2] = mode_o;
    default:
        return 1;
        break;
    }

    for (int i = 0; i < (p[0] == 'a' ? 3 : 1); i++)
    {
        if (p[1] == '=')
        {
            modes[i]->r = 0;
            modes[i]->w = 0;
            modes[i]->x = 0;
        }

        int r = 0, w = 0, x = 0;

        for (int j = 2; j < size; j++)
        {
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
                return 1;
                break;
            }
        }

        if (r > 1 || w > 1 || x > 1)
        {
            perror("Gave the same mode more than once");
            return 1;
        }
    }

    return 0;
}

void print_options(options opts)
{
    printf("Option v: %d\n", opts.v);
    printf("Option c: %d\n", opts.c);
    printf("Option R: %d\n", opts.R);
}

int get_options(int argc, char *argv[], options *opts, int *ret)
{

    int i;

    if (argc < 3)
    { //not needed
        perror("Too few args\n");
        return 1;
    }

    for (i = 1; argv[i][0] == '-'; i++)
    {
        if (!(i < argc - 2))
        {
            perror("Too few args\n");
            return 1;
        }
        //for(int j = 1; argv[i][j] != '\0'; j++){
        int j = 1;
        switch (argv[i][j])
        {
        case 'c':
            if (opts->c)
            {
                perror("Invalid, repeated options\n");
                return 1;
            }
            else
            {
                *ret += 1;
                opts->c = 1;
            }
            break;
        case 'v':
            if (opts->v)
            {
                perror("Invalid, repeated options\n");
                return 1;
            }
            else
            {
                *ret += 1;
                opts->v = 1;
            }
            break;
        case 'R':
            if (opts->R)
            {
                perror("Invalid, repeated options\n");
                return 1;
            }
            else
            {
                *ret += 1;
                opts->R = 1;
            }
            break;
        default:
            perror("Invalid options\n");
            return 1;
            break;
        }
        if (argv[i][j + 1] != '\0')
        {
            perror("Invalid options\n");
            return 1;
        }
        //}
    }

    //printf("Done reading options\n");

    return 0;
}

int xmod(int argc, char *argv[],options opts, int no_options)
{

    //printf("XMOD of %d given path is %s\n", getpid(), argv[argc-1]);

    char *canonical_path = realpath(argv[argc-1], NULL); // pased to chmod()
    if (canonical_path == NULL) {
        perror("");
        return 1;
    }

    char *path_used_shell = argv[argc-1]; // usefull for prints
    int mode;
    char *mode_str = (char *)malloc(4);
    perm_mode mode_u, mode_g, mode_o;
    char *oldmode = (char *)malloc(5);
    char *oldmode_letters = (char *)malloc(9);
    char *mode_letters = (char *)malloc(9);

    

    if (getoldmodeletters(argv[1 + no_options], argv[2 + no_options], oldmode_letters))
    {
        return 1;
    }

    //Turn mode (when written in digits) to an octal number in order to call chmod function
    if (isdigit(argv[1 + no_options][0]))
    {
        mode_str[0] = argv[1 + no_options][0];
        mode_str[1] = argv[1 + no_options][1];
        mode_str[2] = argv[1 + no_options][2];
        mode_str[3] = argv[1 + no_options][3];

        getnewmodeletters(mode_str, mode_letters);

        mode = strtol(argv[1 + no_options], 0, 8);
        if (getoldmode(argv[1 + no_options], argv[2 + no_options], oldmode))
        {
            return 1;
        }

        if (oldmode == NULL)
            return 1;
    }
    else
    {
        /*We need to know the current permissions in place because here we 
       are just adding or deleting permissions to certain people, we
       don't change the current permissions for the other people.
       */
        if (parse(argv[1 + no_options], argv[2 + no_options], &mode_u, &mode_g, &mode_o, oldmode))
        {
            return 1;
        }

        if (oldmode == NULL)
            return 1;

        int modeu = 0, modeg = 0, modeo = 0;

        calculate_mode(mode_u, &modeu);
        calculate_mode(mode_g, &modeg);
        calculate_mode(mode_o, &modeo);

        mode_str[0] = '0';
        mode_str[1] = modeu + '0';
        mode_str[2] = modeg + '0';
        mode_str[3] = modeo + '0';

        getnewmodeletters(mode_str, mode_letters);

        mode = strtol(mode_str, 0, 8);
    }

    nftot++;

    if (IMPOSSIBLE)
    {

        fprintf(stderr, "chmod: cannot access '%s': %s\n", path_used_shell, strerror(errno));

        if (opts.v)
        {
            char const *msg = "failed to change mode of '";
            char const *msg2 = "' from ";
            char const *msg3 = " to ";

            size_t nbytes = snprintf(NULL, 0, "%s%s%s%s%s%s%s%s%s%s%s%s\n", msg, path_used_shell, msg2, oldmode, "(", oldmode_letters, ")", msg3, mode_str, "(", mode_letters, ")");
            if (nbytes == -1)
            {
                return 1;
            }
            char *print = (char *)malloc(nbytes);

            if (snprintf(print, nbytes, "%s%s%s%s%s%s%s%s%s%s%s%s\n", msg, path_used_shell, msg2, oldmode, "(", oldmode_letters, ")", msg3, mode_str, "(", mode_letters, ")") == -1)
            {
                return 1;
            }
            printf("%s\n", print);
        }
    }
    else
    {
        if (chmod(canonical_path, mode) == -1)
        {
            fprintf(stderr, "chmod: cannot access '%s': %s\n", path_used_shell, strerror(errno));
        }
        else
        { //FILE_MODF here (reason why went to get oldmode)

            MODE = mode;
            nfmod++;
            if (mode_str[1] != '0')
            {
                if (strcmp(mode_str, oldmode) != 0)
                { //Think we only need to write if they are different

                    if (write_FILE_MODF(oldmode, mode_str, canonical_path))
                    {
                        return 1;
                    }
                    if (opts.c || opts.v)
                    {
                        char const *msg = "mode of '";
                        char const *msg2 = "' changed from ";
                        char const *msg3 = ") to ";

                        size_t nbytes = snprintf(NULL, 0, "%s%s%s%s%s%s%s%s%s%s%s\n", msg, path_used_shell, msg2, oldmode, "(", oldmode_letters, msg3, mode_str, "(", mode_letters, ")");
                        if (nbytes == -1)
                        {
                            return 1;
                        }
                        char *print = (malloc(nbytes));

                        if (snprintf(print, nbytes, "%s%s%s%s%s%s%s%s%s%s%s\n", msg, path_used_shell, msg2, oldmode, "(", oldmode_letters, msg3, mode_str, "(", mode_letters, ")") == -1)
                        {
                            return 1;
                        }

                        printf("%s\n", print);
                    }
                }
                else
                {
                    if (opts.v)
                    {
                        char const *msg = "mode of '";
                        char const *msg2 = "' retained as ";

                        size_t nbytes = snprintf(NULL, 0, "%s%s%s%s%s%s%s\n", msg, path_used_shell, msg2, oldmode, "(", oldmode_letters, ")");
                        if (nbytes == -1)
                        {
                            return 1;
                        }
                        char *print = (char *)malloc(nbytes);

                        if (snprintf(print, nbytes, "%s%s%s%s%s%s%s\n", msg, path_used_shell, msg2, oldmode, "(", oldmode_letters, ")") == -1)
                        {
                            return 1;
                        }
                        printf("%s\n", print);
                    }
                }
            }
        }
    }

    free(canonical_path);
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

    return true;
}

int checkLog(char *envp[], char *reg)
{

    //Check if LOG_FILENAME was defined by user
    for (int j = 0; envp[j] != NULL; j++)
    {

        if (aretheyequal(envp[j], "LOG_FILENAME"))
        {
            if (snprintf(reg, 100, "%s", getenv("LOG_FILENAME")) == -1)
            {
                return 1;
            }

            int fd;

            if (access(reg, F_OK) == 0)
            { //When file exists->Truncate it

                char *s = getenv("XMOD_PARENT_PROCESS");
                if (s == NULL)
                {
                    fd = open(reg, O_CREAT | O_TRUNC | O_WRONLY, 0600);
                    if (fd == -1)
                    {
                        return 1;
                    }
                    if (close(fd) == -1)
                    {
                        return 1;
                    }
                }
            }
            else
            {
                //If file doesn't exist->Create a new one
                fd = open(reg, O_CREAT | O_EXCL, 0644);
                if (fd == -1)
                {
                    return 1;
                }
                if (close(fd) == -1)
                {
                    return 1;
                }
            }
            return 0;
        }
    }
    return -1;
}

int main(int argc, char *argv[], char *envp[])
{
    IMPOSSIBLE=false;
    PID_CURRENT_CHILD = 0;
    nftot = 0;
    nfmod = 0;
    
    global_file_path = realpath(argv[argc - 1], NULL);
    if (global_file_path == NULL) {
        perror("");
        return 1;
    }

    if (subscribe_SIGINT())
    { //Ctrl+C interruption
        perror("Subscribe SIGINT failed\n");
        if(write_PROC_EXIT(1))
        {
           return 1;
        }
        return 1;
    }

    if(subscribe_osignals()){
        perror("Subscribe to Other signals (besides SIGINT) failed\n");
        if(write_PROC_EXIT(1))
        {
           return 1;
        }
        return 1;
    }

    char *reg = (char *)malloc(100); //should be enough right?

    int result=checkLog(envp,reg);
    if(result==1)
    {
        
        if(write_PROC_EXIT(1))
        {
           return 1;
        }
        return 1;
    }

    if(result!=-1)
    {
    if (getfd(reg))
    {
        if (write_PROC_EXIT(1))
        {
            return 1;
        }
        return 1;
    }
    }

    //Environment variable for initial instant
    struct timeval start;
    gettimeofday(&start, NULL);
    char t[64];
    if (snprintf(t, 64, "%lld", 1000000LL * start.tv_sec + start.tv_usec) == -1)
    {
        if (write_PROC_EXIT(1))
        {
            return 1;
        }
        return 1;
    }
    if (setenv("XMOD_PARENT_PROCESS", t, false) == -1)
    {
        if (write_PROC_EXIT(1))
        {
            return 1;
        }
        return 1;
    }
    char *s = getenv("XMOD_PARENT_PROCESS");
    if (s == NULL)
    {
        if (write_PROC_EXIT(1))
        {
            return 1;
        }
        return 1;
    }

    long time = atol(s);
    START_TIME.tv_sec = time / (1000000LL);
    START_TIME.tv_usec = time % (1000000LL);

    //It's gonna have a PROC_CREAT here (only PROC_CREAT right now-->because we only have one process)
    //eventHandler(0, argc, argv, reg,time_taken);
    if (write_PROC_CREATE(argv))
    {
        if (write_PROC_EXIT(1))
        {
            return 1;
        }
        return 1;
    }

    if (argc < 3)
    { //chmod options permissions file_name
        /*If no options are specified, chmod modifies the permissions of the file 
                specified by file name to the permissions specified by permissions.
                So it's possible to have only 3 arguments--->xmod, permissions, file_name*/
        printf("Not enough arguments\n");
        if (write_PROC_EXIT(1))
        {
            return 1;
        }
        return 1;
    }

    options opts;
    opts.c = 0;
    opts.v = 0;
    opts.R = 0;
    int ops = 0;
    if (get_options(argc, argv, &opts, &ops))
    {
        if (write_PROC_EXIT(1))
        {
            return 1;
        }
        return 1;
    }
    //print_options(opts);

     if (xmod(argc, argv,opts,ops))
    {
        if (write_PROC_EXIT(1))
        {
            return 1;
        }
        return 1;
    }

    if (opts.R)
    {
        if (traverse(argc, argv,opts,ops) != 0)
        {
            if (write_PROC_EXIT(1))
            {
                return 1;
            }
            return 1;
        }
    }


    if(IMPOSSIBLE){
        if (chmod(global_file_path, MODE) < 0)
        {
           if (write_PROC_EXIT(1))
           {
            return 1;
           }
        return 1;
        }
    }
    if (write_PROC_EXIT(0))
    {
        return 1;
    }
    free(reg);
    if (close(FD_LOG_FILE) == -1)
    {
        return 1;
    }
    return 0;
}
