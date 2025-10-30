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

extern pthread_mutex_t server_mtx;
extern char status_message[1024];
extern pthread_t manage_client_threads[MAXIMUM_CLIENT];

/**
 * @brief Using pthread_create() with this function pointer as
 * an argutment for pthread_create() to create a thread that manage incoming client connections
 * @param arg: Passing struct pollfd * so the thread can monitor when the client is ready to read
 * @return NULL
 */
void *handle_message_thread(void *arg);