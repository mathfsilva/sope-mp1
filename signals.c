//Signals.c
/*Okay so here's the plan (I think):
  1.In main, call function to subscribe SIGINT interruptions->using example ssignal as a
base. struct sigaction new; new.sa_handler=signal_handler; (function that handles signals)
  2.In signal_handler:
    2.1.Program receives SIGINT-->call file function write_SIGNAL_RECV;
    2.2.Writes lines mentioned by teacher in Tratamento de sinais--->Important for recursion?
    2.3.Program sends SIGSTOP to process group (try to send signals to everyone, thinking
with recursion (parent & children) in mind), using kill or killpg.-->call file function
write_SIGNAL_SENT.
    Sidenote: use getpgrp() or getpgid() to get process group id
    2.4.Asks user if they wish to terminate program.
    2.5.If yes:
        2.5.1. Send SIGTERM or SIGQUIT to process group-->call write_SIGNAL_SENT
    2.6.If no:
        2.6.1. Send SIGCONT to process group-->call write_SIGNAL_SENT

  Think this is it...
*/

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "traverse.h"
#include "xmod.h"

void signals_handler(int signo)
{
    //int pg=getpgrp();
    printf("%d\n",getppid());
    //kill(getppid(),SIGSTP); 
    /*write_SIGNAL_RECV("SIGINT");
    write_SIGNAL_SENT("SIGSTOP",getppid());*/
 
    bool valid=false;
    printf("Value\n");
    while(!valid){
    printf("Are you sure you want to terminate this program?\n");
    char answer;
    scanf("%s",&answer);
    if(answer=='N' || answer=='n'){
        valid=true;
        //kill(getppid(),SIGCONT); 
        //write_SIGNAL_SENT("SIGCONT");

    }
    else if(answer=='Y' || answer=='y'){
        valid=true;
        kill(getpid(),SIGTERM);
        //write_SIGNAL_SENT(signal="SIGTERM");
        //write_PROC_EXIT(exit status=1);
        //writes lines here
        exit(1);
    }
  }
}

void sigint_handler(int signo){
  
  write_SIGNAL_RECV("SIGINT");

  int pid_size =20;
  char pid[pid_size];
  snprintf(pid, pid_size, "%d", getpid());

  int nftot_size = 20;
  char nftot_str[nftot_size];
  snprintf(nftot_str, nftot_size, "%d", nftot);

  int nfmod_size = 20;
  char nfmod_str[nfmod_size];
  snprintf(nfmod_str, nfmod_size, "%d", nfmod);

  int file_path_size = strlen(global_file_path);

  size_t final_size = pid_size + nfmod_size + nftot_size + file_path_size + 9; //10 is for 3* " ; " and \0

  char *str_final = malloc(final_size * sizeof(char));

  strcat(str_final, pid);
  strcat(str_final, " ; ");
  strcat(str_final, global_file_path);
  strcat(str_final, " ; ");
  strcat(str_final, nftot_str);
  strcat(str_final, " ; ");
  strcat(str_final, nfmod_str);
  str_final[final_size-1] = '\0';

  printf("%s\n", str_final);
  //printf("My current Childs pid is: %d\n", PID_CURRENT_CHILD);

  if (PID_CURRENT_CHILD == 0){
    sleep(5);
    printf("Do you wish to end the program? (y/n)\n");
    char letter = getchar();
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

    if(letter == 'y'){
      printf("Ending the program\n");
      exit(1);
    }
    else if(letter == 'n'){
      printf("Continuing the program\n");
    }
    else{
      printf("Bad input continuing the program\n");
    }
  }
}

void subscribe_SIGINT(){

  //printf("Subscribed SIGINT signal\n");
  struct sigaction interruption;
  interruption.sa_handler = sigint_handler;
  interruption.sa_flags = 0;
  sigemptyset(&interruption.sa_mask);

  sigaction(SIGINT,&interruption,NULL);

}
