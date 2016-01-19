#include <sys/socket.h>
#include <arpa/inet.h>        /* for sockaddr_in() and inet_ntoa() */
#include <string.h>           /* for memset() */
#include <unistd.h>           /* for close() */
#include <QString>
#include <QQuaternion>
#include "networkmonitor.h"

#define BUFFER_SIZE 1024


NetworkMonitor::NetworkMonitor(int portNumber):QObject() {

  int servSock;                       /* Socket descriptor for server */
  int clntSock;                       /* Socket descriptor */
  struct sockaddr_in echoServerAddr;  /* local address */
  struct sockaddr_in echoClntAddr;    /* client address */
  unsigned int clntLen;               /* length of client address data structure */

  /*  create socket for incoming connections */
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    qDebug("Socket failed. Restart program");
  }

  /* Construct local address structure */
  memset(&echoServerAddr, 0, sizeof(echoServerAddr));    /* Zero out structure */
  echoServerAddr.sin_family = AF_INET;                   /* Internet address family */
  echoServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);    /* Any incoming interface */
  echoServerAddr.sin_port = htons(portNumber);         /* local port */

  /* bind to the local address */
  if (bind(servSock, (struct sockaddr*) &echoServerAddr, sizeof(echoServerAddr)) < 0) {
    qDebug("bind() failed");
  }

  /* Markt the socket so it will listen for incoming connections */
  if (listen(servSock, 1) < 0) {
    qDebug("listen() failed");
  }

  /* set the size of the in-out parameter */
  clntLen = sizeof(echoClntAddr);
  /* wait for client to connect*/
  if ((clntSock = accept(servSock, (struct sockaddr*) &echoClntAddr, &clntLen)) < 0) {
    qDebug("Error accepting connection. Please restart program");
  }
  /* indicate the the server has connected  */
  QString ipString = QString(inet_ntoa(echoClntAddr.sin_addr));
  emit acceptedConnection(ipString);

  while(true) {
      // receive data

  }
}
NetworkMonitor::~NetworkMonitor()
{
    close(clntSocket);
}

/*
 * clntSocket - socket descriptor for connection between client and server
 * get string from client and sent it back
 */
/* void HandleTCPClient(int clntSocket) {
  char buffer[BUF_SIZE];
  // receive the data from the client */
  /* ssize_t numBytesRcvd = recv(clntSocket, buffer, BUF_SIZE, 0);
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
} */


void NetworkMonitor::sendMotorCommand() {

}

void NetworkMonitor::sendData(char * serializedData, int dataLen) {

}

// returns number of bytes of data sent
int NetworkMonitor::recieveData(char *dataBuffer) {
    //recv();

    emit receievedQuaternion(QQuaternion(0, 1, 2, 3));
}
