#include "multilple_client_server.h"
#include "socket.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#define OBJECT_NAME "/shared_zone"
#define SIZE 200

char buff[1024];
char message[1024];
int ret, state;
struct pollfd connect_fd;
struct pollfd read_fdp;
pid_t client_process_id, childPID;
int val_read;
int shared_mem_fd;
int message_handling = 0;
char *ptr;

int main()
{

    /*create an TCP server using IPv4 address and defined maximum client*/
    shm_unlink(OBJECT_NAME);
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

    ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_mem_fd, 0);

    if (create_TCP_IPv4_server(&server, PORT, MAXIMUM_CLIENT, &server_fd, &len) == ERROR)
    {
        return -1;
    }

    connect_fd.fd = server_fd;
    connect_fd.events = POLLIN;
    printf("Ready to connect \n");
    int process_num = 0;

    while (1)
    {
        ret = poll(&connect_fd, 1, 100);
        if (ret > 0)
        {
            if (accept_client_connection(&server, &server_fd, &client_fd, &len) == ERROR)
            {
                printf("Connection to this client has failed\n");
                return -1;
            }
            process_num++;
            client_process_id = fork();
            if (client_process_id == 0)
            {
                break;
            }
            else if (client_process_id < 0)
            {
                close(server_fd);
                exit(EXIT_FAILURE);
            }
        }

        if (strcmp(ptr, "Close") == 0)
        {
            break;
        }
    }

    if (client_process_id == 0)
    {
        printf("Client is using process with %d \n", getpid());
        read_fdp.fd = client_fd;
        read_fdp.events = POLLIN;
    }
    while (client_process_id == 0)
    {
        if (strcmp(ptr, "Close") == 0)
        {
            close(client_fd);
            exit(EXIT_SUCCESS);
        }

        ret = poll(&read_fdp, 1, 10);
        if (ret == 0)
        {
            continue;
        }
        else if (ret < 0)
        {
            close(client_fd);
            exit(EXIT_FAILURE);
        }
        message_handling = handling_message_for_multiple_clients(&client_fd, message);
        if (message_handling == FIND_FILE)
        {
            printf("Finding file\n");
            find_file_following_client_request(message, buff, &client_fd);
        }
        else if (message_handling == CLOSE_MESSAGE)
        {
            close(client_fd);
            printf("Close server\n");
            sprintf(ptr, "Close");
            exit(EXIT_SUCCESS);
        }
        else if (message_handling == DISCONNECTED)
        {
            exit(EXIT_SUCCESS);
        }
    }

    for (size_t i = 0; i < process_num; i++)
    {
        childPID = wait(NULL);
        if (childPID != -1)
        {
            printf("Close client in PID %d\n", childPID);
        }
        else
        {
            printf("No child need to be closed\n");
        }
    }

    shm_unlink(OBJECT_NAME);
    return 0;
}