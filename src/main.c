#include "manage_server_with_thread.h"
#include "socket.h"
#include <sys/poll.h>
#include <pthread.h>

char buff[1024];
/*These structs are used to control multiple clients*/
pthread_t manage_client_threads[MAXIMUM_CLIENT];
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
        if (ret > 0)
        {

            if (accept_client_connection(&server, &server_fd, &new_fd, &len) == ERROR)
            {
                printf("Connection to this client has failed\n");
                return -1;
            }
            for (int i = 0; i < MAXIMUM_CLIENT; i++)
            {
                if (client_fd[i] == 0)
                {
                    client_fd[i] = new_fd;
                    read_fdps[i].fd = new_fd;
                    if (pthread_create(&manage_client_threads[i], NULL, handle_message_thread, read_fdps + i) > 0)
                    {
                        perror("pthread_create");
                        printf("Error on pthread\n");
                        close(server_fd);
                        mq_close(server_mq);
                        mq_unlink(MANAGE_SERVER_QUEUE_NAME);
                        return -1;
                    }
                    num_of_thread++;
                    break;
                }
            }
        }

        if (num_of_thread > 0)
        {
            printf("Ready to handle something\n");
            state = pthread_mutex_lock(&server_mtx);
            if (state != 0)
            {
                printf("Error locking mutex\n");
            }

            printf("%s\n", status_message);
            if (strcmp(status_message, CLOSE_FILE_MESSAGE) == 0)
            {
                printf("Closing all file\n");
                for (int i = 0; i < MAXIMUM_CLIENT; i++)
                {
                    if (client_fd[i] != 0)
                    {
                        writen(client_fd[i], "Closing connect from server\n", 29);
                        pthread_detach(manage_client_threads[i]);
                        close(client_fd[i]);
                        client_fd[i] = 0;
                    }
                }
                num_of_thread = 0;
                break;
            }
            else if (strcmp(buff, CLIENT_DISCONNECTED_MESSAGE) == 0)
            {
                printf("Disconnect now\n");
                for (int i = 0; i < MAXIMUM_CLIENT; i++)
                {
                    if (client_fd[i] != read_fdps[i].fd)
                    {
                        pthread_detach(manage_client_threads[i]);
                        close(client_fd[i]);
                        client_fd[i] = 0;
                        num_of_thread--;
                    }
                }
            }
            state = pthread_mutex_unlock(&server_mtx);
            if (state != 0)
            {
                printf("Error locking mutex\n");
            }
        }
    }

    printf("Closing the server\n");
    /*Close the server socket*/
    close(server_fd);
    mq_close(server_mq);
    mq_unlink(MANAGE_SERVER_QUEUE_NAME);

    return 0;
}