/*
 * Amanda Adkins
 * CS 3516 Program 1
 * 
 * Date: 11/11/2014
 * using modified version of Donahoo & Calvert book example of TCP Echo Client and Server
*/

#include <stdio.h>       /* for printf() and fprintf() */
#include <sys/socket.h>  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>   /* for sockiaddr_in and inet_addr*() */
#include <stdlib.h>      /* for atoi() and exit() */
#include <string.h>      /* for memset() */
#include <unistd.h>      /* for close() */
#include <netdb.h>       /* for gethostbyname() */

#define MAX_CMD_LEN 92
#define RECEIVE_BUF_SIZE MAX_CMD_LEN + 1  /* size of buffer for transmitting and recieving */

#define ECHO_PORT 12456       /* port to use */

#define MAX_ARGS 5
#define MAX_ARG_LEN 30
#define TRUE 1
#define FALSE 0

#define RECV_SIZE 200

enum CommandType {
  login,
  add,
  list, 
  removeCmd,
  quit,
  invalidLength, 
  invalidCmdType
};

void DieWithError(char *errorMessage);  /* error handling function */


void parseCommand(char *fullCommand, char commandArguments[MAX_ARGS][MAX_ARG_LEN], int *numArgsInCmd, enum CommandType *typeOfCommand);

int main(int argc, char *argv[]){
  int sock;                           /* socket descriptor */
  struct sockaddr_in echoServerAddr;  /* Echo server address */
  unsigned short echoServPort;        /* echo server port */
  char *servIP;                       /* server IP address */
  char *echoString; /* array for strings to echo */
  char echoBuffer[RECV_SIZE];  /* buffer for receving */
  unsigned int echoStringLen;         /* length of string to transmit */ 
  int bytesRcvd, totalBytesRcvd;      /* byes read in single recv and total bytes read */
  int textFromFile = FALSE;
  FILE *inputFile;
  FILE *logFile;
  int continueReading = TRUE;
  char currentCommand[MAX_CMD_LEN + 1];
  char commandArguments[MAX_ARGS][MAX_ARG_LEN + 1];
  enum CommandType typeOfCommand;
  int numArgsInCmd;
  char fullCommandUnparsed[RECEIVE_BUF_SIZE];
  char receivedData[RECEIVE_BUF_SIZE];

  // check if there are the appropriate number of arguments
 if ((argc < 2) || (argc > 3)) {
    fprintf(stderr, "Usage: %s <Server Machine> [<optional text file>] \n", argv[0]);
    fprintf(stderr, "Try again.\n");
    exit(1);
  }
  else if (argc == 3) {
    // command line input
    textFromFile = TRUE;

    inputFile = fopen(argv[2], "r");
    if (inputFile == NULL) {
      DieWithError("Invalid file");
    }

  }
  else if (argc == 2) {
    inputFile = stdin;
  }

  logFile = fopen("./LClient.log", "w+");
  
  // get the ServerMachine name from the command line and determine the 
  // corresponding IP address
  /*  server name to IP based off code from  http://www.beej.us/guide/bgnet/output/html/multipage/gethostbynameman.html */
  struct hostent *host;
  struct in_addr **addr_list;
  
  if (( host = gethostbyname(argv[1])) == NULL) {
    DieWithError("gethostbyname() failed");
  }

  addr_list = (struct in_addr **) host->h_addr_list;
  servIP = inet_ntoa(*addr_list[0]);

  int isLoggedIn = FALSE;
  char *fgetReturn;
  int goodToSend = FALSE;
  int closeLogFile = FALSE;

  while (continueReading) {
    // get next command
    fgetReturn = fgets(currentCommand, RECEIVE_BUF_SIZE, inputFile);
    // if end of file
    if (fgetReturn == NULL) {
      continueReading = FALSE;
      closeLogFile = TRUE;
      char backUpEOF[10] = "quit EOF\n";
      if (isLoggedIn) {
        if (send(sock, fullCommandUnparsed, echoStringLen, 0) != echoStringLen){
          DieWithError("send sent a different number of bytes than expected");

totalBytesRcvd = 0;
	  while (strstr(echoBuffer, "\n") == NULL) {
	    if ((bytesRcvd = recv(sock, echoBuffer, RECV_SIZE-1, 0)) <= 0) {
              DieWithError("recv() failed or connection closed prematurely");
            }
            totalBytesRcvd += bytesRcvd;
            echoBuffer[bytesRcvd] = '\0';
            printf("%s", echoBuffer);
          }
          fputs(echoBuffer, logFile);
        }
      }
    }
    else {
      goodToSend = FALSE;
      
      int commandLen = strlen(currentCommand);
      int j;
      for (j = 0; j < strlen(currentCommand); j++) {
        currentCommand[j] = tolower(currentCommand[j]);
      }
  
      if ((commandLen == MAX_CMD_LEN) && (currentCommand[commandLen-1] != '\n')) {
        printf("Invalid command length\n");
        typeOfCommand = invalidLength;
        numArgsInCmd = 0;
      }
      else {
        if (currentCommand[commandLen-1] == '\n') {
          currentCommand[commandLen-1] = '\0';
        }
     
        char *delim = " ";
        int argCount = 0;
        char *returnString;
        strncpy(fullCommandUnparsed, currentCommand, RECEIVE_BUF_SIZE);
        returnString = strtok(currentCommand, delim);
        
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
      }

      if (typeOfCommand == invalidLength) {
        printf("Invalid Length.\n");
        fgets(currentCommand, RECEIVE_BUF_SIZE, inputFile);
        while(currentCommand[strlen(currentCommand) -1] != '\n') {
          fgets(currentCommand, RECEIVE_BUF_SIZE, inputFile);
        } 
      }
      else if (typeOfCommand == invalidCmdType) {
        printf("Not a valid command\n");
      }
      else if (isLoggedIn) {
        if (typeOfCommand == login) {
          printf("Someone is already logged in.\n");
          printf("Quit before trying to log in again.\n");
        }
        else {
          goodToSend = TRUE;

          if ((typeOfCommand == quit) && (!strncasecmp(commandArguments[1], "EOF", RECEIVE_BUF_SIZE))) {
            continueReading = FALSE;
            closeLogFile = TRUE;
          }
          else if (typeOfCommand == quit) {
            isLoggedIn = FALSE;
          }
        }

      }
      else {
        if (typeOfCommand == login) {
	  if ((numArgsInCmd == 2) && (strlen(commandArguments[1]) <= 10)) {
            goodToSend = TRUE;
            isLoggedIn = TRUE;
  
            // create socket descriptor
            if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
              DieWithError("socket() failed");
            } 
            memset(&echoServerAddr, 0, sizeof(echoServerAddr)); /* zero out structure */
            echoServerAddr.sin_family = AF_INET;                /* internet address family */
            echoServerAddr.sin_addr.s_addr = inet_addr(servIP); /* server ip address */
            echoServerAddr.sin_port = htons(ECHO_PORT);         /* server port */
	    // connect to server
	    if (connect(sock, (struct sockaddr*) &echoServerAddr, sizeof(echoServerAddr)) < 0) {
              DieWithError("connect() failed");
            } 
	  }
	  else {
	    printf("Invalid login.\n");
	  }
        }
        else {
          printf("You must login before you can send any other commands.\n"); 
        }
      }


      if (goodToSend) {
        echoStringLen = strlen(fullCommandUnparsed);
        strncpy(echoBuffer, "No newlines",12);
          
        if (send(sock, fullCommandUnparsed, echoStringLen, 0) != echoStringLen){
          DieWithError("send sent a different number of bytes than expected");
        } 

        if (typeOfCommand == list) {
          int recvdDone = FALSE;
          while(!recvdDone) {
            totalBytesRcvd = 0;
            strcpy(echoBuffer, "no newlines");
	    while (strstr(echoBuffer, "\n") == NULL) {
  	      if ((bytesRcvd = recv(sock, echoBuffer, RECV_SIZE-1, 0)) <= 0) {
                DieWithError("recv() failed or connection closed prematurely");
              }
              totalBytesRcvd += bytesRcvd;
              echoBuffer[bytesRcvd] = '\0';
            }
            
            if (strstr(echoBuffer, "DONE") != NULL) {
              recvdDone = TRUE;
              if (strcmp(echoBuffer, "DONE\n")) {
                echoBuffer[strlen(echoBuffer) - strlen("DONE\n")] = '\0';
                printf("%s", echoBuffer);

                fputs(echoBuffer, logFile);
              }
            } 
            else {
              printf("%s", echoBuffer);
              fputs(echoBuffer, logFile);
            }
          }
        }
        else {
	  totalBytesRcvd = 0;
	  while (strstr(echoBuffer, "\n") == NULL) {
	    if ((bytesRcvd = recv(sock, echoBuffer, RECV_SIZE-1, 0)) <= 0) {
              DieWithError("recv() failed or connection closed prematurely");
            }
            totalBytesRcvd += bytesRcvd;
            echoBuffer[bytesRcvd] = '\0';
            printf("%s", echoBuffer);
          }
          fputs(echoBuffer, logFile);
        }
        
      }
      if (closeLogFile) {
        fclose(logFile);
        close(sock);
	if (textFromFile) {
          fclose(inputFile);
        }
      }
      else if (typeOfCommand == quit) {
        close(sock); 
      }
    }
  }

  exit(0);

}


void DieWithError(char *errorString) {
  fprintf(stderr, "error: %s\n", errorString);
  exit(1);
}

