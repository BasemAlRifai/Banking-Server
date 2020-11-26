#include "atm.h"
#include "net.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>



ATM* atm_create(char *ip, unsigned short port)
{
	ATM *atm = (ATM*) calloc(1, sizeof(ATM));
	if(atm == NULL) {
		perror("Could not allocate ATM");
		exit(1);
	}

#define BOOL_CHK(x,msg) if (x) { perror(msg); exit(255); }

	// Set up the network state

	BOOL_CHK(inet_pton(AF_INET, ip, &(atm->bank_addr.sin_addr)) != 1, "could not convert ip address");
	atm->bank_addr.sin_port = htons(port);
	atm->bank_addr.sin_family = AF_INET;

	atm->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	BOOL_CHK(atm->sockfd < 0, "could not create socket");

	BOOL_CHK(connect(atm->sockfd, (struct sockaddr*)&(atm->bank_addr), sizeof(atm->bank_addr)) < 0, "could not connect");

#undef BOOL_CHK
	

	// Set up the protocol state
	// TODO set up more, as needed

	return atm;
}

void atm_free(ATM *atm){
	close(atm->sockfd);
// TODO
}

/* send a message to the bank of size data_len stored in the buffer
 * data. return 0 on success, negative on error */
int atm_send(ATM *atm, const char *data, size_t data_len){

	if (atm->sockfd < 0) {
		return -1;
	}

	if (send_all(atm->sockfd, (const char*)&data_len, sizeof(data_len)) != sizeof(data_len)) {
		return -2;
	}

	if (send_all(atm->sockfd, data, data_len) != (ssize_t)data_len) {
		return -3;
	}
	return 0;
}

/* receive a message (i.e., something sent via bank_send) and store it
 * in data. If the message exceeds max_data_len, a negative value is
 * returned and the message is discarded */
ssize_t atm_recv(ATM *atm, char *data, size_t max_data_len){

	size_t msg_len;

	if (atm->sockfd < 0) {
		return -1;
	}

	if (recv_all(atm->sockfd, (char*)&msg_len, sizeof(msg_len)) != sizeof(msg_len)) {
		return -2;
	}

	if (msg_len > max_data_len) {
		/* message doesn't fit in data, read all of the message to discard it */
		char tmp[4096];
		do {
			size_t to_read = msg_len > sizeof(tmp) ? sizeof(tmp) : msg_len;
			if (recv_all(atm->sockfd, tmp, to_read) != sizeof(to_read)) {
				/* logic error somewhere, should probably crash/restart */
				return -3;
			}
			msg_len -= to_read;
		} while(msg_len > 0) ;
	}

	return recv_all(atm->sockfd, data, msg_len);
}

void atm_process_command(ATM *atm, char *command)
{
	// Implement the ATM's side of the ATM-bank protocol
}

// Add more functions as needed
