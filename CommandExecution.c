#include "CommandExecution.h"
void execute_command(char command[]){
    struct Command cmd = parse_command(command);
    pid_t pid = fork();

    if (pid == 0){
        execvp(cmd.command, cmd.argv);
        exit(0);
    }
    else{
        wait(NULL);
    }
}