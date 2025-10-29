#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

char *sock_ntop(const struct sockaddr *sa, socklen_t salen);
/**
 * @author : Unix network programing volume 1 by W. Richard Stevens, Bill Fenner, Andrew M. Rudoff
 * @brief : Write "n" bytes to a descriptor.
 * @param fd: file descriptor of the socket
 * @param vptr: buffer address
 * @param n: number of bytes
 * @return number of byte has been written to
 */
ssize_t writen(int fd, void *vptr, size_t n);
/**
 * @author : Unix network programing volume 1 by W. Richard Stevens, Bill Fenner, Andrew M. Rudoff
 * @brief : Read "n" bytes from a descriptor.
 * @param fd: file descriptor of the socket
 * @param vptr: buffer address
 * @param n: number of bytes
 * @return number of bytes have been read from
 */
ssize_t readn(int fd, void *vptr, size_t n);
/**
 * @author : Unix network programing volume 1 by W. Richard Stevens, Bill Fenner, Andrew M. Rudoff
 * @brief : Read until you meet a '\n character and the number of characters only reaches maxlen
 * @param fd: file descriptor of the socket
 * @param vptr: buffer address
 * @param maxlen: maximum characters can be read
 * @return number of bytes have been read from socket
 */
ssize_t readline(int fd, void *vptr, size_t maxlen);
