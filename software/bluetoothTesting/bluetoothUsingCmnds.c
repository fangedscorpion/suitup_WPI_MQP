#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <bluetooth/rfcomm.h>

#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char* argv) {
	int pid; // process id
	int totalTime; // holds elapsed time

	int i = system("hcitool scan");
}
