#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0
#define SEARCH_TIME 20

volatile int printing = TRUE;

void bluetoothDetermineDevices();
void parseArgs(int, char **);

int main(int argc, char **argv) {

	parseArgs(argc, argv);

	bluetoothDetermineDevices();
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
            name, 0) < 0)
        strcpy(name, "[unknown]");
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
