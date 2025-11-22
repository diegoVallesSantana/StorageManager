#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

int fd[2];
static int write_fd;
static pid_t child_pid;

int write_to_log_process(char *msg){
    if (msg == NULL) return -1;
    write(write_fd, msg, strlen(msg));
    write(write_fd, "\n", 1);
    return 0;
}

int create_log_process(){

    if (pipe(fd) == -1) {
        fprintf(stderr, "Error: pipe creation failed in create_log_process\n");
        return -1;
    }

    pid_t pid = fork(); // parent gets pid  > 0 and child gets pid == 0

    if (pid < 0) {
        fprintf(stderr, "Error: fork failed in create_log_process\n");
        return -1;
    }

    if (pid > 0) {
        // ---------------- PARENT PROCESS ---------------- pid >0
        write_fd = fd[1];
        child_pid = pid;
        close(fd[0]);
        return 0;

    } else {
        // ---------------- CHILD PROCESS ---------------- pid = 0
        close(fd[1]);

        FILE* log = fopen("gateway.log", "a");
        if (log == NULL) {
            fprintf(stderr, "Error: could not open log file in child create_log_process\n");
            exit(1);}

        char buffer[256];
        int counter = 1;

        while (1) {
            ssize_t n = read(fd[0], buffer, sizeof(buffer)-1);
            if (n <= 0) break; // not sure the code breaks when needed and the child reads the messages
            buffer[n] = '\0';
            if (buffer[n-1] == '\n') {
                buffer[n-1] = '\0';
            }
            if (strcmp(buffer, "SHUTDOWN") == 0) {
                break;
            }

            // Get timestamp
            time_t t = time(NULL);
            char *timestamp = ctime(&t);
            timestamp[strlen(timestamp) - 1] = '\0';

            fprintf(log, "%d - %s - %s\n", counter, timestamp, buffer);
            fflush(log);
            counter++;
        }
        fclose(log);
        close(fd[0]);
        exit(0);
    }
}


int end_log_process(){
    write(write_fd, "SHUTDOWN\n", 9);
    close(write_fd);
    waitpid(child_pid, NULL, 0);

    return 0;
}