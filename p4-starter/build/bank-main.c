#include "bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Default port and ip address are defined here

int main(int argc, char** argv){
	
	unsigned short port = 3000;
	char *ip = "127.0.0.1";


	/* no error checking is done on any of this. may need to modify this */
	Bank *b = bank_create("an auth file", ip, port);

	/* process each incoming client, one at a time, until complete */
	for(;;) {

		unsigned int len = sizeof(b->remote_addr);
		b->clientfd = accept(b->sockfd, (struct sockaddr*)&b->remote_addr, &len);
		if (b->clientfd < 0) {
			perror("error on accept call");
			exit(255);
		}

		/* okay, connected to bank/atm. Send/recv messages to/from the bank/atm. */
		char buffer[1024];
		bank_recv(b, buffer, sizeof(buffer));
		printf("bank received:  %s\n", buffer);
		strcpy(buffer, "money money money");
		bank_send(b, buffer, strlen(buffer)+1);


		/* when finished processing commands ...*/
		close(b->clientfd);
		b->clientfd = -1;
	}


	
	
	// Implement how atm protocol will work: sanitizing inputs and using different modes of operations

	return EXIT_SUCCESS;
}
