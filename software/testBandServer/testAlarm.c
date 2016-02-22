/*
* Amanda Adkins
* CS 3516 Program 0
* 
* Date: 11/1/2014
* using modified version of Donahoo & Calvert book example of TCP Echo Client and Server
*/
#include <stdio.h>            /* for printf() and fprintf() */
#include <sys/socket.h>       /* for socket(), bind(), and connect() */
#include <arpa/inet.h>        /* for sockaddr_in() and inet_ntoa() */
#include <stdlib.h>           /* for atoi() and exit() */
#include <string.h>           /* for memset() */
#include <unistd.h>           /* for close() */
#include <signal.h>


#define MAX_PENDING 5         /* Max outstanding connection requests */
#define BUF_SIZE 32           /* size of string buffer for recieving  */
#define TRUE 1
#define FALSE 0
#define RECV 7
#define ALARM_INTERVAL 1
#define SEC_TIMEOUT 0
#define USEC_TIMEOUT 10000



volatile int alarmActivated = FALSE;

void alarmHandler(int sig) {
  signal(SIGALRM, SIG_IGN);
  alarmActivated = TRUE;
  signal(SIGALRM, alarmHandler);
}


int main(int argc, char *argv[]) {
  signal(SIGALRM, alarmHandler);
  alarm(ALARM_INTERVAL);
  while (TRUE) {
    if (alarmActivated) {
      printf("ALARM ACTIVATED\n");
      alarmActivated = FALSE;
      alarm(ALARM_INTERVAL);
    }
  }

}