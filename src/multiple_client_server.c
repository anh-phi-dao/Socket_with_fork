#include "multilple_client_server.h"
#include "socket.h"

struct sockaddr_in server;

socklen_t len = (socklen_t)sizeof(struct sockaddr_in);

int server_fd;
int client_fd;

char IP[INET_ADDRSTRLEN];
uint16_t port_number;

int create_TCP_IPv4_server(struct sockaddr_in *server_addr, uint16_t port, int num_of_client, int *server_fd, socklen_t *len)
{
    *server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (*server_fd < 0)
    {
        *server_fd = 0;
        printf("Failed on socket function\n");
        return ERROR;
    }

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    server_addr->sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        printf("Failed on setsockopt function\n");
        return ERROR;
    }
    if (bind(*server_fd, (struct sockaddr *)server_addr, *len) < 0)
    {
        printf("Failed on binding a server\n");
        return ERROR;
    }

    if (listen(*server_fd, num_of_client) < 0)
    {
        printf("Failed on establishing a server\n");
        return ERROR;
    }
    printf("Successfully created TCP server\n");
    return SUCCESS;
}

int accept_client_connection(struct sockaddr_in *server_addr, int *server_fd, int *client_fd, socklen_t *len)
{
    struct sockaddr_in client_identity;
    printf("Wating for client connection\n");
    *client_fd = accept(*server_fd, (struct sockaddr *)server_addr, len);
    if ((*client_fd) < 0)
    {
        printf("Server has failed to accept client connection\n");
        return ERROR;
    }

    if (getpeername(*client_fd, (struct sockaddr *)&client_identity, len) < 0)
    {
        printf("Can not get client infomation");
    }
    else
    {
        if (inet_ntop(AF_INET, &client_identity.sin_addr, IP, *len) == NULL)
        {
            printf("Can not get client information\n");
            goto out;
        }
        else
        {
            printf("\nNew client\nIPv4 address:%s\n", IP);
        }
    }

out:
    printf("Successfully connected to TCP client\n\n");
    return 0;
}

int get_client_information(int *client_fd, socklen_t *len)
{
    struct sockaddr_in client_identity;
    if (getpeername(*client_fd, (struct sockaddr *)&client_identity, len) < 0)
    {
        printf("Can not get client infomation");
    }
    else
    {
        if (inet_ntop(AF_INET, &client_identity.sin_addr, IP, *len) == NULL)
        {
            printf("Can not get client information\n");
            return ERROR;
        }
        else
        {
        }
    }
    printf("From client with IPv4 address:%s\n", IP);
    return SUCCESS;
}

int handling_message_for_multiple_clients(int *client_fd, char *message_file_name)
{
    int handling_message = 0;

    int val_read = read(*client_fd, message_file_name, 100);

    if (val_read == 0)
    {
        close(*client_fd);
        *client_fd = 0;
        printf("Client disconnected\n\n");
        handling_message = DISCONNECTED;
    }
    else
    {
        if (strcmp(message_file_name, "") == 0)
        {
            return DO_NOTHING;
        }
        handling_message = FIND_FILE;
    }
    if (strcmp(message_file_name, "Close") == 0)
    {
        handling_message = CLOSE_MESSAGE;
    }

    return handling_message;
}

int find_file_following_client_request(char *file_name, char *buff, int *client_fd)
{

    FILE *fileptr = fopen(file_name, "rb");
    if (fileptr == NULL)
    {
        get_client_information(client_fd, &len);
        printf("Can not find the requested file\n");
        writen(*client_fd, "Can not find the requested file\n", 33);
        return FILE_NOT_FOUND;
    }
    fscanf(fileptr, "%1024[^\n]s", buff);
    writen(*client_fd, buff, strlen(buff));
    writen(*client_fd, "\n", 1);
    return FILE_FOUND;
}