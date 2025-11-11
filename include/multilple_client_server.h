#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*libray for socket structure*/
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8000
#define MAXIMUM_CLIENT 10

extern struct sockaddr_in server;
extern socklen_t len;
extern int server_fd;
extern int client_fd;
extern char IP[INET_ADDRSTRLEN];
extern uint16_t port_number;
/**
 * @brief Create a TCP server, user can choose the PORT, number of clients that server can connect
 * @param server_addr: pointer to server address following IPv4, the function will create a server that can received
 * any client address and the information will be stored in server address. Users can use predefined struct sockaddr_in server
 * @param port: port number, this is a parameter belongs to transport layer of OSI model. User can use predefined PORT =8000
 * @param num_of_client: maximum number that client can receive. Users can use predefined MAXIMUM_CLIENT = 10
 * @param fd: A pointer that points to the file descriptor of the server socket. User can use predefined int server_fd
 * @param len: a pointer to socken_t which store the length of the address (IPv4 is different, IPv6 is different)
 * . Users can use predefined socklen_t len;
 * @return SUCCESS=0, ERROR=-1
 */
int create_TCP_IPv4_server(struct sockaddr_in *server_addr, uint16_t port, int num_of_client, int *fd, socklen_t *len);
/**
 * @brief Accept connection from TCP client
 * @param server_addr: pointer to server address following IPv4. This pointer must point to the server address has been dealed
 * by create_TCP_IPv4_server() function.
 * @param server_fd: A pointer that points to the file descriptor of the server socket. User can use predefined int server_fd
 * @param client_fd: A pointer that points to the file descriptor of the connected client socket. The function will
 * generate a new client file descriptor which this pointer is pointing to. User must use this file descriptor to communicate with
 * the client socket.
 * @param len: a pointer to socken_t which store the length of the address (IPv4 is different, IPv6 is different)
 * . Users can use predefined socklen_t len;
 * @return SUCCESS=0, ERROR=-1
 */
int accept_client_connection(struct sockaddr_in *server_addr, int *server_fd, int *client_fd, socklen_t *len);
/**
 * @brief Print information about a client socket coresponding to a particular file descriptor
 * @param client_fd: A pointer that points to the file descriptor of the connected client socket. This file descriptor must
 * be generated from accept_client_connection()
 * @param len: a pointer to socken_t which store the length of the address (IPv4 is different, IPv6 is different)
 * . Users can use predefined socklen_t len;
 * @return SUCCESS=0, ERROR=-1
 */
int get_client_information(int *client_fd, socklen_t *len);
/**
 * @brief This function is used to handle client's file request or file request of closing server
 * @return FIND_FILE=0 , CLOSE_MESSAGE=1
 */
int handling_message_for_multiple_clients(int *client_fd, char *message_file_name);
/**
 * @brief This function is used to find the file following client request
 * @return FILE_FOUND=1, FILE_NOT_FOUND=0
 */
int find_file_following_client_request(char *file_name, char *buff, int *client_fd);

enum
{
    ERROR = -1,
    SUCCESS
};

enum MessHandlingState
{
    DISCONNECTED,
    FIND_FILE,
    DO_NOTHING,
    CLOSE_MESSAGE,
};

enum FileState
{
    FILE_NOT_FOUND,
    FILE_FOUND
};