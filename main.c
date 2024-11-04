#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// maximum number of clients that can be connected to the server
const int MAX_CLIENTS = 5;
// length of character buffer to read requests
const int BUFFER_LEN = 256;

void error(const char* msg) {
    perror(msg);
    exit(-1);
}

void cerror(const char* msg, int ecode) {
    perror(msg);
    exit(ecode);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        error("Improper arguments: [filename] [port]\n");
    }
    int sockfd, newsockfd, portno, n;
    // character buffer to read requests into
    char* buffer;

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    
    // SOCK_STREAM is TCP bc TCP is a stream protocol
    // SOCK_DGRAM is UDP bc UDP is a datagram protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        cerror("Socket could not be opened\n", -2);
    }
    // clears serv_addr
    bzero((char*) &serv_addr, sizeof(serv_addr));
    // second argument should be port number;
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // htons is host to network shot
    serv_addr.sin_port = htons(portno);
    // bind() assigns an address to the socket
    // @return 0 if successful -1 if not
    if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr) < 0)) {
        cerror("Could not bind server address to socket\n", -2);
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
        cerror("Client could not be accepted by the server\n", -2);
    }

    // allocating buffer
    buffer = malloc(BUFFER_LEN);

    // main Read-Write loop
    while(1) {
        // clearing buffer
        bzero(buffer, BUFFER_LEN);
        // @param newsockfd: file descriptor generated when a request is accepted
        // @param buffer: char buffer
        // @param buffer_size: size of char buffer
        n = read(newsockfd, buffer, BUFFER_LEN);
        if (n < 0) {
            cerror("Failed to read request\n", -3);
        }
        // printing request to console
        printf("Client : %s\n", buffer);

        // reclearing buffer
        bzero(buffer, BUFFER_LEN);

        // copying request from stdin
        // proper server will generate an appropriate response
        fgets(buffer, BUFFER_LEN, stdin);

        // @param newsockfd
        // @param buffer
        // @param buffersize
        // all params same as in read
        // writing from buffer to client
        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0) {
            cerror("Failed to write response\n", -3);
        }

        // temporary exit code
        if (strncmp("EXIT", buffer, 4) == 0) {
            break;
        }
    }

    // closing sockets
    close(newsockfd);
    close(sockfd);
    
    free(buffer);
    return 0;
}

