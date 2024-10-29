#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFFER_SIZE 256

int main() {
    int pipe1[2], pipe2[2];
    pid_t child1, child2;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
        exit(EXIT_FAILURE);

    child1 = fork();
    if (child1 == -1) {
        exit(EXIT_FAILURE);
    }

    if (child1 == 0) {
        close(pipe1[1]);
        close(pipe2[0]);

        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        close(pipe1[0]);
        close(pipe2[1]);

        execl("./out/child1", "child1", NULL);
        exit(EXIT_FAILURE);
    }

    close(pipe1[0]); 
    close(pipe2[1]);

    char buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        write(pipe1[1], buffer, bytes_read);
    }

    close(pipe1[1]);

    waitpid(child1, NULL, 0);

    child2 = fork();
    if (child2 == -1) {
        exit(EXIT_FAILURE);
    }

    if (child2 == 0) {
        close(pipe2[1]);

        dup2(pipe2[0], STDIN_FILENO);
        dup2(STDOUT_FILENO, STDOUT_FILENO);

        close(pipe2[0]);

        execl("./out/child2", "child2", NULL);
        exit(EXIT_FAILURE);
    }

    close(pipe2[0]);

    waitpid(child2, NULL, 0);

    return 0;
}
