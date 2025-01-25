#include "file.h"

// hard coded 404 not found for now
char* NF404 = "HTTP/1.1 404 Not Found\n\n<!DOCTYPE html><html><body><h1>404 Not Found!</h1></body></html>";

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

// checks if the target file exists in the local file system
bool targetExists(char* path, char* fname) {
    DIR* dir;
    struct dirent* ent;
    // if path cannot be opened
    if ((dir = opendir(path)) == NULL) {
        printf("Target existance checker failed to open path\n");
        return false;
    }
    // if a file in the directory matches fname, return true
    while((ent = readdir(dir)) != NULL) {
        if (strncmp(ent->d_name, fname, strlen(fname)) == 0) return true;
        printf("%s\n", ent->d_name);
    }
    // if none return false
    return false;

}

// given a filename, read the content from the file
// TODO: break this into helper functions
Response* readContent(char* fname) {
    if (strlen(fname) == 1 && fname[0] == '/') fname = "index.html"; // if path to target resource is '/'
    Response* out;
    char* path = createPath(fname);
    printf("%s\n", path); // Debugging
    // copying directory path into a new string
    int dirPathLen = strlen(path) - strlen(fname) - 1;
    char* dir = malloc(dirPathLen);
    for (int i = 0; i < dirPathLen; i++) {
        dir[i] = path[i];
    }
    // checking if file exists
    // if not return 404 page
    if (!targetExists(dir, fname)) {
        out = malloc(sizeof(Response));
        out->payload = NF404;
        out->next = NULL;
        free(dir);
        return out;
    }
    free(dir);

    // opening target file
    FILE* target = fopen(path, "r");
    printf("File Opening Executes\n");
    // error opening file
    if (target == NULL) {
        printf("File Opening Executes but Fails\n");
        fclose(target);
        free(path);
        error("Error opening target of GET request\n");
    }

    // buffer to copy target contents
    char buffer[4096];
    bzero(buffer, 4096);

    // loads content of webpage onto out, a Response instance
    out = malloc(sizeof(Response));
    out->next = NULL;
    Response* cur = out;
    Response* del = out;
    // -1 included because I don't know if fgets will only read sizeof(buffer) -1 characters automatically to fit '\0'
    while (fgets(buffer, sizeof(buffer) - 1, target) != NULL) { // TODO: fgets reads one line so each chunk will have one line of HTML - change this so that the full 4096 is used
        Response* tmp = malloc(sizeof(Response));
        tmp->payload = malloc(strlen(buffer) + 1);
        strncpy(tmp->payload, buffer, strlen(buffer));
        tmp->payload[strlen(buffer)] = '\0';
        tmp->next = NULL;
        cur->next = tmp;
        cur = cur->next;
        bzero(buffer, 4096); // clearing buffer
    }
    out = out->next;
    free(del);

    printf("Response Building Executes\n"); // Debugging

    free(path);
    fclose(target);
    return out;
}



// TODO:

// Make the parser able to determine the target of a GET request and connect to this appropriately