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
#include "file.h"

void signals_handler(int signo)
{
    printf("Sup\n");
    //int pg=getpgrp();
    printf("Still\n");
    kill(getppid(),SIGSTOP); //This is working
    printf("Gone\n");
    //write_SIGNALRECV(signal="SIGINT");--->figure out
    //write_SIGNALSEND(signal="SIGSTOP");
 
    bool valid=false;
    printf("Value\n");
    while(!valid){
    printf("Are you sure you want to terminate this program?\n");
    char answer;
    scanf("%s",&answer);
    if(answer=='N' || answer=='n'){
        valid=true;
        kill(getppid(),SIGCONT); //Working 
        //write_SIGNALSEND("signal=SIGCONT");
        printf("GO\n");

    }
    else if(answer=='Y' || answer=='y'){
        printf("hi\n");
        valid=true;
        kill(getppid(),SIGTERM);
        //write_SIGNALSEND(signal="SIGTERM");
        //write_PROC_EXIT(exit status=1);
        //writes lines here
        printf("EXIT\n");
        exit(1);
        printf("Gone\n");
    }
    }
}

void subscribe_SIGINT(){

  printf("Holo\n");
  struct sigaction interruption;
  interruption.sa_handler = signals_handler;
  interruption.sa_flags = 0;
  sigemptyset(&interruption.sa_mask);

  sigaction(SIGINT,&interruption,NULL);

}
