
#include <stdio.h>            /* for printf() and fprintf() */
#include <sys/socket.h>       /* for socket(), bind(), and connect() */
#include <arpa/inet.h>        /* for sockaddr_in() and inet_ntoa() */
#include <stdlib.h>           /* for atoi() and exit() */
#include <string.h>           /* for memset() */
#include <unistd.h>           /* for close() */
#include <errno.h>

#define MAX_PENDING 5         /* Max outstanding connection requests */
#define BUF_SIZE 32           /* size of string buffer for recieving  */

void DieWithError(char *errorMessage);/* error handling function */
void HandleTCPClient(int clntSocket); /* TCP client handling function */
void reverseString(char* stringToReverse, int stringlength);

int main(int argc, char *argv[]) {
  int servSock;                       /* Socket descriptor for server */
  int clntSock;                       /* Socket descriptor */
  struct sockaddr_in echoServerAddr;  /* local address */
  struct sockaddr_in echoClntAddr;    /* client address */
  unsigned short echoServPort;        /* server port */
  unsigned int clntLen;               /* length of client address data structure */

  /* Test for correct number of arguments */
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <ServerPort>\n", argv[0]);
    fprintf(stderr, "Try again\n");
    exit(1);
  }

  echoServPort = atoi(argv[1]);  /* first arg: local port */

  /*  create socket for incoming connections */
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    DieWithError("socket() failed");
  }

  /* Construct local address structure */
  memset(&echoServerAddr, 0, sizeof(echoServerAddr));    /* Zero out structure */
  echoServerAddr.sin_family = AF_INET;                   /* Internet address family */
  echoServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);    /* Any incoming interface */
  echoServerAddr.sin_port = htons(echoServPort);         /* local port */

  /* bind to the local address */
  if (bind(servSock, (struct sockaddr*) &echoServerAddr, sizeof(echoServerAddr)) < 0) {
    DieWithError("bind() failed");
  }

  /* Markt the socket so it will listen for incoming connections */
  if (listen(servSock, MAX_PENDING) < 0) {
    DieWithError("listen() failed");
  }

  /* set the size of the in-out parameter */
  clntLen = sizeof(echoClntAddr);
  /* wait for client to connect*/
  if ((clntSock = accept(servSock, (struct sockaddr*) &echoClntAddr, &clntLen)) < 0) {
    DieWithError("accept() failed");
  }
  /* indicate the the server has connected  */
  printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
  /* call function to recieve and send back a string from the client */
  HandleTCPClient(clntSock);

}


/* prints out error message and exits program */
void DieWithError(char *errorString) {
  fprintf(stderr, "code: %d, %s\n", errno, strerror(errno));
  fprintf(stderr, "error: %s\n", errorString);
  exit(1);
}

/*
 * clntSocket - socket descriptor for connection between client and server
 * get string from client and sent it back
 */
void HandleTCPClient(int clntSocket) {
  char buffer[BUF_SIZE];
  // receive the data from the client */
  ssize_t numBytesRcvd = recv(clntSocket, buffer, BUF_SIZE, 0);
  if (numBytesRcvd < 0) {
    DieWithError("recv() failed");
  }
  // while bytes are being trasmitted, keep sending and receiving data
  while (numBytesRcvd > 0) {
    // send back the message just received
    reverseString(buffer, numBytesRcvd -1);
    ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
    if (numBytesSent < 0) {
      DieWithError("send() failed");
    }
    else if (numBytesSent != numBytesRcvd) {
      DieWithError("send() sent unexpected number of bytes");
    }
    // receive more data
    numBytesRcvd = recv(clntSocket, buffer, BUF_SIZE, 0);
    if (numBytesRcvd < 0) {
      DieWithError("recv() failed");
    }
  }
  // close the socket
  close(clntSocket);
}

void reverseString(char* stringToReverse, int stringlength) {
  int i;
  char beginChar;

  for (i = 0; i < stringlength/2; i++) {
    beginChar = stringToReverse[i];
    stringToReverse[i] = stringToReverse[stringlength - i - 1];
    stringToReverse[stringlength - i - 1] = beginChar;    
  }
}
