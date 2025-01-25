#include <string.h>

#include "server.h"
#include "file.h"
#include "error.h"

// TODO:
// Connect to file.readContent

Response* serve(Request request) {
    Response* response;
    if (request.method == GET) {
        response = malloc(sizeof(response));
        response->payload = "HTTP/1.1 200 OK\n\n"; // TODO: Delete this; response struct should have a method and this file should assemble appropriately
        response->next = readContent(request.target);
    } else if (request.method == UNKNOWN) {
        response->next = NULL;
        response->payload = "HTTP/1.1 400 Bad Request\n\n<!DOCTYPE html><html><body><h1>Bad Request!</h1></body></html>";
    } else {
        response->next = NULL;
        response->payload = "HTTP/1.1 501 Not Implemented\n\n<!DOCTYPE html><html><body><h1>We don't support this action yet!</h1></body></html>";;
    }
    printf("Response built\n"); // debugging
    return response;
}