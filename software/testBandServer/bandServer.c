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

#define MAX_PENDING 5         /* Max outstanding connection requests */
#define BUF_SIZE 32           /* size of string buffer for recieving  */
#define TRUE 1
#define RECV 7

void DieWithError(char *errorMessage);/* error handling function */
void HandleTCPClient(int clntSocket); /* TCP client handling function */
int constructMsg(char *sendBuff, char *dataBuf, char msgType);
int processReply(char *recvdBuffer, char* sendBuff);

void reverseString(char *stringToReverse);

typedef enum {
  COMPUTER_INITIATE_CONNECTION, 
  BAND_CONNECTING, 
  COMPUTER_PING, 
  BAND_PING, 
  BAND_POSITION_UPDATE, 
  POSITION_ERROR,
  START_RECORDING, 
  STOP_RECORDING, 
  START_HAPTICS, 
  STOP_HAPTICS, 
  VOICE_CONTROL} msgType;


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
  if (setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
    DieWithError("setsockopt(SO_REUSEADDR) failed");

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
  fprintf(stderr, "error: %s\n", errorString);
  exit(1);
}

/* 
 * clntSocket - socket descriptor for connection between client and server
 * get string from client and sent it back
 */
void HandleTCPClient(int clntSocket) {
  char buffer[BUF_SIZE];
  int msgTypeInt;
  char msgType;
  char tmpBuff[BUF_SIZE];
  int dataLen;
  //ssize_t numBytesRcvd;
  // receive the data from the client */
  ssize_t numBytesRcvd = recv(clntSocket, buffer, BUF_SIZE, 0);
  if (numBytesRcvd < 0) {
    DieWithError("recv() failed");
  }

  dataLen = processReply(buffer, tmpBuff);

  // while bytes are being trasmitted, keep sending and receiving data
  while (TRUE) {
    printf("Input here:");
    //scanf("%d %s", &msgTypeInt, tmpBuff);
    msgType = (char) *(buffer + 1);


    //strncpy(tmpBuff, buffer + 2, BUF_SIZE);
    //reverseString(tmpBuff);

    //constructMsg(buffer, tmpBuff, msgType);
    // send back the message just received 
    ssize_t numBytesSent = send(clntSocket, tmpBuff, dataLen, 0);
    if (numBytesSent < 0) {
      DieWithError("send() failed");
    }
    
    // receive more data
    numBytesRcvd = recv(clntSocket, buffer, BUF_SIZE, 0);
    if (numBytesRcvd < 0) {
      DieWithError("recv() failed");
    }
    processReply(buffer, tmpBuff);
  }
  // close the socket 
  close(clntSocket);
}

int constructMsg(char *sendBuff, char *dataBuf, char msgType) {
  int len = strlen(dataBuf);
  sendBuff[0] = (char) (len+2);
  sendBuff[1] = msgType;

  strncpy(sendBuff + 2, dataBuf, BUF_SIZE - 2);
  if ((len + 3) < (BUF_SIZE - 1)) {
    sendBuff[len+2] = '\n';
    sendBuff[len+2] = '\0';
  } else {
    sendBuff[BUF_SIZE - 2] = '\n';
    sendBuff[BUF_SIZE - 1] = '\0';
  }

  printf("Sending\n");
  printf("length: %d\n", ((int) sendBuff[0] - 2));
  printf("msgType: %d\n", (int) sendBuff[1]);
  printf("msg: %s\n\n", sendBuff + 2);
  return len + 3;
}

int processReply(char *recvdBuffer, char *sendBuff) {
  int dataLen;
  printf("Received:\n");
  printf("length: %d\n", ((int) recvdBuffer[0] - 2));
  printf("msgType: %d\n", (int) recvdBuffer[1]);
  printf("msg: %s\n", recvdBuffer + 2);

  printf("initiate connection type: %d\n", (int) COMPUTER_INITIATE_CONNECTION);

  msgType typeOfMsg = (msgType) recvdBuffer[1];
  printf("typeOfMsg %d\n", (int) typeOfMsg);
  switch(typeOfMsg) {
    case COMPUTER_INITIATE_CONNECTION:
      sendBuff[0] = (char) 2;
      sendBuff[1] = (char) BAND_CONNECTING;
      sendBuff[2] = '\n';
      printf("Initiating connection\n");
      dataLen = 3;
       break;
    case COMPUTER_PING:
      sendBuff[0] = (char) 2;
      sendBuff[1] = (char) BAND_PING;
      sendBuff[2] = '\n';
      dataLen = 3;
      printf("returning ping\n");
    break;
    /*case POSITION_ERROR:
    break;
    case START_RECORDING:
    break;
    case STOP_RECORDING:
    break;
    case START_HAPTICS:
    break;
    case STOP_HAPTICS:
    break;
    */default:
      printf("Default case\n");
      int recvdLen = strlen(recvdBuffer);
      recvdBuffer[recvdLen - 1] = '\0';
      reverseString(recvdBuffer + 2);
      dataLen = constructMsg(sendBuff, recvdBuffer + 2, (char) recvdBuffer[1]);
      // this shouldn't happen
    break;
  }
  printf("Done with case statement");
  return dataLen;
}

void reverseString(char *stringToReverse) {
  int len = strlen(stringToReverse);
  for (int i = 0; i < len/2; i++) {
    char tmpChar = stringToReverse[i];
    stringToReverse[i] = stringToReverse[len - i - 1];
    stringToReverse[len - i - 1] = tmpChar;
  }
}