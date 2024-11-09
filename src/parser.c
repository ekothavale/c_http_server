#include "parser.h"
#include "string.h"
#include "error.h"

Parser parser;
Request request;

// length of the CONNECT method which is the longest http method
const int LONGEST_METHOD_LENGTH = 7;


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
        error("Parser error: tried to parse http method after parsing other information from the request");
    }
    char method[LONGEST_METHOD_LENGTH];
    int tally = 0;
    while(*(parser.current) != ' ') {
        method[tally] = *(parser.current);
        if (tally > LONGEST_METHOD_LENGTH) {
            error("First component of request must be valid http method");
        }
        tally++;
        parser.current++;
    }

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


// parses http request for important information
// currently only parses the method of the request
Request parse(char* raw) {
    resetPartser();
    resetRequest();
    initializeParser(raw);
    getMethod();
    return request;
}