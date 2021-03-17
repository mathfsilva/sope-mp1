// Signals.c
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
#include "signals.h"
#include "xmod.h"

void sigint_handler(int signo) {
  if (write_SIGNAL_RECV("SIGINT")) {
    perror("Failed to write to log file\n");
    write_PROC_EXIT(1);
    exit(1);
  }

  size_t nbytes = snprintf(NULL, 0, "%d ; %s ; %d ; %d\n",
                            getpid(), global_file_path, nftot, nfmod) + 1;

  if (nbytes < 0) {
    perror("Failed snprintf\n");
    write_PROC_EXIT(1);
    exit(1);
  }

  char *str_final = malloc(nbytes);

  snprintf(str_final, nbytes, "%d ; %s ; %d ; %d\n",
            getpid(), global_file_path, nftot, nfmod);

  printf("%s", str_final);
  // printf("My current Childs pid is: %d\n", PID_CURRENT_CHILD);

  if (PID_CURRENT_CHILD == 0) {
    // sleep(5);
    printf("Do you wish to end the program? (y/n)\n");
    char letter = getchar();
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    if (letter == 'y') {
      printf("Ending the program\n");
      write_PROC_EXIT(1);
      exit(1);
    } else if (letter == 'n') {
      printf("Continuing the program\n");
    } else {
      printf("Bad input continuing the program\n");
    }
  }
  free(str_final);
}

int subscribe_SIGINT() {
  // printf("Subscribed SIGINT signal\n");
  struct sigaction interruption;
  interruption.sa_handler = sigint_handler;
  interruption.sa_flags = 0;

  if (sigemptyset(&interruption.sa_mask)) {
    perror("sigemptyset failed - subscribe_SIGINT\n");
    return 1;
  }

  if (sigaction(SIGINT, &interruption, NULL)) {
    perror("sigaction failed - subscribe_SIGINT\n");
    return 1;
  }
  return 0;
}

void signals_handler(int signo) {
  switch (signo) {
    case 1:
         if (write_SIGNAL_RECV("SIGHUP")) {
          perror("Failed to write to log file\n");
         }
    case 3:
         if (write_SIGNAL_RECV("SIGQUIT")) {
          perror("Failed to write to log file\n");
         }
    case 10:
         if (write_SIGNAL_RECV("SIGUSR1")) {
          perror("Failed to write to log file\n");
         }
    case 11:
         if (write_SIGNAL_RECV("SIGSEGV")) {
          perror("Failed to write to log file\n");
         }
    case 12:
         if (write_SIGNAL_RECV("SIGUSR2")) {
          perror("Failed to write to log file\n");
         }
    case 13:
         if (write_SIGNAL_RECV("SIGPIPE")) {
          perror("Failed to write to log file\n");
         }
    case 14:
         if (write_SIGNAL_RECV("SIGALRM")) {
          perror("Failed to write to log file\n");
         }
    case 15:
         if (write_SIGNAL_RECV("SIGTERM")) {
          perror("Failed to write to log file\n");
         }
    case 17:
         if (write_SIGNAL_RECV("SIGCHLD")) {
          perror("Failed to write to log file\n");
         }
    default:
         break;
  }
}

int subscribe_osignals() {
  struct sigaction interruption;
  interruption.sa_handler = signals_handler;
  interruption.sa_flags = 0;
  if (sigemptyset(&interruption.sa_mask)) {
    perror("sigemptyset failed - subscribe_osignals\n");
    return 1;
  }

  if (sigaction(SIGHUP, &interruption, NULL)) {
    perror("sigaction failed - subscribe_osignals\n");
    return 1;
  }

  if (sigaction(SIGQUIT, &interruption, NULL)) {
    perror("sigaction failed - subscribe_osignals\n");
    return 1;
  }

  if (sigaction(SIGUSR1, &interruption, NULL)) {
    perror("sigaction failed - subscribe_osignals\n");
    return 1;
  }

  if (sigaction(SIGUSR2, &interruption, NULL)) {
    perror("sigaction failed - subscribe_osignals\n");
    return 1;
  }

  if (sigaction(SIGSEGV, &interruption, NULL)) {
    perror("sigaction failed - subscribe_osignals\n");
    return 1;
  }

  if (sigaction(SIGPIPE, &interruption, NULL)) {
    perror("sigaction failed - subscribe_osignals\n");
    return 1;
  }

  if (sigaction(SIGALRM, &interruption, NULL)) {
    perror("sigaction failed - subscribe_osignals\n");
    return 1;
  }

  if (sigaction(SIGTERM, &interruption, NULL)) {
    perror("sigaction failed - subscribe_osignals\n");
    return 1;
  }

  if (sigaction(SIGCHLD, &interruption, NULL)) {
    perror("sigaction failed - subscribe_osignals\n");
    return 1;
  }

  return 0;
}
