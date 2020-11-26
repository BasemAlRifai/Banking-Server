/*
 * The ATM interfaces with the user.  User commands should be
 * handled by atm_process_command.
 *
 * The ATM can read .card files, but not .pin files.
 *
 * You can add more functions as needed.
 */

#ifndef __ATM_H__
#define __ATM_H__

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

typedef struct _ATM {
	// Networking state
	int sockfd;
	struct sockaddr_in bank_addr;

	// Protocol state
	// TODO add more, as needed
} ATM;

ATM* atm_create(char *ip, unsigned short port);
void atm_free(ATM *atm);
int atm_send(ATM *atm, const char *data, size_t data_len);
ssize_t atm_recv(ATM *atm, char *data, size_t max_data_len);
void atm_process_command(ATM *atm, char *command);

#endif
