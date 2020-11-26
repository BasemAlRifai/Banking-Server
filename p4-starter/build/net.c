#include "net.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>


/* send len bytes of data to fd, return number of bytes sent, or a negative value on error */
ssize_t send_all(int fd, const char *data, size_t len) {
	size_t remaining = len;
	do {
		ssize_t t = send(fd, data + (len - remaining), remaining, 0);
		assert(t != 0);
		if (t < 0) {
			return t;
		} else {
			remaining -= t;
		}
	} while(remaining > 0);

	return len;
}

/* read len bytes into data from fd, return number of bytes sent or a negative value on error */
ssize_t recv_all(int fd, char *data, size_t len) {

	size_t remaining = len;
	do {
		ssize_t t = recv(fd, data + (len - remaining), remaining, 0);
		assert(t != 0);
		if (t < 0) {
			return t;
		} else {
			remaining -= t;
		}
	} while(remaining > 0);

	return len;
		
}

