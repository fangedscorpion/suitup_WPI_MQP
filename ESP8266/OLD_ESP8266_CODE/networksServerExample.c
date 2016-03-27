/*
* Amanda Adkins
* CS 3516 Program 1
* 
* Date: 11/11/2014
* using modified version of Donahoo & Calvert book example of TCP Echo Client and Server
*/
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h> /* for sockaddr_in() and inet_ntoa() */
#include <stdlib.h> /* for atoi() and exit() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */

#define MAX_PENDING 5 /* Max outstanding connection requests */
#define BUF_SIZE 93
#define ECHO_PORT 12456

#define MAX_CMD_LEN 92
#define RECEIVE_BUF_SIZE 200

#define MAX_ARGS 5
#define MAX_ARG_LEN 31
#define TRUE 1
#define FALSE 0

struct locationData {
  char lastName[MAX_ARG_LEN];
  char firstName[MAX_ARG_LEN];
  char idNum[MAX_ARG_LEN];
  char location[MAX_ARG_LEN];
  struct locationData *nextNode;
};

enum CommandType {
  login,
  add,
  list, 
  removeCmd,
  quit,
  invalidLength, 
  invalidCmdType
};

void DieWithError(char *errorMessage);/* error handling function */
int HandleTCPClient(int clntSocket, struct locationData ** listHead); /* TCP client handling function */
int handleInput(char *buffer, int clntSocket, int numBytesRcvd, struct locationData ** listHead);
void handleAdd(char *idNum, char *lastName, char *firstName, char *location, int numArgsInCmd, int clntSocket, struct locationData **listHead);
void handleRemove(char *idNum, int numArgsInCmd, int clntSocket, struct locationData **listHead);

void handleList(char *start, char *end, int numArgsInCmd, int clntSocket, struct locationData **listHead);

void writeDBToFile(struct locationData **listHead);

int packetsSent = 0;
int clientsHandled = 0;
int commandsForClient = 0; 


int main(int argc, char *argv[]) {
  int servSock;                       /* Socket descriptor for server */
  int clntSock;                       /* Socket descriptor */
  struct sockaddr_in echoServerAddr;  /* local address */
  struct sockaddr_in echoClntAddr;    /* client address */
  unsigned short echoServPort;        /* server port */
  unsigned int clntLen;               /* length of client address data structure */

  struct locationData **listHead;

  *listHead = 0;

  /* Test for correct number of arguments */
  if (argc != 1) {
    fprintf(stderr, "Usage: %s \n", argv[0]);
    fprintf(stderr, "Try again\n");
    exit(1);
  }

  // create socket for incoming connections 
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    DieWithError("socket() failed");
  }

  /* Construct local address structure */
  memset(&echoServerAddr, 0, sizeof(echoServerAddr));    /* Zero out structure */
  echoServerAddr.sin_family = AF_INET;                   /* Internet address family */
  echoServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);    /* Any incoming interface */
  echoServerAddr.sin_port = htons(ECHO_PORT);            /* local port */
  /* bind to the local address */
  if (bind(servSock, (struct sockaddr*) &echoServerAddr, sizeof(echoServerAddr)) < 0) {
    DieWithError("bind() failed");
  }

  int notFoundQuitEOF = 1;
  while (notFoundQuitEOF) {
    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAX_PENDING) < 0) {
      DieWithError("listen() failed");
    }
    clntLen = sizeof(echoClntAddr);
    if ((clntSock = accept(servSock, (struct sockaddr*) &echoClntAddr, &clntLen)) < 0) {
      DieWithError("accept() failed");
    }
    /* indicate the the server has connected  */
    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
    /* call function to recieve and send back a string from the client */
    clientsHandled++;
    notFoundQuitEOF = HandleTCPClient(clntSock, listHead);
  }
  writeDBToFile(listHead);
}

void DieWithError(char *errorString) {
  fprintf(stderr, "error: %s\n", errorString);
  exit(1);
}

/* 
 * clntSocket - socket descriptor for connection between client and server
 * get string from client and sent it back
 */
int HandleTCPClient(int clntSocket, struct locationData ** listHead) {
  char buffer[BUF_SIZE];
  commandsForClient = 0;
  int noEOF = TRUE;
  int dontSend = FALSE;
  // receive data from client 
  ssize_t numBytesRcvd = recv(clntSocket, buffer, BUF_SIZE, 0);
  if (numBytesRcvd < 0) {
    DieWithError("recv() failed");
  }
  int bufferLen;
  while (numBytesRcvd > 0) {
    commandsForClient++;
    noEOF = handleInput(buffer, clntSocket, numBytesRcvd, listHead);
    numBytesRcvd = recv(clntSocket, buffer, BUF_SIZE, 0);
    if (numBytesRcvd < 0) {
      DieWithError("recv() failed");
    }  
  }
  // close socket 
  close(clntSocket);

  return noEOF;
}


int handleInput(char *buffer, int clntSocket, int numBytesRcvd, struct locationData ** listHead) {
  int noEOF = TRUE;
  int bufferLen;
  buffer[numBytesRcvd] = '\0';
  enum CommandType typeOfCommand = invalidCmdType;
  int numArgsInCmd;
  char fullCommand[RECEIVE_BUF_SIZE];
  char commandArguments[MAX_ARGS][RECEIVE_BUF_SIZE];
  static char name[MAX_ARG_LEN];

  strncpy(fullCommand, buffer, MAX_CMD_LEN);

  int commandLen = strlen(buffer);
  
  if (fullCommand[commandLen-1] == '\n') {
    fullCommand[commandLen-1] = '\0';
  }
  char *delim = " ";
  int argCount = 0;
  char *returnString;
  returnString = strtok(fullCommand, delim);
  
  while (returnString != NULL) {
    strncpy(commandArguments[argCount], returnString, MAX_ARG_LEN);
    argCount++;
    returnString = strtok(NULL, delim);
  }

  numArgsInCmd = argCount;

  if (argCount == 0) {
    printf("Invalid command length\n");
    typeOfCommand = invalidLength;
  }
  else {
    if (!(strcasecmp("login", commandArguments[0]))) {
       typeOfCommand = login;
    }
    else if (!(strcasecmp("list", commandArguments[0]))) {
       typeOfCommand = list;
    }
    else if (!(strcasecmp("remove", commandArguments[0]))) {
      typeOfCommand = removeCmd;
    }
    else if (!(strcasecmp("quit", commandArguments[0]))) {
      typeOfCommand = quit;
    }
    else if (!(strcasecmp("add", commandArguments[0]))) {
      typeOfCommand = add;
    }
    else {
      typeOfCommand = invalidCmdType;
    }
  } 
  
  if (typeOfCommand == add) {
    if (numArgsInCmd != 5) {
      sprintf(buffer, "not a valid add command\n");
      bufferLen = strlen(buffer);
      ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
      packetsSent++;
      if (numBytesSent < 0) {
        DieWithError("send() failed");
      }
      else if (numBytesSent != bufferLen) {
        DieWithError("send() sent unexpected number of bytes");
      }
    } 
    else {
      handleAdd(commandArguments[1], commandArguments[3], commandArguments[2], commandArguments[4], numArgsInCmd, clntSocket, listHead);
    }
  }
  else if (typeOfCommand == removeCmd) {
    if (numArgsInCmd != 2) {
      sprintf(buffer, "not a valid remove command\n");
      bufferLen = strlen(buffer);
      ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
      packetsSent++;
      if (numBytesSent < 0) {
        DieWithError("send() failed");
      }
      else if (numBytesSent != bufferLen) {
        DieWithError("send() sent unexpected number of bytes");
      }
    } 
    else {
      handleRemove(commandArguments[1], numArgsInCmd, clntSocket, listHead);
    }
  }
  else if (typeOfCommand == list) {
    ssize_t numBytesSent;
    if (numArgsInCmd > 3) {
      sprintf(buffer, "not a valid add command\n");
      bufferLen = strlen(buffer);
      ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
      packetsSent++;
      if (numBytesSent < 0) {
        DieWithError("send() failed");
      }
      else if (numBytesSent != bufferLen) {
        DieWithError("send() sent unexpected number of bytes");
      }
    }
    else if (numArgsInCmd == 3) {
      handleList(commandArguments[1], commandArguments[2], numArgsInCmd, clntSocket, listHead);
    }
    else if (numArgsInCmd == 2) {
      handleList(commandArguments[1], "", numArgsInCmd, clntSocket, listHead);
    }
    else {
      handleList("", "", numArgsInCmd, clntSocket, listHead);
    }
    sprintf(buffer, "DONE\n");

    bufferLen = strlen(buffer);  
    numBytesSent = send(clntSocket, buffer, bufferLen, 0);
    packetsSent++;
    if (numBytesSent < 0) {
      DieWithError("send() failed");
    }
    else if (numBytesSent != bufferLen) {
      DieWithError("send() sent unexpected number of bytes");
    }
  }
  else if (typeOfCommand == quit) {
    if (!(strncasecmp(buffer, "quit EOF", 8))) {
      noEOF = FALSE;
      sprintf(buffer, "session for %s closed with %d commands handled, %d clientsHandled.\n", name, commandsForClient, clientsHandled);
    }
    else {
      sprintf(buffer, "session for %s closed with %d commands handled\n", name, commandsForClient);
    }
    bufferLen = strlen(buffer);

    ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
    packetsSent++;
    if (numBytesSent < 0) {
      DieWithError("send() failed");
    }
    else if (numBytesSent != bufferLen) {
      DieWithError("send() sent unexpected number of bytes");
    }
    
  }
  else if (typeOfCommand == invalidCmdType) {
    strncpy(buffer, "Invalid command\n", BUF_SIZE);
    bufferLen = strlen(buffer);
  
    ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
    packetsSent++;
    if (numBytesSent < 0) {
      DieWithError("send() failed");
    }
    else if (numBytesSent != bufferLen) {
      DieWithError("send() sent unexpected number of bytes");
    }  
  }
  else if (typeOfCommand == login) {
    sprintf(buffer, "Hello %s!\n", commandArguments[1]);
    strncpy(name, commandArguments[1], MAX_ARG_LEN-1);
    bufferLen = strlen(buffer);  

  ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
  packetsSent++;
  if (numBytesSent < 0) {
    DieWithError("send() failed");
  }
  else if (numBytesSent != bufferLen) {
    DieWithError("send() sent unexpected number of bytes");
  }

  }


  return noEOF;
}


void handleAdd(char *idNum, char *lastName, char *firstName, char *location, int numArgsInCmd, int clntSocket, struct locationData **listHead) {
  char buffer[RECEIVE_BUF_SIZE];
  int bufferLen;

  struct locationData *current = *listHead;

  int allValid = TRUE;
  if (strlen(idNum) != 9) {
    allValid = FALSE;
  }
  else if (strlen(lastName) > 25) {
    allValid = FALSE;
  }
  else if (strlen(firstName) > 20) {
    allValid = FALSE;
  }
  else if (strlen(location) > 30) {
    allValid = FALSE;
  }
  if (!allValid) {
    sprintf(buffer, "not a valid add command\n");
    bufferLen = strlen(buffer);
    ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
    packetsSent++;
    if (numBytesSent < 0) {
      DieWithError("send() failed");
    }
    else if (numBytesSent != bufferLen) {
      DieWithError("send() sent unexpected number of bytes");
    }
    return;
  }

  int foundDuplicateID = FALSE;
  int notDoneSearching = TRUE;
  while (notDoneSearching) {
    if (current == 0) {
      notDoneSearching = FALSE;
    }
    else if (!(strcasecmp(current->idNum, idNum))) {
      foundDuplicateID = TRUE;
      notDoneSearching = FALSE;
    }
    else {
      current = current->nextNode;
    }
  }
  if (foundDuplicateID) {
    sprintf(buffer, "Duplicate ID\n");
    bufferLen = strlen(buffer);
    ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
    packetsSent++;
    if (numBytesSent < 0) {
      DieWithError("send() failed");
    }
    else if (numBytesSent != bufferLen) {
      DieWithError("send() sent unexpected number of bytes");
    }
    return;
  }
  
  struct locationData * newLoc = malloc(sizeof (struct locationData));
  strncpy(newLoc->idNum, idNum, MAX_ARG_LEN);
  strncpy(newLoc->firstName, firstName, MAX_ARG_LEN);
  strncpy(newLoc->lastName, lastName, MAX_ARG_LEN);
  strncpy(newLoc->location, location, MAX_ARG_LEN);
  
  int doneAdding = FALSE;
  struct locationData *head = *listHead;
  if (head == 0) {
    doneAdding = TRUE;
    *listHead = newLoc;
    newLoc->nextNode = 0;
  }
  else if (strcmp((newLoc->lastName), (head->lastName)) < 0) {
    newLoc->nextNode = head;
    *listHead = newLoc;
    doneAdding = TRUE; 
  }
  else {
    struct locationData *prevPtr = head;
    while (!doneAdding) {
      if (prevPtr->nextNode  == 0) {
        prevPtr->nextNode = newLoc;
        newLoc->nextNode = 0;
        doneAdding = TRUE;
      }
      else if (strcmp((prevPtr->nextNode->lastName), (newLoc->lastName)) > 0) {
        newLoc->nextNode = prevPtr->nextNode;
        prevPtr->nextNode = newLoc;
        doneAdding = TRUE;
      }
      else {
        prevPtr = prevPtr->nextNode;
      }
    }
  }
  sprintf(buffer, "Saved %s %s %s %s\n", idNum, firstName, lastName, location);
  bufferLen = strlen(buffer); 

  ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
  packetsSent++;
  
  if (numBytesSent < 0) {
    DieWithError("send() failed");
  }
  else if (numBytesSent != bufferLen) {
    DieWithError("send() sent unexpected number of bytes");
  }  
}

void handleRemove(char *idNum, int numArgsInCmd, int clntSocket, struct locationData **listHead) {
  char buffer[RECEIVE_BUF_SIZE];
  int bufferLen;

  struct locationData *current = *listHead;

  int allValid = TRUE;
  if (strlen(idNum) != 9) {
    allValid = FALSE;
  }
  if (!allValid) {
    sprintf(buffer, "not a valid remove command\n");
    bufferLen = strlen(buffer);
    ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
    packetsSent++;
    if (numBytesSent < 0) {
      DieWithError("send() failed");
    }
    else if (numBytesSent != bufferLen) {
      DieWithError("send() sent unexpected number of bytes");
    }
    return;
  }
  struct locationData *removeLoc;
  int foundID = FALSE;
  int doneSearching = FALSE;
  struct locationData *head = *listHead;
  if (head == 0) {
    doneSearching = TRUE;
  }
  else if (!(strcmp(head->idNum, idNum))) {
    foundID = TRUE; 
    removeLoc = head;
    doneSearching = TRUE;
    *listHead = head->nextNode;
  }
  else {
    struct locationData *prevPtr = head;
    while (!doneSearching) {
      if (prevPtr->nextNode  == 0) {
        doneSearching = TRUE;
      }
      else if (!strcmp((prevPtr->nextNode->idNum), idNum)) {
        doneSearching = TRUE;
        removeLoc = prevPtr->nextNode;
        prevPtr->nextNode = prevPtr->nextNode->nextNode;
        foundID = TRUE;
      }
      else {
        prevPtr = prevPtr->nextNode;
      }
    }
  }

  if (foundID) {
    sprintf(buffer, "Removed %s %s %s %s\n", removeLoc->idNum, removeLoc->firstName, removeLoc->lastName, removeLoc->location);
    free(removeLoc);
  }
  else {
    sprintf(buffer, "ID not found.\n");
  }
  bufferLen = strlen(buffer); 

  ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
  packetsSent++;
  
  if (numBytesSent < 0) {
    DieWithError("send() failed");
  }
  else if (numBytesSent != bufferLen) {
    DieWithError("send() sent unexpected number of bytes");
  }  
}

void handleList(char *start, char *end, int numArgsInCmd, int clntSocket, struct locationData **listHead) {
  char buffer[RECEIVE_BUF_SIZE];
  int bufferLen;
  int listCount;

  int validArgs = TRUE;
  if (numArgsInCmd == 3) {
    if (strlen(start) != 2) {
      validArgs = FALSE;
    }
    else if (strlen(end) != 2) {
      validArgs = FALSE;
    }
    else if (strcasecmp(start, end) > 0) {
      validArgs = FALSE;
    }
  }
  else if (numArgsInCmd == 2) {
    if (strlen(start) != 1) {
      validArgs = FALSE;
    }
  }
  if (!validArgs) {
    sprintf(buffer, "Invalid list command.\n");
    bufferLen = strlen(buffer); 

    ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
    packetsSent++;
  
    if (numBytesSent < 0) {
      DieWithError("send() failed");
    }
    else if (numBytesSent != bufferLen) {
      DieWithError("send() sent unexpected number of bytes");
    }
    return;
  } 

  if (numArgsInCmd == 1) {
    struct locationData *current = *listHead;
    listCount = 0;

    while(current != 0) {
      listCount++;
      sprintf(buffer, "%s, %s, %s, %s\n", current->idNum, current->firstName, current->lastName, current->location);
      bufferLen = strlen(buffer); 
  
      ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
      packetsSent++;
    
      if (numBytesSent < 0) {
        DieWithError("send() failed");
      }
      else if (numBytesSent != bufferLen) {
        DieWithError("send() sent unexpected number of bytes");
      }   
      current = current->nextNode;   
    }
  }
  else if (numArgsInCmd == 2) {
    listCount = 0;
    struct locationData *current = *listHead;
    int keepLooping = TRUE;

    while (keepLooping) {
      if (current == 0) {
        keepLooping = FALSE;
      }
      else if (strncmp(start, current->lastName, 1) != 0) {
        current = current->nextNode;
      }
      else if (strncmp(start, current->lastName, 1) == 0 ){
        listCount++;
        sprintf(buffer, "%s, %s, %s, %s\n", current->idNum, current->firstName, current->lastName, current->location);
        bufferLen = strlen(buffer); 
  
        ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);

        packetsSent++;
    
        if (numBytesSent < 0) {
          DieWithError("send() failed");
        }
        else if (numBytesSent != bufferLen) {
          DieWithError("send() sent unexpected number of bytes");
        }
        current = current->nextNode;
      }   
    }

  }
  else if (numArgsInCmd == 3) {
    listCount = 0;
    struct locationData *current = *listHead;
    int keepLooping = TRUE;

    while (keepLooping) {
      if (current == 0) {
        keepLooping = FALSE;
      }
      else if (strncmp(end, current->lastName, 2) < 0) {
        keepLooping = FALSE;
      }
      else if (strncmp(start, current->lastName, 2) > 0) {
        current = current->nextNode;
      }
      else {
        listCount++;
        sprintf(buffer, "%s, %s, %s, %s\n", current->idNum, current->firstName, current->lastName, current->location);
        bufferLen = strlen(buffer); 
  
        ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);

        packetsSent++;
    
        if (numBytesSent < 0) {
          DieWithError("send() failed");
        }
        else if (numBytesSent != bufferLen) {
          DieWithError("send() sent unexpected number of bytes");
        }
        current = current->nextNode;
      }   
    }
  }

  if (listCount == 0) {
    sprintf(buffer, "No entries found.\n");
    bufferLen = strlen(buffer); 
  
   ssize_t numBytesSent = send(clntSocket, buffer, bufferLen, 0);
   packetsSent++;
    
    if (numBytesSent < 0) {
      DieWithError("send() failed");
    }
    else if (numBytesSent != bufferLen) {
      DieWithError("send() sent unexpected number of bytes");
    }  
  }
}

void writeDBToFile(struct locationData **listHead) {
  FILE *dbFile;
  dbFile = fopen("./LDatabase.txt", "w+");
  char writeBuff[RECEIVE_BUF_SIZE];
  fputs("idNum      firstName   lastName   location\n", dbFile);
  struct locationData *current = *listHead;
  while (current != 0) {
    sprintf(writeBuff, "%s, %s, %s, %s\n", current->idNum, current->firstName, current->lastName, current->location);
    fputs(writeBuff, dbFile);
    current = current->nextNode;
  }
}
