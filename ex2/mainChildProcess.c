#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork(); // parent gets pid  > 0 and child gets pid == 0

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) {
        // ---------------- PARENT PROCESS ---------------- => pid >0
        close(fd[0]);  // close read end

        char msg[] = "Hello There!";

        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]);

    } else {
        // ---------------- CHILD PROCESS ---------------- => pid = 0
        close(fd[1]);  // close write end

        char buffer[50];
        read(fd[0], buffer, sizeof(buffer));

        // Reverse case
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (islower(buffer[i])) buffer[i] = toupper(buffer[i]);
            else if (isupper(buffer[i])) buffer[i] = tolower(buffer[i]);
        }

        printf("Child received and modified: %s\n", buffer);

        close(fd[0]);
    }

    return 0;
}
