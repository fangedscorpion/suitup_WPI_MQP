	#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <bluetooth/rfcomm.h>

#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define SEARCH_TIME 8

volatile int printing = TRUE;

void bluetoothDetermineDevices();
void parseArgs(int, char **);
int connectToStringAddr(char*);
int connectToBAddr(struct sockaddr_rc*);
int closeSocket(int);


// password for nonLE BT modules is 1234

int main(int argc, char **argv) {

	parseArgs(argc, argv);
	int keepScanning = TRUE;
	int buffLen = 5;
	char line[buffLen];
	char *fgetsResult;

	int goodInput = FALSE;


	while (keepScanning) {
		bluetoothDetermineDevices();
		printf("Enter y to continue scanning or n to quit\n");
		goodInput = FALSE;
		while (!goodInput) {
			fgetsResult = fgets(line, buffLen, stdin);
			if (strncmp(line, "n\n", buffLen) == 0) {
				goodInput = TRUE;
				keepScanning = FALSE;
			}
			else if (strncmp(line, "y\n", buffLen) == 0) {
				goodInput = TRUE;
			}
		}
	}

	// pair with 20:15:05:12:51:35
	int sockNum;
	sockNum = connectToStringAddr("20:15:05:12:51:35");

	sleep(5);

	closeSocket(sockNum);	


}


void bluetoothDetermineDevices() {
	// Get local bluetooth device ------------------------------
	int dev_id = hci_get_route(NULL);
	struct hci_dev_info device_info;
	
	// get info about local bluetooth device --------------------
	if (hci_devinfo(dev_id, &device_info) == 0) {
		char device_address[18];
		ba2str(&device_info.bdaddr, device_address);
		if (printing) {
			printf("Local Device: %s %s\n", device_address, device_info.name);
		}
	}

	// Scan for devices -------------------------------------
	inquiry_info *bluetoothDevicesFound = NULL;
	int numDevicesFound;

	int max_rsp = 255;
	bluetoothDevicesFound = (inquiry_info*) malloc(max_rsp*sizeof(inquiry_info));
	long inquiryFlags;
	inquiryFlags = IREQ_CACHE_FLUSH; //clears old devices, choose 0 to leave them

	if (printing) {
		printf("Scanning for bluetooth devices\n");
	}

	numDevicesFound = hci_inquiry(dev_id, SEARCH_TIME, max_rsp, NULL, &bluetoothDevicesFound, inquiryFlags);
	if (numDevicesFound < 0) {
		printf("Error: %d, %s\n", errno, strerror(errno));
	} 
	if (printing) {
		printf("Found %d devices\n", numDevicesFound);
	}

	// Getting device information -------------------------------
	int i;

	int sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    } 

    char addr[19] = { 0 };
    char name[248] = { 0 };

	for (i = 0; i < numDevicesFound; i++) {
        ba2str(&(bluetoothDevicesFound+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(bluetoothDevicesFound+i)->bdaddr, sizeof(name), 
            name, 0) < 0) {
        	strcpy(name, "[unknown]");
    	}
    	if (printing) {
        	printf("%s  %s\n", addr, name);
        }
    }

    // Clean up -------------------------
    free(bluetoothDevicesFound);
    close(sock);
}


void parseArgs(int argc, char **argv) {
	if (argc > 1) {
		if (strcmp(argv[1], "-np") == 0) {
			printing = FALSE;
		}
	}
}

/*
Returns socket with connection
*/
int connectToStringAddr(char* dest) {
	struct sockaddr_rc *addr;
	addr = (struct sockaddr_rc*) malloc(sizeof(struct sockaddr_rc));
    
    // set the connection parameters (who to connect to)
    addr->rc_family = AF_BLUETOOTH;
    addr->rc_channel = (uint8_t) 1;
    str2ba(dest, &addr->rc_bdaddr );

    return connectToBAddr(addr);
}

int connectToBAddr(struct sockaddr_rc* addr) {

    char addrStr[19] = { 0 };

	ba2str(&(addr->rc_bdaddr), addrStr);
	if (printing) {
       	printf("Connecting to %s\n", addrStr);
    } 

    int s, status;

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (printing) {
    	printf("Socket: %d\n", s);
    }

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if (printing) {
    	printf("Connect returned %d\n", status);
    }
    if (status < 0) {
    	if (printing) {
    		printf("Could not connect\n");
    		printf("Error code: %d, %s\n", errno, strerror(errno));
    	}
    }
    return status;
}

int closeSocket(int socketToClose) {
	close(socketToClose);
}

int sendCharacterSequence(char* sequence, int datalen, int sock) {
	int status;
	status = send(sock, sequence, datalen, 0);
	if (printing && (status < 0)) {
		printf("Error sending character sequence %s", sequence);
	}
	return status;
}

/*
dataBuff should have memory allocated before passing to this function

returns number of characters read
*/
int recvCharacterSequence(char *dataBuff, int sock, int readInMaxBytes) {
	return recv(sock, dataBuff, readInMaxBytes, 0);
}

