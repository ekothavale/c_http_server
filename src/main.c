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

int main(int argc, char** argv) {
    readContent("ethan.html");
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
        char* response = serve(request);


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
        n = write(newsockfd, response, strlen(response));
        //n = write(newsockfd, OK200, strlen(OK200));
        if (n < 0) {
            error("Failed to write response\n");
        }
        printf("Server: %s\n", OK200);

        // temporary exit code
        if (strncmp("EXIT", response, 4) == 0) {
            break;
        }
    }

    // closing sockets
    // as of now, sockets are unclosed when an error function is called
    close(newsockfd);
    close(sockfd);
    return 0;
}

