#include "transverse.h"

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
#include <stdio.h>


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

int transverse(int argc, char *argv[]) {
    struct stat st_buf;
    stat (argv[argc - 1], &st_buf);
    printf("%s\n", argv[argc-1]); // just to debug
    if (! S_ISDIR (st_buf.st_mode)) { // if not a directory, transverse is done by default
        return 0;
    }
    
    char path[1000];
    struct dirent *DIRECTORY;
    char *dir_name = argv[argc-1];
    DIR *DP = NULL;

    if ((DP = opendir(dir_name)) == NULL) {
        //Couldn't open directory stream.
        return -1;
    }

    //readdir() returns NULL if we've reached the end.
    while ((DIRECTORY = readdir(DP)) != NULL) {
        if (strcmp(DIRECTORY->d_name, ".") != 0 && strcmp(DIRECTORY->d_name, "..") != 0) {
            //Construct new path, to keep traversal.
            
            strcpy(path, dir_name);
            strcat(path, "/");
            strcat(path, DIRECTORY->d_name);
            argv[argc-1] = path;
            
            pid_t pid = fork();
            switch (pid) {
            case 0: // child
                if (execv(argv[0], argv) != 0)
                    exit(1);
                else
                    exit(0);
                break;

            case -1: // error
                perror("");
                closedir(DP);
                return -1;

            default: // parent
                      
                if (waitid(P_PID, pid, NULL, WEXITED) == -1) { // TODO confirmar que opções devem ser usadas
                                                                //      e se não será melhor não passar NULL
                                                                //      para ter acesso ao exit status do processo filho
                    perror("parent");
                    closedir(DP);
                    return -1;
                }
                break;
            }
            
        }
    }

    closedir(DP);

    return 0;
}
