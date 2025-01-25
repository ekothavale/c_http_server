#include "parser.h"
#include "string.h"
#include "error.h"

// TODO: Add support for parsing the target of a GET request

Parser parser;
Request request;

// length of the CONNECT method which is the longest http method
const int LONGEST_METHOD_LENGTH = 7;
const int GET_TARGET_MAX_LENGTH = 4096;


// resets the Parset struct
void resetPartser() {
    parser.start = NULL;
    parser.current = NULL;
}

// resets the Request struct
void resetRequest() {
    request.method = METHOD_NULL;
}

// initiallizes the Parser struct to a new raw http request
void initializeParser(char* request) {
    parser.start = request;
    parser.current = request;
}

// parses the http method from the raw http request
void getMethod() {
    // method should be the first part of the request
    if (parser.current != parser.start) {
        error("Parser error: tried to parse http method after parsing other information from the request\n");
    }
    char method[LONGEST_METHOD_LENGTH + 1];
    bzero(method, LONGEST_METHOD_LENGTH);
    int tally = 0;
    while(*(parser.current) != ' ') {
        method[tally] = *(parser.current);
        if (tally > LONGEST_METHOD_LENGTH) {
            error("First component of request must be valid http method\n");
        }
        tally++;
        parser.current++;
    }
    method[tally] = '\0';

    // parsing method and updating parser struct
    if (strcmp(method, "GET") == 0) {
        request.method = GET;
    } else if (strcmp(method, "HEAD") == 0) {
        request.method = HEAD;
    } else if (strcmp(method, "POST") == 0) {
        request.method = POST;
    } else if (strcmp(method, "PUT") == 0) {
        request.method = PUT;
    } else if (strcmp(method, "DELETE") == 0) {
        request.method = DELETE;
    } else if (strcmp(method, "CONNECT") == 0) {
        request.method = CONNECT;
    } else if (strcmp(method, "OPTIONS") == 0) {
        request.method = OPTIONS;
    } else if (strcmp(method, "TRACE") == 0) {
        request.method = TRACE;
    } else {
        request.method = UNKNOWN;
    }
}

void getTarget() {
    if (*(parser.current) != ' ') { // after reading request method, the current character should be a space in a properly formatted request
        error("Parser error: target of GET request formatted incorrectly\n");
    }
    parser.current++; // moving parser one character forward onto the start of the target path
    char method[GET_TARGET_MAX_LENGTH];
    bzero(method, GET_TARGET_MAX_LENGTH);
    int tally = 0;
    while(*(parser.current) != ' ' && tally < GET_TARGET_MAX_LENGTH - 1) {
        method[tally] = *(parser.current);
        tally++;
        parser.current++;
    }
    method[tally] = '\0';
    request.target = malloc(tally);
    strncpy(request.target, method, tally);
}

// parses http request for important information
// currently only parses the method of the request
Request parse(char* raw) {
    resetPartser();
    resetRequest();
    initializeParser(raw);
    getMethod();
    if (request.method == GET) getTarget();
    return request;
}