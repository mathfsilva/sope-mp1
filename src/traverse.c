#include "./traverse.h"
#include "./file.h"
#include "./xmod.h"


int traverse(int argc, char *argv[], options ops, int no_options) {
    struct stat st_buf;
    if (stat(argv[argc - 1], &st_buf) != 0) {
        perror("Stat from path returned != 0\n");
        return 1;
    }

    if (!S_ISDIR(st_buf.st_mode)) {
        // if not a directory, traverse is done by default
        return 0;
    }

    struct dirent *DIRECTORY;
    char *dir_name = argv[argc - 1];
    DIR *DP = NULL;

    if ((DP = opendir(dir_name)) == NULL) {
        // Couldn't open directory stream.
        fprintf(stderr, "chmod: cannot read directory '%s': %s\n",
                        dir_name, strerror(errno));
        return 1;
    }

    // readdir() returns NULL if we've reached the end.
    while ((DIRECTORY = readdir(DP)) != NULL) {
        PID_CURRENT_CHILD = 0;      

        if (strcmp(DIRECTORY->d_name, ".") != 0
            && strcmp(DIRECTORY->d_name, "..") != 0) {
            // Construct new path, to keep traversal.

            size_t nbytes = snprintf(NULL, 0, "%s%s%s",
            dir_name, "/", DIRECTORY->d_name) + 1;

            if (nbytes == -1) {
                if (closedir(DP) == -1) {
                    return 1;
                }
                return 1;
            }
            char *path = malloc(nbytes);

            if (snprintf(path, nbytes, "%s%s%s",
             dir_name, "/", DIRECTORY->d_name) == -1) {
                free(path);
                if (closedir(DP) == -1) {
                    return 1;
                }
                return 1;
            }
            argv[argc - 1] = path;

          
            int status;
            int waitpid_value;
            struct stat st_path;
            if (stat(path, &st_path) != 0) {
                IMPOSSIBLE = true;
                if (chmod(global_file_path, 0777) < 0) {
                    free(path);
                    if (closedir(DP) == -1) {
                        return 1;
                    }
                    return 1;
                }
            }

            if (IMPOSSIBLE) {
                if (xmod(argc, argv, ops, no_options)) {
                    perror("Failed xmod in traverse\n");
                    free(path);
                    if (closedir(DP) == -1) {
                        return 1;
                    }
                    return 1;
                }
                free(path);
                continue;
            }

            if (DIRECTORY->d_type == DT_REG) {
                // if not a directory, traverse is done by default
                if (xmod(argc, argv, ops, no_options)) {
                    perror("Failed xmod in traverse\n");
                    free(path);
                    if (closedir(DP) == -1) {
                        return 1;
                    }
                    return 1;
                }
            } else if (DIRECTORY->d_type == DT_LNK) {
                if (!IMPOSSIBLE) {
                    if (ops.v) {
                       printf("neither symbolic link \'%s\' nor referent has been changed\n", path);
                    }
                }
            } else if (DIRECTORY->d_type == DT_DIR) {

                pid_t pid = fork();

                switch (pid) {
                    case 0:
                     // child
                        if (execv(argv[0], argv) == -1) {
                            perror("Execv failed\n");
                            if (closedir(DP) == -1) {
                            }
                            if (write_PROC_EXIT(1)) {
                            }
                            free(path);
                            exit(1);
                            
                        }
                        free(path);
                        break;
                    case -1:
                    // error
                        free(path);
                        perror("Process failed on creating child\n");
                        if (closedir(DP) == -1) {
                            return 1;
                        }
                        return 1;
                    default:
                    // parent
                        PID_CURRENT_CHILD = pid;

                        do {
                            waitpid_value = waitpid(pid, &status, 0);
                        } while (waitpid_value == -1 && errno == EINTR);
                        // TO SEE more verifications might be needed

                        if (WIFEXITED(status)) {
                            int es = WEXITSTATUS(status);
                            
                            if (es != 0) {
                                if (write_PROC_EXIT(es)) {
                                }
                            free(path);
                            if (closedir(DP) == -1) {
                                return 1;
                            }
                            exit(es);
                            }
                        } else {
                            perror("Bad status uppon waiting on child\n");
                            free(path);
                            if (closedir(DP) == -1) {
                                return 1;
                            }
                        }
                        break;
                }
            } 
            free(path);
        }
    }


    if (closedir(DP) == -1) {
        return 1;
    }

    return 0;
}