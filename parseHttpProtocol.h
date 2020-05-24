#pragma once
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
 * Defines a request structure. 
 */
typedef struct {
	char parsed_request[MAX_NUMBER_OF_HEADERS][MAX_HEADER_SIZE];
	char body[MAX_BODY_SIZE];
	size_t parsed_request_size;
	size_t body_size;
} requestStruct;

/**
 * Defines a response structure.
 */
typedef struct {
	char message[MAX_RESPONSE_MSG_SIZE];
	char body[MAX_RESPONSE_BODY_SIZE];
	char full_response[MAX_RESPONSE_SIZE];
	size_t message_size;
	size_t body_size;
	size_t full_response_size;
} responseStruct;

void * handle_http_request(void* p_client_socket);
int split_http_request(char *buffer, requestStruct *request);
