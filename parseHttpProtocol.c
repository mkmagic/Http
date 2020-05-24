#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>

#include "parseHttpProtocol.h"
#include "responseFunctions.h"

/** 
 * Limit the possible size and number of headers possible for a request.
 * The limits are the same ones used for the Apache 2.3 server.
 */
#define MAX_NUMBER_OF_HEADERS 100
#define MAX_HEADER_SIZE 8190
#define MAX_BODY_SIZE 100000
#define MAX_BUFFER_SIZE MAX_NUMBER_OF_HEADERS * MAX_HEADER_SIZE + MAX_BODY_SIZE
#define MAX_RESPONSE_MSG_SIZE 200
#define MAX_RESPONSE_BODY_SIZE 100000
#define MAX_RESPONSE_SIZE MAX_RESPONSE_MSG_SIZE + MAX_RESPONSE_BODY_SIZE

/**
 * Handles and processes the http request.
 */
void * handle_http_request(void* p_client_socket){
	int client_socket = *(int*)(p_client_socket);
    free(p_client_socket); /** The pointer is no longer needed, so it's freed. */

	/** Read the request i nto a buffer */
    long val_read;
    char buffer[MAX_BUFFER_SIZE] = {0};
    val_read = read(client_socket ,buffer, MAX_BUFFER_SIZE);
    printf("%s\n",buffer);

	/** Initialize request structure */
	requestStruct request;
	request.parsed_request_size = sizeof(request.parsed_request);
	request.body_size = sizeof(request.body);

	/** Initialize response structure */
	responseStruct response;
	response.message_size = sizeof(response.message);
	response.body_size = sizeof(response.body);
	response.full_response_size = sizeof(response.full_response);

    /** Split the request to headers and body*/
	int iResult;
	iResult = split_http_request(buffer,&request);
	if (iResult == EXIT_FAILURE) {
		badRequest(&response);
	}

	/** Send response */
	strncat(response.full_response,response.message,response.message_size);
	strncat(response.full_response, response.body, response.body_size);
	write(client_socket, response.full_response, response.full_response_size);

	close(client_socket);

	return NULL;
}
/**
 * Splits the http request to lines, so that they can be passed to a parsing function.
 * The request and the headers are saved in p_parsed_request 
 * @param buffer is a pointer to the buffer received from the client.
 * @param request is a pointer to a requestStruct where the parsed request will be saved.
 */
int split_http_request(char *buffer, requestStruct *request) {
	int headers_completed = 0;
	for (int i = 0; i < MAX_NUMBER_OF_HEADERS; i++) {
		for (int j = 0; j < MAX_HEADER_SIZE; j++) {
			/** There should never be a null terminating byte in the headers section. */
			if (*buffer == '\0')
				return EXIT_FAILURE;
			/** If header is finished, move to the next one */
			if (*buffer == '\n') {
				request->parsed_request[i][j] = '\0';
				/** Check if header section is completed, indicated by another blank line */
				if (*(++buffer) == '\n') {
					++buffer;
					headers_completed = 1;
				}
				break;
			}
			else {
				request->parsed_request[i][j] = *buffer;
				++buffer;
			}
		}
		if (headers_completed)
			break;
	}
	/** Read the body of the http request */
	for (int i = 0; i < MAX_BODY_SIZE; i++) {
		request->body[i] = *buffer;
		if (*buffer == '\0')
			break;
		buffer++;
	}
	/** Make sure body is null terminated */
	request->body[MAX_BODY_SIZE - 1] = '\0';

	return EXIT_SUCCESS;
}
