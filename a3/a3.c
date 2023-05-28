#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/select.h>
#include <errno.h>

#define REQUEST_PIPE_NAME "REQ_PIPE_48169"
#define RESPONSE_PIPE_NAME "RESP_PIPE_48169"

#define BUFFER_SIZE 256

void handlePingRequest(int responsePipe) {
    char buffer[BUFFER_SIZE];
    strcpy(buffer, "PING PONG 48169");
    write(responsePipe, buffer, strlen(buffer) + 1);
}

int main() {
    int requestPipe, responsePipe;
    char buffer[BUFFER_SIZE];

    if (mkfifo(RESPONSE_PIPE_NAME, 0666) == -1) {
        if (errno != EEXIST) {
            perror("ERROR cannot create the response pipe");
            exit(1);
        }
    }

    requestPipe = open(REQUEST_PIPE_NAME, O_RDONLY | O_NONBLOCK);
    if (requestPipe == -1) {
        perror("ERROR cannot open the request pipe");
        exit(1);
    }

    responsePipe = open(RESPONSE_PIPE_NAME, O_WRONLY);
    if (responsePipe == -1) {
        perror("ERROR cannot open the response pipe");
        exit(1);
    }

    strcpy(buffer, "CONNECT");
    write(responsePipe, buffer, strlen(buffer) + 1);

    if (strcmp(buffer, "CONNECT") == 0) {
        printf("SUCCESS\n");
    } else {
        printf("ERROR\ncannot create the response pipe | cannot open the request pipe\n");
        exit(1);
    }

    while (1) {
        fd_set readFds;
        struct timeval timeout;

        FD_ZERO(&readFds);
        FD_SET(requestPipe, &readFds);

        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;

        int selectResult = select(requestPipe + 1, &readFds, NULL, NULL, &timeout);
        if (selectResult == -1) {
            perror("ERROR in select");
            break;
        } else if (selectResult == 0) {
            continue;
        }

        ssize_t bytesRead = read(requestPipe, buffer, BUFFER_SIZE);
        if (bytesRead > 0) {
            if (strcmp(buffer, "PING") == 0) {
                printf("Received PING request\n");
                handlePingRequest(responsePipe);
            }

        } else if (bytesRead == -1) {
            perror("ERROR reading from request pipe");
            break;
        }
    }

    close(requestPipe);
    close(responsePipe);

    unlink(RESPONSE_PIPE_NAME);

    return 0;
}

