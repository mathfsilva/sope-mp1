#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>

char *getoldmode(char *p, char *f)
{
    char *oldmode_str = (char *)malloc(3);
    struct stat fs;
    stat(f, &fs); //Gets current permission
    int mode_u_r = 0, mode_u_w = 0, mode_u_x = 0, mode_g_r = 0, mode_g_w = 0, mode_g_x = 0, mode_o_r = 0, mode_o_w = 0, mode_o_x = 0;

    //User Permission
    if (fs.st_mode & S_IRUSR)
    {
        mode_u_r += 4;
    }
    if (fs.st_mode & S_IWUSR)
    {
        mode_u_w += 2;
    }
    if (fs.st_mode & S_IXUSR)
    {
        mode_u_x += 1;
    }
    //Group Permission
    if (fs.st_mode & S_IRGRP)
    {
        mode_g_r += 4;
    }
    if (fs.st_mode & S_IWGRP)
    {
        mode_g_w += 2;
    }
    if (fs.st_mode & S_IXGRP)
    {
        mode_g_x += 1;
    }

    //Others Permission
    if (fs.st_mode & S_IROTH)
    {
        mode_o_r += 4;
    }
    if (fs.st_mode & S_IWOTH)
    {
        mode_o_w += 2;
    }
    if (fs.st_mode & S_IXOTH)
    {
        mode_o_x += 1;
    }

    int mode_u = mode_u_r + mode_u_w + mode_u_x;
    int mode_g = mode_g_r + mode_g_w + mode_g_x;
    int mode_o = mode_o_r + mode_o_w + mode_o_x;

    oldmode_str[0] = mode_u + '0';
    oldmode_str[1] = mode_g + '0';
    oldmode_str[2] = mode_o + '0';

    return oldmode_str;
}

char *parse(char *p, char *f, int &mode_u_r, int &mode_u_w, int &mode_u_x, int &mode_g_r, int &mode_g_w, int &mode_g_x, int &mode_o_r, int &mode_o_w, int &mode_o_x)
{
    int size = strlen(p);
    char *oldmode_str = (char *)malloc(3);
    struct stat fs;
    stat(f, &fs); //Gets current permission

    //User Permission
    if (fs.st_mode & S_IRUSR)
    {
        mode_u_r += 4;
    }
    if (fs.st_mode & S_IWUSR)
    {
        mode_u_w += 2;
    }
    if (fs.st_mode & S_IXUSR)
    {
        mode_u_x += 1;
    }
    //Group Permission
    if (fs.st_mode & S_IRGRP)
    {
        mode_g_r += 4;
    }
    if (fs.st_mode & S_IWGRP)
    {
        mode_g_w += 2;
    }
    if (fs.st_mode & S_IXGRP)
    {
        mode_g_x += 1;
    }

    //Others Permission
    if (fs.st_mode & S_IROTH)
    {
        mode_o_r += 4;
    }
    if (fs.st_mode & S_IWOTH)
    {
        mode_o_w += 2;
    }
    if (fs.st_mode & S_IXOTH)
    {
        mode_o_x += 1;
    }

    int mode_u = mode_u_r + mode_u_w + mode_u_x;
    int mode_g = mode_g_r + mode_g_w + mode_g_x;
    int mode_o = mode_o_r + mode_o_w + mode_o_x;

    oldmode_str[0] = mode_u + '0';
    oldmode_str[1] = mode_g + '0';
    oldmode_str[2] = mode_o + '0';

    //Let's consider letters option first (u+w)
    //User
    if (p[0] == 'u')
    {
        int i = 2;
        if (p[1] == '+' || p[1] == '=')
        {
            while (i != size)
            {
                if (p[i] == 'r')
                {
                    mode_u_r = 4;
                    i++;
                }
                if (p[i] == 'w')
                {
                    mode_u_w = 2;
                    i++;
                }
                if (p[i] == 'x')
                {
                    mode_u_x = 1;
                    i++;
                }
            }
        }
        if (p[1] == '-')
        {
            while (i != size)
            {
                if (p[i] == 'r')
                {
                    mode_u_r = 0;
                    i++;
                }
                if (p[i] == 'w')
                {
                    mode_u_w = 2;
                    i++;
                }
                if (p[i] == 'x')
                {
                    mode_u_x = 1;
                    i++;
                }
            }
        }
    }

    //Group
    else if (p[0] == 'g')
    {
        int i = 2;
        if (p[1] == '+' || p[1] == '=')
        {
            while (i != size)
            {
                if (p[i] == 'r')
                {
                    mode_g_r = 4;
                    i++;
                }
                if (p[i] == 'w')
                {
                    mode_g_w = 2;
                    i++;
                }
                if (p[i] == 'x')
                {
                    mode_g_x = 1;
                    i++;
                }
            }
        }
        if (p[1] == '-')
        {
            while (i != size)
            {
                if (p[i] == 'r')
                {
                    mode_g_r = 4;
                    i++;
                }
                if (p[i] == 'w')
                {
                    mode_g_w = 2;
                    i++;
                }
                if (p[i] == 'x')
                {
                    mode_g_x = 1;
                    i++;
                }
            }
        }
    }

    //Others
    else if (p[0] == 'o')
    {
        int i = 2;
        if (p[1] == '+' || p[1] == '=')
        {
            while (i != size)
            {
                if (p[i] == 'r')
                {
                    mode_o_r = 4;
                    i++;
                }
                if (p[i] == 'w')
                {
                    mode_o_w = 2;
                    i++;
                }
                if (p[i] == 'x')
                {
                    mode_o_x = 1;
                    i++;
                }
            }
        }
        if (p[1] == '-')
        {
            while (i != size)
            {
                if (p[i] == 'r')
                {
                    mode_o_r = 4;
                    i++;
                }
                if (p[i] == 'w')
                {
                    mode_o_w = 2;
                    i++;
                }
                if (p[i] == 'x')
                {
                    mode_o_x = 1;
                    i++;
                }
            }
        }
    }

    //All
    else if (p[0] == 'a')
    {
        int i = 2;
        if (p[1] == '+' || p[1] == '=')
        {
            while (i != size)
            {
                if (p[i] == 'r')
                {
                    mode_u_r = 4;
                    mode_g_r = 4;
                    mode_o_r = 4;
                    i++;
                }
                if (p[i] == 'w')
                {
                    mode_u_w = 2;
                    mode_g_w = 2;
                    mode_o_w = 2;
                    i++;
                }
                if (p[i] == 'x')
                {
                    mode_u_x = 1;
                    mode_g_x = 1;
                    mode_o_x = 1;
                    i++;
                }
            }
        }
        if (p[1] == '-')
        {
            while (i != size)
            {
                if (p[i] == 'r')
                {
                    mode_u_r = 0;
                    mode_g_r = 0;
                    mode_o_r = 0;
                    i++;
                }
                if (p[i] == 'w')
                {
                    mode_u_w = 0;
                    mode_g_w = 0;
                    mode_o_w = 0;
                    i++;
                }
                if (p[i] == 'x')
                {
                    mode_u_x = 0;
                    mode_g_x = 0;
                    mode_o_x = 0;
                    i++;
                }
            }
        }
    }
    return oldmode_str;
}

int xmod(int argc, char *argv[])
{

    char *options;
    int mode;
    char mode_str[3] = {'0', '0', '0'};
    int mode_u = 0, mode_g = 0, mode_o = 0;
    int mode_u_r = 0, mode_u_w = 0, mode_u_x = 0;
    int mode_g_r = 0, mode_g_w = 0, mode_g_x = 0;
    int mode_o_r = 0, mode_o_w = 0, mode_o_x = 0;
    char *oldmode;
    if (argv[1][0] == '-' && (argv[1][1] == 'c' | argv[1][1] == 'R' | argv[1][1] == 'v'))
    { //Or argc==4
        options = argv[1];
        printf("%s\n", options);
    }

    //Turn mode (when written in digits) to an octal number in order to call chmod function
    else if (isdigit(argv[1][0]))
    {
        mode = strtol(argv[1], 0, 8);
        oldmode = getoldmode(argv[1], argv[2]);

        //FILE_MODF here (reason why went to get oldmode)
        if (chmod(argv[argc - 1], mode) < 0)
        {
            printf("ERROR");
        }
    }
    else
    {
        /*We need to know the current permissions in place because here we 
       are just adding or deleting permissions to certain people, we
       don't change the current permissions for the other people.
       */
        oldmode = parse(argv[1], argv[2], mode_u_r, mode_u_w, mode_u_x, mode_g_r, mode_g_w, mode_g_x, mode_o_r, mode_o_w, mode_o_x);

        mode_u = mode_u_r + mode_u_w + mode_u_x;
        mode_g = mode_g_r + mode_g_w + mode_g_x;
        mode_o = mode_o_r + mode_o_w + mode_o_x;

        mode_str[0] = mode_u + '0';
        mode_str[1] = mode_g + '0';
        mode_str[2] = mode_o + '0';

        mode = strtol(mode_str, 0, 8);
        printf("%s\n", mode_str);

        //FILE_MODF here (reason why went to get oldmode)
        if (chmod(argv[argc - 1], mode) < 0)
        {
            printf("ERROR");
        }
    }

    if (argc == 4)
    { //In case we have options
        if (isdigit(argv[2][0]))
        {
            mode = strtol(argv[1], 0, 8);
            oldmode = getoldmode(argv[1], argv[2]);

            //FILE_MODF here (reason why went to get oldmode)
            if (chmod(argv[argc - 1], mode) < 0)
            {
                printf("ERROR");
            }
        }
        else
        {
            oldmode = parse(argv[2], argv[3], mode_u_r, mode_u_w, mode_u_x, mode_g_r, mode_g_w, mode_g_x, mode_o_r, mode_o_w, mode_o_x);

            mode_u = mode_u_r + mode_u_w + mode_u_x;
            mode_g = mode_g_r + mode_g_w + mode_g_x;
            mode_o = mode_o_r + mode_o_w + mode_o_x;

            mode_str[0] = mode_u + '0';
            mode_str[1] = mode_g + '0';
            mode_str[2] = mode_o + '0';

            mode = strtol(mode_str, 0, 8);
            printf("%s\n", mode_str);

            //FILE_MODF here (reason why went to get oldmode)
            if (chmod(argv[argc - 1], mode) < 0)
            {
                printf("ERROR");
            }
        }
    }
    //Symbolic Link check
    /*
while ((ent = readdir(dir)) != NULL) {
  if (ent->d_type == DT_LNK) { 
            
  }
}   */

    return 0;
}

int checkLog(char *envp[])
{

    //Check if LOG_FILENAME was defined by user
    char *reg = secure_getenv("LOG_FILENAME");
    //printf("%s\n",reg);

    int fd;
    char const *text1 = "Holo Pat"; //Experiment

    if (access(reg, F_OK) == 0)
    { //When file exists->Truncate it
        //printf("File exists\n");
        fd = open(reg, O_CREAT | O_TRUNC | O_WRONLY, 0600);
        write(fd, text1, 8);
    }
    else
    {
        //If file doesn't exist->Create a new one
        //printf("File doesn't exist\n");
        fd = open(reg, O_CREAT | O_EXCL, 0644);
        write(fd, text1, 8);
    }
    return fd;
}

void checkSymlink(int argc, char *argv[])
{
    //Symlink check.
    struct stat buffer;
    int t = lstat(argv[argc - 1], &buffer);
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

void eventHandler(int code, char *argv[], int fd){
    switch (code){
        case 0:
            char msg[] = "PROCESS CREATED; PID: ";
            write(fd, msg, sizeof(msg)/sizeof(char));
            char pid[5];
            sprintf(pid, "%d", getpid());
            write(fd, pid, sizeof(pid)/sizeof(int));
            break;
    }
}

int main(int argc, char *argv[], char *envp[])
{
    clock_t start, end;

    start = clock();
    int fd = checkLog(envp);

    //It's gonna have a PROC_CREAT here (only PROC_CREAT right now-->because we only have one process)
    eventHandler(0, argv, fd);


    if (argc < 3)
    {   //chmod options permissions file_name
        /*If no options are specified, chmod modifies the permissions of the file 
                specified by file name to the permissions specified by permissions.
                So it's possible to have only 3 arguments--->xmod, permissions, file_name*/
        printf("Not enough arguments\n");
        //PROC_EXIT here
        return 1;
    }

    
    checkSymlink(argc, argv);

    if (xmod(argc, argv))
    {
        return 1;
    }

    close(fd);
    end = clock() - start;

    double time_taken = ((double)end) / (CLOCKS_PER_SEC / 1000); // in miliseconds

    printf("Process took %f miliseconds to execute \n", time_taken);

    //Should have a PROC_EXIT here
    return 0;
}