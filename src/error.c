#include "error.h"

void error(const char* msg) {
    perror(msg);
    exit(1);
}

void cerror(const char* msg, int ecode) {
    perror(msg);
    exit(ecode);
}