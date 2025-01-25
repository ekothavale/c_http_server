#ifndef chttp_struct_h
#define chttp_struct_h

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
    char* target; // Target resource of a get request
}Request;

// payload is any content being sent back to the client
// next is next node in linked list
typedef struct Response {
    char* payload;
    struct Response* next;
}Response;

#endif