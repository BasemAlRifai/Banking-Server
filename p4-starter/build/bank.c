#include "bank.h"
#include "net.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

Bank* bank_create(char *auth_file, char *ip, unsigned short port)
{

	Bank *bank = (Bank*) calloc(1, sizeof(Bank));
    
	// if not auth_file exists, create auth_file
	//bank->auth_file = auth_file
        
	if(bank == NULL) {
		perror("Could not allocate Bank");
		exit(255);
	}

#define BOOL_CHK(x,msg) if (x) { perror(msg); exit(255); }


	/* setup network connection */
	BOOL_CHK(inet_pton(AF_INET, ip, &(bank->bank_addr.sin_addr)) != 1, "could not convert ip address");

	bank->bank_addr.sin_port = htons(port);
	bank->bank_addr.sin_family = AF_INET;

	int s = socket(AF_INET, SOCK_STREAM, 0);
	BOOL_CHK(s<0,"could not create socket");

	int enable = 1;
	BOOL_CHK(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0, "setsockopt(SO_REUSEADDR) failed");

	BOOL_CHK(bind(s, (struct sockaddr*)&(bank->bank_addr), sizeof(bank->bank_addr)) < 0, "could not bind");

	listen(s, 5);

	bank->sockfd = s;

#undef BOOL_CHK



	// Set up the protocol state
	// TODO set up more, as needed

	return bank;
}

void bank_free(Bank *bank){

	close(bank->sockfd);
	// TODO
}

/* sends data_len bytes from data to atm, returns size 0 on success, negative on failure */
int bank_send(Bank *bank, const char *data, size_t data_len) {
	if (bank->clientfd < 0) {
		return -1;
	}

	if (send_all(bank->clientfd, (const char*)&data_len, sizeof(data_len)) != sizeof(data_len)) {
		return -2;
	}

	if (send_all(bank->clientfd, data, data_len) != (ssize_t)data_len) {
		return -3;
	}
	return 0;	
}

/* receive a message (i.e., something sent via atm_send) and store it
 * in data. If the message exceeds max_data_len, a negative value is
 * returned and the message is discarded */
ssize_t bank_recv(Bank *bank, char *data, size_t max_data_len) {

	size_t msg_len;

	if (bank->clientfd < 0) {
		return -1;
	}

	if (recv_all(bank->clientfd, (char*)&msg_len, sizeof(msg_len)) != sizeof(msg_len)) {
		return -2;
	}

	if (msg_len > max_data_len) {
		/* message doesn't fit in data, read all of the message to discard it */
		char tmp[4096];
		do {
			size_t to_read = msg_len > sizeof(tmp) ? sizeof(tmp) : msg_len;
			if (recv_all(bank->clientfd, tmp, to_read) != sizeof(to_read)) {
				/* logic error somewhere, should probably crash/restart */
				return -3;
			}
			msg_len -= to_read;
		} while(msg_len > 0) ;
	}

	return recv_all(bank->clientfd, data, msg_len);	

}

void bank_process_remote_command(Bank *bank, char *command, size_t len) {
	// TODO: Implement the bank side of the ATM-bank protocol
}

//Add more functions as required
