/*
 * The Bank takes commands from the ATM, handled by
 * bank_process_remote_command.
 *
 * You can add more functions as needed.
 */

#ifndef __BANK_H__
#define __BANK_H__

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h>


typedef struct _Bank {
	// Networking state
	int sockfd;
	int clientfd;
	struct sockaddr_in bank_addr;
	struct sockaddr_in remote_addr;
	char *auth_file; 
    
	// Protocol state
	// Specify struct for storing state of bank and acount holders

} Bank;

Bank* bank_create(char *auth_file, char *ip, unsigned short port);
void bank_free(Bank *bank);
int bank_send(Bank *bank, const char *data, size_t data_len);
ssize_t bank_recv(Bank *bank, char *data, size_t max_data_len);
void bank_process_remote_command(Bank *bank, char *command, size_t len);
// Add more as required

#endif
