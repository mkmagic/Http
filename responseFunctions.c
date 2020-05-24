#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parseHttpProtocol.h"

#define PROTOCOL "HTTP/1.1"
#define NUM_OF_SUPPORED_REQUESTS 7

/**
 * Defines a pointer to a generic function that constructs a response message and body.
 */
typedef void (*responseFunction)(requestStruct *request, responseStruct	*response);

/**
 * A structure that stores functions that construct response messages according to a request.
 * A short description of the request (i.e. GET, POST etc) is stored in request_description.
 */
typedef struct {
	const char *request_description;
	responseFunction func;
}responseDictionary;

void response_selector(requestStruct *request, responseStruct *response) {

responseDictionary dictionary[NUM_OF_SUPPORED_REQUESTS] = {};

}

/**
 * Creates a 400 Bad Request message, indicating a syntax error.
 * @param response holds the response struct.
 */
void badRequest(responseStruct *response) {
	const char msg[] = PROTOCOL " 400 Bad Request\n\n";
	strncat(response->message, msg, response->message_size);
}

void getRequest(requestStruct *request, responseStruct *response) {
	char command[MAX_HEADER_SIZE] = request->parsed_request[0];
	char path[MAX_HEADER_SIZE];	
	char resolved_path[PATH_MAX];
	readPath(command, path, resolved_path);

	/** Try to convert the path to a real path */
}

/** 
 * Reads the path for the file requested.
 * @param p_command a pointer to the first line of the http request.
 * @param p_path a pointer to a pointer to a buffer to store the path.
 * @return EXIT_SUCCESS if succesful, otherwise calls badRequest and returns 
 * EXIT_FAILURE.
 */
int readPath(char *p_command, char *p_path, char *p_resolved_path) {
	/** Read the path to a char array pointed by p_path*/
	while (*p_command != ' ')
		p_command++;
	p_command++;
	while(*p_command != ' ') {
		if (*p_command == '\0') {
			badRequest(response);
			return EXIT_FAILURE;
		}
		*p_path = *p_command;
	}
	*p_path = '\0';
	/** Resolve the path */





	return EXIT_SUCCESS;
}
