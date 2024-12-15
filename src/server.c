#include <string.h>

#include "server.h"
#include "error.h"


char* serve(Request request) {
    if (request.method == GET) {
        return "HTTP/1.1 200 OK\n\n<!DOCTYPE html><html><body><h1>Hello!</h1></body></html>";
    } else if (request.method == UNKNOWN) {
        return "HTTP/1.1 400 Bad Request\n\n<!DOCTYPE html><html><body><h1>Bad Request!</h1></body></html>";
    } else {
        return "HTTP/1.1 501 Not Implemented\n\n<!DOCTYPE html><html><body><h1>We don't support this action yet!</h1></body></html>";
    }
}