#include "multilple_client_server.h"
#include "socket.h"
#include <sys/poll.h>
#include <pthread.h>

char buff[1024];
/*These structs are used to control multiple clients*/

struct pollfd read_fdps[MAXIMUM_CLIENT];
struct pollfd connect_fdp;
int ret, state;
int new_fd;

int num_of_thread = 0;

int main()
{

    /*create an TCP server using IPv4 address and defined maximum client*/
    if (create_TCP_IPv4_server(&server, PORT, MAXIMUM_CLIENT, &server_fd, &len) == ERROR)
    {
        return -1;
    }

    printf("Ready to connect \n");

    /*Assign POLLIN event for server fd*/
    /*This means when server receive a connect request from client, poll() will return >0 result*/
    connect_fdp.fd = server_fd;
    connect_fdp.events = POLLIN;

    for (int i = 0; i < MAXIMUM_CLIENT; i++)
    {
        read_fdps[i].events = POLLIN;
    }

    while (1)
    {
        /*Reset the buff and message to store the file name*/
        for (int i = 0; i < 1024; i++)
        {
            buff[i] = 0;
        }
        /*Wait 0.1 seconds for new connection*/
        ret = poll(&connect_fdp, 1, 100);

        /*If there are new connection, accept the client's connection request*/
        /*create new thread for the client*/
        if (ret > 0)
        {

            if (accept_client_connection(&server, &server_fd, &new_fd, &len) == ERROR)
            {
                printf("Connection to this client has failed\n");
                return -1;
            }
        }
    }

    printf("Closing the server\n");
    /*Close the server socket*/
    close(server_fd);
    return 0;
}