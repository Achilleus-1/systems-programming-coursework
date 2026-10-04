/* Coursework process-control demo: bounded comma-separated commands, no shell expansion. */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_COMMANDS 6
#define BUFFER_SIZE 1024
#define MAX_ARGS (BUFFER_SIZE / 2)

int parse_commands(char *input, char *commands[])
{
    int count = 0;
    char *token = strtok(input, ",");
    while (token != NULL) {
        while (isspace((unsigned char)*token)) token++;
        char *end = token + strlen(token);
        while (end > token && isspace((unsigned char)end[-1])) end--;
        *end = '\0';
        if (*token == '\0' || count == MAX_COMMANDS || strlen(token) >= BUFFER_SIZE)
            return -1;
        commands[count++] = token;
        token = strtok(NULL, ",");
    }
    commands[count] = NULL;
    return count;
}

int write_commands_to_pipe(int write_fd, char *commands[], int command_count)
{
    for (int i = 0; i < command_count; i++) {
        const char *cursor = commands[i];
        size_t remaining = strlen(cursor) + 1;
        while (remaining > 0) {
            ssize_t written = write(write_fd, cursor, remaining);
            if (written < 0) {
                if (errno == EINTR) continue;
                perror("Error writing command");
                close(write_fd);
                return -1;
            }
            cursor += written;
            remaining -= (size_t)written;
        }
    }
    close(write_fd);
    return 0;
}

void read_and_execute_command_from_pipe(int read_fd)
{
    char command[BUFFER_SIZE];
    size_t used = 0;
    while (used < sizeof(command)) {
        ssize_t received = read(read_fd, command + used, sizeof(command) - used);
        if (received < 0) {
            if (errno == EINTR) continue;
            perror("Error reading command");
            close(read_fd);
            _exit(EXIT_FAILURE);
        }
        if (received == 0) break;
        used += (size_t)received;
    }
    close(read_fd);
    if (used == 0 || memchr(command, '\0', used) == NULL) _exit(EXIT_FAILURE);

    char *args[MAX_ARGS + 1];
    size_t count = 0;
    char *token = strtok(command, " \t\r\n");
    while (token != NULL && count < MAX_ARGS) {
        args[count++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    if (count == 0 || token != NULL) _exit(EXIT_FAILURE);
    args[count] = NULL;
    execvp(args[0], args);
    perror("Error executing command");
    _exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"command1, command2, ...\"\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *commands[MAX_COMMANDS + 1];
    int command_count = parse_commands(argv[1], commands);
    if (command_count <= 0) {
        fprintf(stderr, "Provide 1-6 nonempty commands, each shorter than 1024 bytes.\n");
        return EXIT_FAILURE;
    }

    pid_t pids[MAX_COMMANDS];
    int launched = 0, failed = 0;
    for (int i = 0; i < command_count; i++) {
        int pipe_fd[2];
        if (pipe(pipe_fd) != 0) {
            perror("Error creating pipe");
            failed = 1;
            break;
        }
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            close(pipe_fd[0]); close(pipe_fd[1]);
            failed = 1;
            break;
        }
        if (pid == 0) {
            close(pipe_fd[1]);
            read_and_execute_command_from_pipe(pipe_fd[0]);
        }
        close(pipe_fd[0]);
        pids[launched++] = pid;
        /* Each child owns one pipe, so partial reads cannot split another command. */
        if (write_commands_to_pipe(pipe_fd[1], &commands[i], 1) != 0) failed = 1;
    }
    for (int i = 0; i < launched; i++) {
        int status;
        pid_t result;
        do { result = waitpid(pids[i], &status, 0); } while (result < 0 && errno == EINTR);
        if (result < 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0) failed = 1;
    }
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
