//  assign6
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_COMMANDS 6
#define BUFFER_SIZE 1024



void parse_commands(char *input, char *commands[]) 
{
    int i = 0;

    char *token = strtok(input, ",");

    while (token != NULL && i < MAX_COMMANDS) 
{
        while (*token == ' ') token++; 
        char *end = token + strlen(token) - 1;
 
     while (end > token && *end == ' ') *end-- = '\0';

        commands[i++] = token;
        token = strtok(NULL, ",");
    }


    commands[i] = NULL;
}

 
void write_commands_to_pipe(int write_fd, char *commands[], int command_count) 
{ 
    for (int i = 0; i < command_count; i++) 
{
        if (write(write_fd, commands[i], BUFFER_SIZE) == -1)  
{
            fprintf(stderr, "Error writing to pipe: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
 

        }
    }


    close(write_fd);

}



void read_and_execute_command_from_pipe(int read_fd) 
{
    char command[BUFFER_SIZE];
    if (read(read_fd, command, BUFFER_SIZE) > 0) 
{
        char *args[MAX_COMMANDS];
        int i = 0;


        args[i] = strtok(command, " ");

        while (args[i] != NULL) {
            args[++i] = strtok(NULL, " ");

        }

        // printing all
        fprintf(stderr, "PID: %d, PPID: %d, CMD: %s\n", getpid(), getppid(), args[0]);
        execvp(args[0], args);

        fprintf(stderr, "Error executing command '%s': %s\n", args[0], strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}



int main(int argc, char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"command1 , command2 , ...\"\n", argv[0]); 


        return EXIT_FAILURE;
    }
	
    char *commands[MAX_COMMANDS + 1] = {NULL};
    parse_commands(argv[1], commands);

  	  int command_count = 0;
   	  for (int i = 0; commands[i] != NULL; i++) 
{
        command_count++;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) 
{	//error
        fprintf(stderr, "Error creating pipe: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // commands go to pipe

    if (fork() == 0) {
        // child wriitng
        close(pipe_fd[0]); 

        write_commands_to_pipe(pipe_fd[1], commands, command_count); 
    exit(EXIT_SUCCESS);
    }
 else 
{
        close(pipe_fd[1]); 
        wait(NULL);
    }


 
    pid_t pids[MAX_COMMANDS]; 

    // forks

    for (int i = 0; i < command_count; i++) 
{
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork failed: %s\n", strerror(errno)); 
            return EXIT_FAILURE;
        }
 else if (pid == 0) {
  
            read_and_execute_command_from_pipe(pipe_fd[0]);
        } else 
{
            pids[i] = pid; 
        }
    }
    close(pipe_fd[0]);

    for (int i = 0; i < command_count; i++) 
{
        waitpid(pids[i], NULL, 0); 
    }


    return EXIT_SUCCESS;
}
