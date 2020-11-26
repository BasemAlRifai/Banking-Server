#include "atm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Default port and ip address are defined here

int main(int argc, char** argv){
  	unsigned short port = 3000;
	char *ipAddr = "127.0.0.1";


	ATM *atm = atm_create(ipAddr, port);

	/* send messages */

	char buffer[] = "Hello I am the atm and I would like to have money please";
	atm_send(atm, buffer, sizeof(buffer));
	atm_recv(atm, buffer, sizeof(buffer));
	printf("atm received %s\n", buffer);
	
	atm_free(atm);
	

	// Implement how atm protocol will work: sanitizing inputs and using different modes of operations

	return EXIT_SUCCESS;
}
