#ifndef chttp_parser_h
#define chttp_parser_h

// Possible http request methods
// Only GET is supported in the current iteration
typedef enum {
    UNKNOWN, // represents an illegal method type
    METHOD_NULL, // placeholder for when a Request struct is reset
    GET,
    HEAD, // unsupported
    POST, // unsupported
    PUT, // unsupported
    DELETE, // unsupported
    CONNECT, // unsupported
    OPTIONS, // unsupported
    TRACE // unsupported
}httpMethod;

// Contains state of the parser
// Start is the start of a request
// Current is a pointer to the current character in the parse
typedef struct Parser {
    char* start;
    char* current;
}Parser;

// method is the http method of the request
// Add more fields as more support for other parts of http request is added
typedef struct Request {
    httpMethod method;
}Request;

// Should later create a request struct to hold the components of the request
// for now just returns the http method
Request parse(char* raw);

#endif