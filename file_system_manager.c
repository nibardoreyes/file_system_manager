// Nibardo Reyes Felix, Jashua Alvarez
// CST-315 Operating Systems
// Professor Ricardo Citro
// October 20, 2024
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h> // For exit and mkdir
#include <errno.h>  // For error handling

// Function to exit the shell
void exit_shell() {
    printf("Exiting shell\n");
    _exit(0); // Exit the program
}

// Function to change directory
 // uses the chchir() system calll
void change_directory(char *path) {
    if (chdir(path) != 0) {
        perror("cd failed");
    }
}

// Function to execute a command
// use exev to execute command
// this is a more elegant manner that using else if commands 
void execute_command(char *command) {
    pid_t pid = fork(); // Fork a new process

    if (pid < 0) {
        perror("Fork failed");
        return; // Continue the loop if fork fails
    } else if (pid == 0) { // Child process to handle command execution
        char *args[] = {"/bin/sh", "-c", command, NULL};
        execv(args[0], args); // Execute the command
        perror("execv failed"); // Only reached if execv fails
        exit(1); // Exit child process on failure
    } else { // Parent process waits for the child process to finish
        wait(NULL); // Wait for the child process to finish executing
    }
}



int main() {
    char input[300]; // Buffer to store user input
    printf("Input one of the following commands-\n");
    printf("pwd, ls, ps, df, whoami, uname, date, mkdir, cd, ./script.sh, exit\n");
    printf("\n========================\n");

    // Infinite loop for continuous shell interaction
    while (1) {
        printf("NibardoShell >> "); // Prompt for user input
        // fgets to help prevent buffer overflow
        fgets(input, sizeof(input), stdin); // Get user command
        input[strcspn(input, "\n")] = 0; // Remove newline character

        if (strcmp(input, "exit") == 0) {
            exit_shell(); // Exit the shell
        }

        //for cd
        if (strncmp(input, "cd ", 3) == 0) {
            change_directory(input + 3);
            continue; // use to skip the execution step
        }

        //for split inputs using ";"
        // used strtok to help break string into smaller parts
        char *command = strtok(input, ";");
        while (command != NULL) {
            //use to trim leading spaces
            while (*command == ' ') command++;

            // Execute the command
            execute_command(command);
            command = strtok(NULL, ";"); // Get the next command
        }
    }

    return 0;
}
