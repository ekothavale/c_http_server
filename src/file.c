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
    return pathToTarget;
}

bool targetExists(char* path, char* fname) {
    DIR* dir;
    struct dirent* ent;
    // if path cannot be opened
    if ((dir = opendir(path)) == NULL) {
        return false;
    }
    // if a file in the directory matches fname, return true
    while((ent = readdir(dir)) != NULL) {
        if (strncmp(ent->d_name, fname, strlen(fname)) == 0) return true;
    }
    // if none do return false
    return false;

}

// given a filename, read the content from the file
void readContent(char* fname) {
    char* path = createPath(fname);
    // should check if path exists; if not, respond 404
    // copying directory path into a new string
    int dirPathLen = strlen(path) - strlen(fname) - 1;
    char* dir = malloc(dirPathLen);
    for (int i = 0; i < dirPathLen; i++) {
        dir[i] = path[i];
    }
    // checking if file exists
    if (!targetExists(dir, fname)) {
        // will later return 404 page
        printf("404 Not found\n");
    }
    free(dir);

    // opening target file
    FILE* target = fopen(path, "r");
    // error opening file
    if (target == NULL) {
        fclose(target);
        free(path);
        error("Error opening target of GET request\n");
    }

    // buffer to copy target contents
    char buffer[4096];

    // for now prints out contents of target file
    while (fgets(buffer, sizeof(buffer), target) != NULL) {
        printf("%s", buffer);
    }
    printf("\n");

    free(path);
    fclose(target);
}



// TODO:

// Read and return the contents of the file

// Make the parser able to determine the target of a GET request connect to this appropriately