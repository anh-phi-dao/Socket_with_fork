#include "multilple_client_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <mqueue.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MANAGE_SERVER_QUEUE_NAME "/server_queue"
#define FIND_FILE_MESSAGE "FIND_FILE"
#define CLOSE_FILE_MESSAGE "CLOSE_FILE"
#define CLIENT_DISCONNECTED_MESSAGE "Client has disconnected"
#define MAX_MESSAGES MAXIMUM_CLIENT
#define MAX_MSG_SIZE 1024

extern mqd_t server_mq;
extern struct mq_attr attr;
extern pthread_mutex_t server_mtx;
extern char status_message[1024];

enum InitQueueState
{
    MQ_OPEN_ERROR = -1,
    MQ_OPEN_SUCCESS
};

int init_server_message_queue();
void *handle_message_thread(void *arg);