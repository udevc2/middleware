#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define FIFO_OUT "/tmp/fifo_out"
#define FIFO_IN "/tmp/fifo_in"
#define BUFFER_SIZE 1024

void *read_fifo(void *arg) {
    int fd;
    char buffer[BUFFER_SIZE];

    fd = open(FIFO_OUT, O_RDONLY);
    if (fd == -1) {
        perror("Error opening FIFO_OUT");
        pthread_exit(NULL);
    }

    while (1) {
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("%s", buffer);
            fflush(stdout);
        }
    }

    close(fd);
    return NULL;
}

void *write_fifo(void *arg) {
    int fd;
    char buffer[BUFFER_SIZE];

    fd = open(FIFO_IN, O_WRONLY);
    if (fd == -1) {
        perror("Error opening FIFO_IN");
        pthread_exit(NULL);
    }

    while (fgets(buffer, sizeof(buffer), stdin)) {
        write(fd, buffer, strlen(buffer));
    }

    close(fd);
    return NULL;
}

int main() {
    pthread_t read_thread, write_thread;

    // Create FIFOs if they do not exist
    mkfifo(FIFO_OUT, 0666);
    mkfifo(FIFO_IN, 0666);

    // Create threads
    pthread_create(&read_thread, NULL, read_fifo, NULL);
    pthread_create(&write_thread, NULL, write_fifo, NULL);

    // Wait for threads to finish
    pthread_join(read_thread, NULL);
    pthread_join(write_thread, NULL);

    return 0;
}
