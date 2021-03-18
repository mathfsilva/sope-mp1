#include "./file.h"

int getfd(char *reg) {
    FD_LOG_FILE = open(reg, O_WRONLY | O_SYNC | O_APPEND, 0600);
    if (FD_LOG_FILE == -1) {
        return 1;
    } else {
        return 0;
    }
}

void calculate_time(long double *time_taken) {
    struct timeval end;
    gettimeofday(&end, NULL);
    struct timeval temp_diff;

    temp_diff.tv_sec = end.tv_sec - START_TIME.tv_sec;
    temp_diff.tv_usec = end.tv_usec - START_TIME.tv_usec;

    /* Using while instead of if below makes the code slightly more robust. */

    while (temp_diff.tv_usec < 0) {
        temp_diff.tv_usec += 1000000;
        temp_diff.tv_sec -= 1;
    }

    *time_taken = (1000000LL * temp_diff.tv_sec + temp_diff.tv_usec);
}

int write_PROC_CREATE(char *argv[]) {
    if (FD_LOG_FILE == 0) {
        return 0;
    }
    long double time_taken = 0;
    calculate_time(&time_taken);
    time_taken /= 1000;
    const int kSizet = 32;
    char t[kSizet];
    char const *msg2 = "PROC_CREAT";
    const int kSize = 20;
    char pid[kSize];
    if (snprintf(pid, kSize, "%d", getpid()) == -1) {
        return 1;
    }
    if (snprintf(t, kSizet, "%Lf", time_taken) == -1) {
        return 1;
    }

    size_t size2 = sizeof(char *) + kSizet + kSize + strlen(msg2);
    char *str_final = (char *)malloc(size2 * 2);

    if(snprintf(str_final,size2,"%s%s%s%s%s%s",t," ; ",pid," ; ",msg2," ; ")==-1)
    {
        free(str_final);
        return 1;
    }

    size_t n_total_bytes = snprintf(NULL, 0, "%s", argv[0]) + 1;
    if(n_total_bytes==-1)
    {
        free(str_final);
        return 1;
    }
    char *path_final = malloc(n_total_bytes);
    if(snprintf(path_final, n_total_bytes, "%s", argv[0])==-1)
    {
        free(str_final);
        free(path_final);
        return 1;
    }

    for (int j = 1; argv[j] != NULL; j++) {

        char *path1 = malloc(n_total_bytes);
        if(snprintf(path1, n_total_bytes, "%s", path_final)==-1)
        {
            free(str_final);
            free(path_final);
            free(path1);
            return 1;
        }

        size_t nbytes = snprintf(NULL, 0, "%s", argv[j]) + 1;
        if(nbytes==-1)
        {
            free(str_final);
            free(path_final);
            free(path1);
            return 1;
        }
        char *path2 = malloc(nbytes);
        if(snprintf(path2, nbytes, "%s", argv[j])==-1)
        {
            free(str_final);
            free(path_final);
            free(path1);
            free(path2);
            return 1;
        }

        n_total_bytes = snprintf(NULL, 0, "%s %s", path1, path2) + 1;
        if(n_total_bytes==-1)
        {
            free(str_final);
            free(path_final);
            free(path1);
            free(path2);
            return 1;
        }
        char* path_final = malloc(n_total_bytes + 1);
        if(snprintf(path_final, n_total_bytes, "%s %s", path1, path2)==-1)
        {
            free(str_final);
            free(path_final);
            free(path1);
            free(path2);
            return 1;
        }
        free(path1);
        free(path2);
        free(path_final);
    }

    

    size_t nbytes_final = snprintf(NULL, 0, "%s%s\n", str_final, path_final) + 1;
    if(nbytes_final==-1)
    {
        free(path_final);
        free(str_final);
        return 1;
    }
    char *str_final_w_path = malloc(nbytes_final);
    if(snprintf(str_final_w_path, nbytes_final, "%s%s\n", str_final, path_final)==-1)
    {
        free(path_final);
        free(str_final);
        free(str_final_w_path);
        return 1;
    }

    free(str_final);
    free(path_final);

    if (write(FD_LOG_FILE, str_final_w_path, strlen(str_final_w_path)) == -1) {
        free(str_final_w_path);
        return 1;
    }

    free(str_final_w_path);
    return 0;
}

int write_PROC_EXIT(int exit_code) {
    if (FD_LOG_FILE == 0) {
        return 0;
    }
    long double time_taken = 0;
    calculate_time(&time_taken);
    time_taken /= 1000;
    char const *final = "PROC_EXIT";

    size_t nbytes = snprintf(NULL, 0, "%Lf ; %d ; %s ; %d\n",
                            time_taken, getpid(), final, exit_code) + 1;
    if (nbytes == -1) {
        return 1;
    }
    char *str_final = malloc(nbytes);

    if (snprintf(str_final, nbytes, "%Lf ; %d ; %s ; %d\n",
                time_taken, getpid(), final, exit_code) == -1) {
        free(str_final);
        return 1;
    }

    if (write(FD_LOG_FILE, str_final, strlen(str_final)) == -1) {
        free(str_final);
        return 1;
    }

    free(str_final);
    return 0;
}

int write_FILE_MODF(char *old_mode, char *new_mode, char *file_name) {
    if (FD_LOG_FILE == 0) {
        return 0;
    }
    long double time_taken = 0;
    calculate_time(&time_taken);
    time_taken /= 1000;
    char const *msg = "FILE_MODF";

    size_t nbytes = snprintf(NULL, 0, "%Lf ; %d ; %s ; %s : %s : %s\n",
                            time_taken, getpid(), msg, file_name,
                            old_mode, new_mode) + 1;

    if (nbytes == -1) {
        return 1;
    }
    char *str_final = malloc(nbytes);

    if (snprintf(str_final, nbytes, "%Lf ; %d ; %s ; %s : %s : %s\n",
        time_taken, getpid(), msg, file_name, old_mode, new_mode) == -1) {
        free(str_final);
        return 1;
    }

    if (write(FD_LOG_FILE, str_final, strlen(str_final)) == -1) {
        free(str_final);
        return 1;
    }
    free(str_final);
    return 0;
}

int write_SIGNAL_RECV(char *signal) {
    if (FD_LOG_FILE == 0) {
        return 0;
    }
    long double time_taken = 0;
    calculate_time(&time_taken);
    time_taken /= 1000;

    char const *msg = "SIGNAL_RECV";

    size_t nbytes = snprintf(NULL, 0, "%Lf ; %d ; %s ; %s\n",
                            time_taken, getpid(), msg, signal) + 1;

    if (nbytes == -1) {
        return 1;
    }
    char *str_final = malloc(nbytes);

    if (snprintf(str_final, nbytes, "%Lf ; %d ; %s ; %s\n",
                time_taken, getpid(), msg, signal) == -1) {
        free(str_final);
        return 1;
    }

    if (write(FD_LOG_FILE, str_final, strlen(str_final)) == -1) {
        free(str_final);
        return 1;
    }
    free(str_final);
    return 0;
}

int write_SIGNAL_SENT(char *signal, pid_t target_pid) {
    if (FD_LOG_FILE == 0) {
        return 0;
    }
    long double time_taken = 0;
    calculate_time(&time_taken);
    time_taken /= 1000;
    char const *msg = "SIGNAL_SENT";

    size_t nbytes = snprintf(NULL, 0, "%Lf ; %d ; %s ; %s : %d\n",
                            time_taken, getpid(), msg, signal, target_pid) + 1;

    if (nbytes == -1) {
        return 1;
    }
    char *str_final = malloc(nbytes);

    if (snprintf(str_final, nbytes, "%Lf ; %d ; %s ; %s : %d\n",
                time_taken, getpid(), msg, signal, target_pid) == -1) {
        free(str_final);
        return 1;
    }

    if (write(FD_LOG_FILE, str_final, strlen(str_final)) == -1) {
        free(str_final);
        return 1;
    }
    free(str_final);
    return 0;
}
