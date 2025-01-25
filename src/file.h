#ifndef chttp_file_h
#define chttp_file_h

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

#include "error.h"
#include "struct.h"

Response* readContent(char* fname);

#endif