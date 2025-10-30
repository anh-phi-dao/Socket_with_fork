#include "manage_server_with_thread.h"

pthread_mutex_t server_mtx = PTHREAD_MUTEX_INITIALIZER;
char status_message[1024];
pthread_t manage_client_threads[MAXIMUM_CLIENT];

void *handle_message_thread(void *arg)
{
    struct pollfd *read_fdp = (struct pollfd *)arg;
    int ret = 0;
    int s;
    char message_name[200];
    char buff[1024];
    int message_handling;
    while (1)
    {
        ret = poll(read_fdp, 1, 1000);
        if (ret > 0)
        {
            printf("Ready handling\n");
            message_handling = handling_message_for_multiple_clients(read_fdp, &read_fdp->fd, message_name);
            if (message_handling == FIND_FILE)
            {
                printf("Finding file\n");
                find_file_following_client_request(message_name, buff, &read_fdp->fd);
                s = pthread_mutex_lock(&server_mtx);
                if (s != 0)
                {
                    printf("Error locking mutex\n");
                }
                sprintf(status_message, "%s", FIND_FILE_MESSAGE);
                s = pthread_mutex_unlock(&server_mtx);
                if (s != 0)
                {
                    printf("Error locking mutex\n");
                }
            }
            else if (message_handling == CLOSE_MESSAGE)
            {
                printf("Close all file\n");
                s = pthread_mutex_lock(&server_mtx);
                if (s != 0)
                {
                    printf("Error locking mutex\n");
                }
                sprintf(status_message, "%s", CLOSE_FILE_MESSAGE);
                s = pthread_mutex_unlock(&server_mtx);
                if (s != 0)
                {
                    printf("Error locking mutex\n");
                }
            }
            else if (message_handling == DISCONNECTED)
            {
                printf("Dealing with disconnected client\n");
                read_fdp->fd = 0;
                s = pthread_mutex_lock(&server_mtx);
                if (s != 0)
                {
                    printf("Error locking mutex\n");
                }
                sprintf(status_message, "%s", CLIENT_DISCONNECTED_MESSAGE);
                s = pthread_mutex_unlock(&server_mtx);
                if (s != 0)
                {
                    printf("Error locking mutex\n");
                }
            }
        }
        ret = 0;
    }

    return NULL;
}