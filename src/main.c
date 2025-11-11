#include "multilple_client_server.h"
#include "socket.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#define OBJECT_NAME "/shared_zone"
#define SIZE 1024

char buff[1024];
char message[1024];
int ret, state;

int num_of_thread = 0;
pid_t client_process_id, childPID;
int val_read;
int shared_mem_fd;

int message_handling = 0;
#include <sys/wait.h>
int main()
{

    /*create an TCP server using IPv4 address and defined maximum client*/
    int i = 0;
    do
    {
        i++;
        shared_mem_fd = shm_open(OBJECT_NAME, O_CREAT | O_RDWR, 0644);
        if (shared_mem_fd < 0 && i < 6)
        {
            shm_unlink(OBJECT_NAME);
        }
        else if (i > 6)
        {
            perror("shm_open");
            exit(EXIT_FAILURE);
        }
    } while (shared_mem_fd < 0);

    if (ftruncate(shared_mem_fd, SIZE) == -1)
    {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    if (create_TCP_IPv4_server(&server, PORT, MAXIMUM_CLIENT, &server_fd, &len) == ERROR)
    {
        return -1;
    }

    printf("Ready to connect \n");

    while (1)
    {

        if (accept_client_connection(&server, &server_fd, &client_fd, &len) == ERROR)
        {
            printf("Connection to this client has failed\n");
            return -1;
        }
        client_process_id = fork();

        if (client_process_id == 0)
        {
            while (childPID == 0)
            {

                val_read = read(client_fd, buff, 1024);
                if (val_read > 0)
                {
                    message_handling = handling_message_for_multiple_clients(&client_fd, buff);
                    if (message_handling == FIND_FILE)
                    {
                        find_file_following_client_request(message, buff, &client_fd);
                    }
                    else if (message_handling == CLOSE_MESSAGE)
                    {
                        close(client_fd);
                        exit(EXIT_SUCCESS);
                    }
                    else if (message_handling == DISCONNECTED)
                    {
                        exit(EXIT_SUCCESS);
                    }
                }
            }
        }
    }

    childPID = wait(NULL);
    if (childPID != -1)
    {
        printf("Close client in PID %d\n", childPID);
    }
    else
    {
        printf("No child need to be closed\n");
    }
    shm_unlink(OBJECT_NAME);
    return 0;
}