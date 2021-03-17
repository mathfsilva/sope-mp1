#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ftw.h>
#include <stdbool.h>

#include "traverse.h"
#include "file.h"
#include "xmod.h"

int traverse(int argc, char *argv[],options ops,int no_options)
{
    struct stat st_buf;
    if (stat(argv[argc - 1], &st_buf) != 0)
    {
        perror("Stat from path returned != 0\n");
        return 1;
    }

    if (!S_ISDIR(st_buf.st_mode))
    { // if not a directory, traverse is done by default
        return 0;
    }

    //char *path = malloc(1000*sizeof(char));
    struct dirent *DIRECTORY;
    char *dir_name = argv[argc - 1];
    DIR *DP = NULL;
    

    if ((DP = opendir(dir_name)) == NULL)
    {
        //Couldn't open directory stream.
        perror("Could not open dir\n");
        return 1;
    }

    //readdir() returns NULL if we've reached the end.
    while ((DIRECTORY = readdir(DP)) != NULL)
    {

        PID_CURRENT_CHILD = 0;

        //printf("PID: %d new dir name is: %s\n", getpid(), DIRECTORY->d_name);

        if (strcmp(DIRECTORY->d_name, ".") != 0 && strcmp(DIRECTORY->d_name, "..") != 0)
        {
            //Construct new path, to keep traversal.

            //printf("Taking a nap zZzZzZZZzZzZzZzZzZz\n");
            //sleep(3);

            /*strcpy(path, dir_name);
            strcat(path, "/");
            strcat(path, DIRECTORY->d_name);*/

            size_t nbytes = snprintf(NULL, 0, "%s%s%s", dir_name,"/", DIRECTORY->d_name) + 1;

    if (nbytes == -1)
    {
        return 1;
    }
    char *path = malloc(nbytes);

    if (snprintf(path, nbytes, "%s%s%s", dir_name,"/", DIRECTORY->d_name) == -1)
    {
        return 1;
    }
            argv[argc - 1] = path;
            printf("%s\n",path);

            // TODO do we need to check if it's a folder and if not change the mode of the file in the current process?
            // right now it's being done in a child process

            int status;
            int waitpid_value;
            struct stat st_path;
            if (stat(path, &st_path) != 0)
            {
                IMPOSSIBLE = true;
                if (chmod(global_file_path, 0777) < 0)
                {
                    return 1;
                }
            }

            if (IMPOSSIBLE) 
            {
                if (xmod(argc, argv,ops,no_options))
               {
                    //perror("Failed xmod in traverse\n");
                    //return 1;
                }
                continue;
            }

            if (DIRECTORY->d_type == DT_REG)
            { // if not a directory, traverse is done by default
                //printf("PID: %d found a file in %s\n", getpid(), path);
                if (xmod(argc, argv,ops,no_options))
                {
                    perror("Failed xmod in traverse\n");
                    return 1;
                }
            }
            else if (DIRECTORY->d_type == DT_LNK)
            {
                if(!IMPOSSIBLE)
                {
                    if(ops.v){
                       printf("neither symbolic link \'%s\' nor referent has been changed\n", path);
                    }
                }
            }

            if (DIRECTORY->d_type == DT_DIR)
            {
                //printf("PID: %d found a dir in %s\n", getpid(), path);

                pid_t pid = fork();

                switch (pid)
                {
                case 0: // child
                    if (execv(argv[0], argv) == -1)
                    {
                        perror("Execv failed\n");
                        if (closedir(DP) == -1)
                        {
                            //return 1;
                        }
                       if (write_PROC_EXIT(1))
                        {
                            //return 1;
                        }
                        exit(1); // TODO aqui tem que se ver melhor pois return  não parece fazer sentido
                                 // já que seria no processo filho mas sem estar exec'd
                    }
                    break;

                case -1: // error
                    perror("Process failed on creating child\n");
                    if (closedir(DP) == -1)
                    {
                        return 1;
                    }
                    return 1;

                default: // parent
                    PID_CURRENT_CHILD = pid;

                    do
                    {
                        waitpid_value = waitpid(pid, &status, 0);
                    } while (waitpid_value == -1 && errno == EINTR); //TODO more verifications might be needed

                    if (WIFEXITED(status))
                    {
                        int es = WEXITSTATUS(status);
                        //printf("Exit code is %d\n",es);
                        /*if (write_PROC_EXIT(es))
                        {
                            return 1;
                        }*/
                        if (es != 0)
                        {
                            if (write_PROC_EXIT(es))
                            {
                            //return 1;
                            }
                            exit(es);
                        }
                    }
                    else
                    {
                        perror("Bad status uppon waiting on child\n");
                        if (closedir(DP) == -1)
                        {
                            return 1;
                        }
                    }

                    break;
                }
            }
            else
            {
                //printf("PID: %d found something %s\n", getpid(), path);
            }
        }
    }

   // free(path);

    if (closedir(DP) == -1)
    {
        return 1;
    }

    return 0;
}
