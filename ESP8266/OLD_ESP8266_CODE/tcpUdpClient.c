#include <stdio.h>       /* for printf() and fprintf() */
#include <sys/socket.h>  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>   /* for sockiaddr_in and inet_addr*() */
#include <stdlib.h>      /* for atoi() and exit() */
#include <string.h>      /* for memset() */
#include <unistd.h>      /* for close() */
#include <errno.h>

#define MAX_STR_LEN 32                   /* maximum size of string to transmit */
#define MAX_RECV_LEN
#define RECEIVE_BUF_SIZE MAX_RECV_LEN + 1  /* size of buffer for transmitting and recieving */

#define PORT 12345
#define TRUE 1
#define FALSE 0

void DieWithError(char *errorMessage); /* error handling function */
void sendString(int sock, char* stringToSend, int lengthOfString);


int main(int argc, char *argv[]){
  int sock;                           /* socket descriptor */
  struct sockaddr_in echoServerAddr;  /* Echo server address */
  char *servIP;                       /* server IP address */
  char *echoString;                   /* string to echo */
  unsigned int echoStringLen;         /* length of string to transmit */
  int sendingSingleWord;
  sendingSingleWord = FALSE;

  // check if there are the appropriate number of arguments
  if ((argc != 2) && (argc != 3)) {
    fprintf(stderr, "Usage: %s <Server IP> [word to send]\n", argv[0]);
    fprintf(stderr, "Try again.\n");
    exit(1);
  }

  // get the IP address and string to echo from the command line
  servIP = argv[1];
  if (argc == 3) {
    echoString = argv[2];

    // check to make sure the input string isn't too long
    if (strlen(echoString) > MAX_STR_LEN) {
      DieWithError("Input string can be at most 32 bytes.");
    }
    sendingSingleWord = TRUE;
  }

  // create socket descriptor
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    DieWithError("socket() failed");
  }

  memset(&echoServerAddr, 0, sizeof(echoServerAddr)); /* zero out structure */
  echoServerAddr.sin_family = AF_INET;                /* internet address family */
  echoServerAddr.sin_addr.s_addr = inet_addr(servIP); /* server ip address */
  echoServerAddr.sin_port = htons(PORT);      /* server port */
  /* connect to server */
  if (connect(sock, (struct sockaddr*) &echoServerAddr, sizeof(echoServerAddr)) < 0) {
    DieWithError("connect() failed");
  }

  if (sendingSingleWord) {
    sendString(sock, echoString, strlen(echoString));
  }
  else {
    printf("Type exit to quit\n");
    char stringRead[MAX_STR_LEN];
    while (TRUE) {
      // read line
      fgets(stringRead, MAX_STR_LEN, stdin);
      if (strncmp(stringRead, "exit", 6) == 0) {
        break;
      }
      int stringLen = strlen(stringRead);
      sendString(sock, stringRead, stringLen);
    }
  }
    // close and exit
    close(sock);
    exit(0);

}

void sendString(int sock, char* stringToSend, int lengthOfString) {
    char echoBuffer[RECEIVE_BUF_SIZE];  /* buffer for receving */
    int bytesRcvd, totalBytesRcvd;      /* byes read in single recv and total bytes read */
  
    bytesRcvd = 5;
  
    /* send the string to the server*/
    if (send(sock, stringToSend, lengthOfString, 0) != lengthOfString){
      DieWithError("send sent a different number of bytes than expected");
    }

    /* receive the string back grom the server */
    totalBytesRcvd = 0;
    printf("Received: \n");  /* setup to print echoed string */
    /* receive until all data is echoed back */
    while (bytesRcvd != 0) {
      printf("Trying to recieve\n");
      if ((bytesRcvd = recv(sock, echoBuffer, RECEIVE_BUF_SIZE-1, 0)) < 0) {
        printf("Error code: %d, %s\n", errno, strerror(errno));
        DieWithError("recv() failed or connection closed prematurely");
      }
      printf("Waiting to recieve\n");
      totalBytesRcvd += bytesRcvd;
      // add the null terminator so the string can be printed
      echoBuffer[bytesRcvd] = '\0';
      // print the string
      printf("%s", echoBuffer);
    }
    // print a new line after the string
    printf("\n");
}


void DieWithError(char *errorString) {
  fprintf(stderr, "error: %s\n", errorString);
  exit(1);
}
