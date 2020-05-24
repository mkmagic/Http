#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "parseHttpProtocol.h"

#define DEFUALT_PORT 8080
#define MAX_BACKLOG 10  

// Defines a type for a pointer to a handler function
typedef void* handler_function(void*);  

int start_server(handler_function func);

int start_server(handler_function func) {
    int server_fd, client_socket; 
    struct sockaddr_in address;
    int addrlen = sizeof(address);
        
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error creating socket");
        return EXIT_FAILURE;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( DEFUALT_PORT );
    // Padd the structure so it can be typecast to sockaddr
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("Error binding");
        return EXIT_FAILURE;
    }
    if (listen(server_fd, MAX_BACKLOG) < 0)
    {
        perror("Error listening");
        return EXIT_FAILURE;
    }
    while(true)
    {
        printf("\n---- Waiting for new connection -----\n\n");
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("Erro accepting");
            return EXIT_FAILURE;
        }
        // Create a new thread for the client
        pthread_t new_thread;
        int *pclient = malloc(sizeof(int));
        *pclient = client_socket;
        pthread_create(&new_thread, NULL, func, pclient);

    }
    return EXIT_SUCCESS;
}


int main(int argc, char const *argv[])
{
    int iResult = start_server(handle_http_request);
    return iResult;
}
