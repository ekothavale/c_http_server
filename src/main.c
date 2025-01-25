//#include <stdio.h> included in error.h
//#include <stdlib.h> same with this
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "error.h"
#include "parser.h"
#include "server.h"
#include "file.h"

// maximum number of clients that can be connected to the server
const int MAX_CLIENTS = 5;
// length of character buffer to read requests
const int BUFFER_LEN = 256;

const int PORT = 5001;

//http responses
const char* OK200 = "HTTP/1.1 200 OK\n\n<!DOCTYPE html><html><body><h1>Hello!</h1></body></html>";

// TODO: test function
char* growArr(char* arr, int size) { // untested function
    char* out = malloc(size);
    for (int i = 0; i < size/2; i++) {
        out[i] = arr[i];
    }
    free(arr);
    return out;
}

// Move to appropriate location after tested
// Converts webpage loaded into a linked list of response objects into a single string
char* compileResponse(Response* response) {
    char* out = malloc(4096);
    int outsize = 4096;
    int pos = 0;
    while (response != NULL) {
        for (int i = 0; i < 4096; i++) {
            printf("%d.", *(response->payload+i));
            // stop reading string node if null terminator encountered
            if (*(response->payload+i) == '\0') {
                break;
            }
            // grow array
            if (pos == outsize) {
                outsize *= 2;
                out = growArr(out, outsize);
            }
            // read characters into output
            out[pos] = *(response->payload+i);
            pos++;
        }
        response = response->next;
        if (response == NULL) printf("response is eventually null\n");
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }
    printf("compileResponse() almost returns\n"); // debugging
    return out;
    printf("compileResponse() almost returns\n"); // debugging
}

int main(int argc, char** argv) {
    // Debugging to test if readContent functions properly. delete once this is confirmed
    /*Response* test = readContent("/");*/
    /*if (argc != 2) {
        error("Improper arguments: [filename] [port]");
    }*/
    int sockfd, newsockfd, portno, n;
    // character buffer to read requests into
    char buffer[BUFFER_LEN];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    
    // SOCK_STREAM is TCP bc TCP is a stream protocol
    // SOCK_DGRAM is UDP bc UDP is a datagram protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        error("Socket could not be opened");
    }
    // clears serv_addr
    bzero((char*) &serv_addr, sizeof(serv_addr));
    // second argument should be port number;
    //portno = atoi(argv[1]);
    portno = PORT;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // htons is host to network shot
    serv_addr.sin_port = htons(portno);
    // bind() assigns an address to the socket
    // @return 0 if successful -1 if not
    if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Could not bind server address to socket");
    }
    // AT THIS POINT:
    // SOCKET HAS BEEN BUILT AND CONNECTED TO AN ADDRESS
    // NOW LISTENING FOR CLIENTS

    // @param backlog is the number of clients that can connect to the server at once
    int listened = listen(sockfd, MAX_CLIENTS);

    clilen = sizeof(cli_addr);

    // accepting a client;
    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error("Client could not be accepted by the server");
    }
    
    printf("Executes\n");

    // main Read-Write loop
    while(1) {
        // clearing buffer
        bzero(buffer, BUFFER_LEN);
        // @param newsockfd: file descriptor generated when a request is accepted
        // @param buffer: char buffer
        // @param buffer_size: size of char buffer
        n = read(newsockfd, buffer, BUFFER_LEN);
        if (n < 0) {
            error("Failed to read request");
        }
        // printing request to console
        printf("Client : %s\n", buffer);

        Request request = parse(buffer);
        Response* response = serve(request);
        char* compResponse = compileResponse(response);
        printf("Response compiled\n"); // debugging

        // reclearing buffer
        bzero(buffer, BUFFER_LEN);

        // copying request from stdin
        // proper server will generate an appropriate response
        //fgets(buffer, BUFFER_LEN, stdin);

        // @param newsockfd
        // @param buffer
        // @param buffersize
        // all params same as in read
        // writing from buffer to client

        //n = write(newsockfd, buffer, strlen(buffer));
        n = write(newsockfd, compResponse, strlen(compResponse));
        free(compResponse);
        //n = write(newsockfd, OK200, strlen(OK200));
        if (n < 0) {
            error("Failed to write response\n");
        }
        printf("Server: %s\n", compResponse);

        // temporary exit code
        if (strncmp("EXIT", compResponse, 4) == 0) {
            break;
        }
    }

    // closing sockets
    // as of now, sockets are unclosed when an error function is called
    close(newsockfd);
    close(sockfd);
    return 0;
}

