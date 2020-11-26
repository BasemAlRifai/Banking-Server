#ifndef NET_H
#define NET_H

#include <stddef.h>
#include <sys/types.h>

/* used by atm.c and bank.c to just wrap some network
 * functions. Looking at this is probably unnecessary. */


/* send len bytes of data to fd, return number of bytes sent, or a
 * negative value on error */
ssize_t send_all(int fd, const char *data, size_t len);

/* read len bytes into data from fd, return number of bytes sent or a
 * negative value on error */
ssize_t recv_all(int fd, char *data, size_t len);

#endif
