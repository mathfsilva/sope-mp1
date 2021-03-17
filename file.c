//File.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include<sys/time.h>
#include "file.h"
extern struct timeval START_TIME;


int getfd(char*reg){
    printf("Called\n");
    FD_LOG_FILE= open(reg, O_WRONLY|O_SYNC|O_APPEND,0600);
    //printf("%d\n",FD_LOG_FILE);
    if(FD_LOG_FILE==-1){
        return 1;
    }
    else{
        return 0;
    }
}

void calculate_time(long double *time_taken){
    struct timeval end;
    gettimeofday(&end, NULL);
    struct timeval temp_diff;

  

  temp_diff.tv_sec =end.tv_sec -START_TIME.tv_sec ;
  temp_diff.tv_usec=end.tv_usec-START_TIME.tv_usec;

  /* Using while instead of if below makes the code slightly more robust. */

  while(temp_diff.tv_usec<0)
  {
    temp_diff.tv_usec+=1000000;
    temp_diff.tv_sec -=1;
  }

 *time_taken=(1000000LL*temp_diff.tv_sec+temp_diff.tv_usec);

} 

int write_PROC_CREATE(char *argv[]){
    long double time_taken=0;
    calculate_time(&time_taken);
    time_taken/=1000;
    static int sizet=32;
    char t[sizet]; 
    char const *msg2 = "PROC_CREAT";
    static int size =20;
    char pid[size];
    if(snprintf(pid, size, "%d", getpid())==-1){
        return 1;
    }
    if(snprintf(t,sizet,"%Lf",time_taken)==-1){
        return 1;
    }

    
    size_t size2=sizeof(char*)+sizet+size+strlen(msg2);
    char *str_final=(char*)malloc(size2*2);
    strcat(str_final,t);
    strcat(str_final," ; ");
    strcat(str_final,pid);
    strcat(str_final," ; ");
    strcat(str_final,msg2);
    strcat(str_final," ; ");
    for(int j=0;argv[j]!=NULL;j++){
        strcat(str_final,argv[j]);
        strcat(str_final," ");
    }
    strcat(str_final,"\n");
    if(write(FD_LOG_FILE,str_final,strlen(str_final))==-1){
        return 1;
    }
    
    free(str_final);
    return 0;
}

int write_PROC_EXIT(int exit_code){
    long double time_taken=0;
    calculate_time(&time_taken);
    time_taken/=1000;
    char const *final= "PROC_EXIT";

    size_t nbytes = snprintf(NULL, 0, "%Lf ; %d ; %s ; %d\n", time_taken, getpid(), final, exit_code) + 1;
    if(nbytes==-1){
        return 1;
    }
    char *str_final = malloc(nbytes);

    if(snprintf(str_final, nbytes, "%Lf ; %d ; %s ; %d\n", time_taken, getpid(), final, exit_code)==-1){
        return 1;
    }

    if(write(FD_LOG_FILE,str_final,strlen(str_final))==-1){
        return 1;
    }
    if(close(FD_LOG_FILE)==-1){
        return 1;
    }
    free(str_final);
    return 0;
}


int write_FILE_MODF(char*old_mode,char* new_mode,char*file_name){ 
    long double time_taken=0;
    calculate_time(&time_taken);
    time_taken/=1000;
    char const *msg="FILE_MODF";

    size_t nbytes = snprintf(NULL, 0, "%Lf ; %d ; %s ; %s : %s : %s\n", time_taken, getpid(), msg, file_name, old_mode, new_mode) + 1;

    if(nbytes==-1){
        printf("STUPID\n");
        return 1;
    }
    char *str_final = malloc(nbytes);

    if(snprintf(str_final, nbytes, "%Lf ; %d ; %s ; %s : %s : %s\n", time_taken, getpid(), msg, file_name, old_mode, new_mode)==-1){
        printf("ERROQ\n");
        return 1;
    }

    printf("%d\n",FD_LOG_FILE);
    if(write(FD_LOG_FILE,str_final,strlen(str_final))==-1){
        printf("OPSSIE\n");
        return 1;
    }
    free(str_final);
    return 0;
}

int write_SIGNAL_RECV(char *signal){
    long double time_taken=0;
    calculate_time(&time_taken);
    time_taken/=1000;

    char const *msg="SIGNAL_RECV";

    size_t nbytes = snprintf(NULL, 0, "%Lf ; %d ; %s ; %s\n", time_taken, getpid(), msg, signal) + 1;
 
    if(nbytes==-1){
        return 1;
    }
    char *str_final = malloc(nbytes);

    if(snprintf(str_final, nbytes, "%Lf ; %d ; %s ; %s\n", time_taken, getpid(), msg, signal)==-1){
        return 1;
    }

    if(write(FD_LOG_FILE,str_final,strlen(str_final))==-1){
        return 1;
    }
    free(str_final);
    return 0;
}

int write_SIGNAL_SENT(char *signal,pid_t target_pid){
    long double time_taken=0;
    calculate_time(&time_taken);
    time_taken/=1000;
    char const *msg="SIGNAL_SENT";
    

    size_t nbytes = snprintf(NULL, 0, "%Lf ; %d ; %s ; %s : %d\n", time_taken, getpid(), msg, signal, target_pid) + 1;

    if(nbytes==-1){
        return 1;
    }
    char *str_final = malloc(nbytes);

    if(snprintf(str_final, nbytes, "%Lf ; %d ; %s ; %s : %d\n", time_taken, getpid(), msg, signal, target_pid)==-1){
        return 1;
    }
    
    if(write(FD_LOG_FILE,str_final,strlen(str_final))==-1){
        return 1;
    }
    free(str_final);
    return 0;

}