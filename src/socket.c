#include "socket.h"

char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
{

    char str[128]; /* Unix domain is largest */
    static char result[170];

    /*Unix Networking Programing Volume 1*/
    switch (sa->sa_family)
    {
    case AF_INET:
    {
        struct sockaddr_in *sin = (struct sockaddr_in *)sa;
        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
        {
            return NULL;
        }
        uint16_t port = ntohs(sin->sin_port);
        sprintf(result, "Port:%u IPv4 Address:%s", port, str);
        return result;
    }
    /*Self define based on the book*/
    case AF_INET6:
    {
        struct sockaddr_in6 *sin = (struct sockaddr_in6 *)sa;
        if (inet_ntop(AF_INET6, &sin->sin6_addr, str, sizeof(str)) == NULL)
        {
            return NULL;
        }
        uint16_t port = ntohs(sin->sin6_port);
        sprintf(result, "Port:%u IPv6 Address:%s", port, str);
        return result;
    }

    default:
        return NULL;
    }
}

ssize_t writen(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    char *ptr;

    ptr = vptr;
    nleft = n;

    /*If there is no leftover character, escape the loop*/
    while (nleft > 0)
    {
        /*write nleft byte to socket*/
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            /*if there is an interrupt system call, rewrite*/
            if (nwritten < 0 && errno == EINTR)
            {
                nwritten = 0;
            }
            /*if it failed because something else, stop writting*/
            else
            {
                return -1;
            }
        }
        /*calculate the leftover characters*/
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

ssize_t readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = vptr;
    nleft = n;
    /*if the function dose not read enough characters, continue*/
    while (nleft > 0)
    {
        /*Attemp to read by using read system call*/
        if ((nread = read(fd, ptr, nleft)) < 0)
        {
            /*if there is an interrupt system call, try again*/
            if (errno == EINTR)
            {
                nread = 0;
            }
            /*when the function is truly error,break out of the function*/
            else
            {
                return -1;
            }
        }
        /*if the function has read enough characters, break out of the loop*/
        else if (nread == 0)
        {
            break;
        }

        /*calculate the leftover characters*/
        nleft -= nread;
        ptr += nread;
    }

    return (n - nleft);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char c, *ptr;

    ptr = vptr;
    /*Continue reading until we reach maxlen*/
    for (n = 1; n < maxlen; n++)
    {
    again:
        /*read one character*/
        rc = read(fd, &c, 1);
        if (rc == 1)
        {
            *ptr++ = c;
            if (c == '\n')
                break;
        }
        /*if we reach end of file, return*/
        else if (rc == 0)
        {
            *ptr = 0;
            return (n - 1);
        }
        else
        {
            /*if there is an interrupt system call, try again*/
            if (errno == EINTR)
            {
                goto again;
            }
            /*when the function has an error, return*/
            return -1;
        }
    }
    /*create NULL character to end string and return*/
    *ptr = 0;
    return n;
}
