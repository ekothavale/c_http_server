#include "file.h"
#include "error.h"

//void readContent(char* path) {
//    if (!fileExists(path));
//
//}

// gets name of PWD
char* getDirName(char* cwd, int strl) {
    int tally = 0;
    for (int i = strl-1; i >= 0; i--) {
        if (cwd[i] == '/') break;
        tally++;
    }
    if (tally == 0) {
        error("Error: PWD is unnamed\n");
    }
    char* out = malloc(tally);
    for (int i = strl-tally; i < strl; i++) {
        out[i + tally - strl] = cwd[i];
    }

    return out;
}

// creates a path to the content folder
char* createPath(char* fname) {
    char pwd[1024];
    char* pathToTarget = malloc(128);

    if (getcwd(pwd, sizeof(pwd)) != NULL) {
        char* dirName = getDirName(pwd, strlen(pwd));
        if (strcmp(dirName, "src") == 0) {
            strncpy(pathToTarget, "../content/", 11);
        } else if (strcmp(dirName, "c_http_server") == 0) {
            strncpy(pathToTarget, "/content/", 9);
        } else  {
            free(dirName);
            error("Error: Server run from illegal directory. Should be run from c_http_server or c_http_server/src");
        }      
    } else {
        error("Error: Failed to read present working directory");
    }

    // compete path to target resource;
    strcat(pathToTarget, fname);
    printf("Works fine through here\n");
    return pathToTarget;
}

// given a filename, read the content from the file
void readContent(char* fname) {
    char* path = createPath(fname);
    
    FILE* target = fopen(path, "r");
    // error opening file
    if (target == NULL) {
        printf("The problem is here\n");
        fclose(target);
        free(path);
        char* emess = "Error: error opening resource file: ";
        strcat(emess, fname);
        error(emess);
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), target) != NULL) {
        printf("%s", buffer);
    }

    free(path);
    fclose(target);
}



// TODO:
// Create system that determines the path to the content folder whether or not
// the program is run from c_http_server or src

// Use that path to check if the filename requested by the client exists and
// that this program is allowed to read from it

// Read and return the contents of the file