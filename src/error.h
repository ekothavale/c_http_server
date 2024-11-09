#ifndef chttp_error_h
#define chttp_error_h

#include <stdio.h>
#include <stdlib.h>

// prints error to stderr and exits program
void error(const char* msg);

// same as error but with specified exit code
void cerror(const char* msg, int ecode);

#endif