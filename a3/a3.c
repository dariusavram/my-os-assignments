#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>


int main() {
    const char* resp_pipe = "RESP_PIPE_48169";
    const char* req_pipe = "REQ_PIPE_48169";

    if (mkfifo(resp_pipe, 0666) == -1) {
        perror("ERROR: Failed to create response pipe or open request pipe");
        exit(1);
    }

    int req_fd = open(req_pipe, O_RDONLY);
    if (req_fd == -1) {
        perror("ERROR: Failed to create response pipe or open request pipe");
        unlink(resp_pipe);
        exit(1);
    }

    int resp_fd = open(resp_pipe, O_WRONLY);
    if (resp_fd == -1) {
        perror("ERROR: Failed to create response pipe or open request pipe");
        unlink(resp_pipe);
        close(req_fd);
        exit(1);
    }

    const char* req_msg = "CONNECT$";

    if (write(resp_fd, req_msg, strlen(req_msg)) == -1) {
        perror("ERROR: Failed to create the response pipe or open the request pipe");
        unlink(resp_pipe);
        close(req_fd);
        close(resp_fd);
        exit(1);
    }

    printf("SUCCESS\n");

    while (1) {
        char req_buffer[250];
        unsigned int pos = 0;
        char ch = '\0';

        while (ch != '$' && pos < 250) {
            read(req_fd, &ch, sizeof(char));
            req_buffer[pos] = ch;
            pos++;
        }

        req_buffer[pos-1] = '\0';

        if (strcmp(req_buffer, "PING") == 0) {
            char* ping = "PING$";
            char* pong = "PONG$";
            unsigned int number = 48169;

            write(resp_fd, ping, strlen(ping));
            write(resp_fd, pong, strlen(pong));
            write(resp_fd, &number, sizeof(unsigned int));
        }

        int shm_fd;

        if (strcmp(req_buffer, "CREATE_SHM") == 0) {
            unsigned int size = 0;
            read(req_fd, &size, sizeof(unsigned int));

            char* create_msg = "CREATE_SHM$";
            char* success_msg = "SUCCESS$";
            char* error_msg = "ERROR$";

            shm_fd = shm_open("/8B9EuEB", O_CREAT | O_RDWR, 0664);

            if (shm_fd < 0) {
                write(resp_fd, create_msg, strlen(create_msg));
                write(resp_fd, error_msg, strlen(error_msg));
            } else {
                ftruncate(shm_fd, size);

                  write(resp_fd, create_msg, strlen(create_msg));
                  write(resp_fd, success_msg, strlen(success_msg));

            }
        }
            close(req_fd);
            close(resp_fd);
            unlink(resp_pipe);
            break;

    }

    return 0;
}

