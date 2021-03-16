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


/*xmod(file_dir) //assuming -R option
    chmod(file_dir)
    if (file_dir is dir)
        for (fd in file_dir)
            if (fd is dir)
                fork()
                parent:
                    waits for child
                child:
                    exec xmod(fd2)
            else if (fd is file)
                chmod(fd2)
      end cycle
*/

int traverse(int argc, char *argv[]) {
    struct stat st_buf;
    if (stat (argv[argc - 1], &st_buf) != 0) {
        perror("Hoo\n");
        return -1;
    }
    
    if (! S_ISDIR (st_buf.st_mode)) { // if not a directory, traverse is done by default
        return 0;
    }
    
    char path[1000];
    struct dirent *DIRECTORY;
    char *dir_name = argv[argc-1];
    DIR *DP = NULL;

    if ((DP = opendir(dir_name)) == NULL) {
        //Couldn't open directory stream.
        perror("Holo\n");
        return -1;
    }



    //readdir() returns NULL if we've reached the end.
    while ((DIRECTORY = readdir(DP)) != NULL) {

        PID_CURRENT_CHILD = 0;

        //printf("PID: %d new dir name is: %s\n", getpid(), DIRECTORY->d_name);

        if (strcmp(DIRECTORY->d_name, ".") != 0 && strcmp(DIRECTORY->d_name, "..") != 0) {
            //Construct new path, to keep traversal.
            

            
            //printf("Taking a nap zZzZzZZZzZzZzZzZzZz\n");
            //sleep(3);

            strcpy(path, dir_name);
            strcat(path, "/");
            strcat(path, DIRECTORY->d_name);
            argv[argc-1] = path;

            // TODO do we need to check if it's a folder and if not change the mode of the file in the current process?
            // right now it's being done in a child process

            int status;
            int waitpid_value;

            struct stat st_path;
            if (stat (path, &st_path) != 0) {
                IMPOSSIBLE=true;
                chmod(global_file_path,0777);
            }
    
            if (DIRECTORY->d_type == DT_REG) { // if not a directory, traverse is done by default
                //printf("PID: %d found a file in %s\n", getpid(), path);
                xmod(argc, argv);
            }
            else if(DIRECTORY->d_type == DT_LNK){
                printf("neither symbolic link \'%s\' nor referent has been changed\n", path);
            }
            else if(DIRECTORY->d_type == DT_DIR){
                printf("PID: %d found a dir in %s\n", getpid(), path);

                pid_t pid = fork();

                switch (pid) {
                case 0: // child
                    if (execv(argv[0], argv) == -1) {
                        perror("Here\n");
                        closedir(DP);
                        write_PROC_EXIT(1);
                        exit(1); // TODO aqui tem que se ver melhor pois return  não parece fazer sentido
                                // já que seria no processo filho mas sem estar exec'd
                    }
                    break;

                case -1: // error
                    perror("Nope\n");
                    closedir(DP);
                    return -1;

                default: // parent
                    PID_CURRENT_CHILD=pid;

                    do{
                        waitpid_value = waitpid(pid, &status, 0);
                    }
                    while(waitpid_value == -1 && errno == EINTR); //TODO more verifications might be needed

                    if(WIFEXITED(status)){
                        int es=WEXITSTATUS(status);
                        //printf("Exit code is %d\n",es);
                        write_PROC_EXIT(es);
                        if(es != 0){
                            exit(es);
                        }
                    }
                    else{   
                        perror("I wish\n");
                        closedir(DP);
                        return -1;
                    }
                    
                    break;
                }
            }
            
            
        }

    }

    closedir(DP);

    return 0;
}
