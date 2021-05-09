#include "BuiltinFunction.h"
#include "CommandParser.h"
#include "HistoryManager.h"
#include "CommandExecution.h"
#include "Utility.h"
#include <signal.h>

void signal_handler(int signum){
    if (signum == SIGINT){
        printf("Ctr+C founded \n");
    }
    else if (signum == SIGTSTP){
        printf("Ctr+Z founded \n");
    }
    printf("sh> ");
    fflush(stdout);
}

int main(int argc, char** argv){
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);

    char* command = NULL;
    size_t len = 0;
    while (1){
        fflush(stdout);
        printf("sh> ");
        
        size_t tmp = getline(&command, &len, stdin);
        command[tmp - 1] = '\0';
        
        if (strcmp(command, builtin_functions[EXIT]) == 0)
            break;
        else if (strcmp(command, "") == 0)
            continue;

        history_handling(command);

        execute_command(command);
    }
    free(command);
    return 0;
}

