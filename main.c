#include "BuiltinFunction.h"
#include "CommandParser.h"
#include "HistoryManager.h"
#include "CommandExecution.h"
#include "Utility.h"
#include <signal.h>
void init();
void cleanup();

int main(int argc, char** argv){
    
    init();

    char* command = NULL;
    size_t len = 0;
    while (1){
        fflush(stdout);
        fflush(stdin);
        printf("sh> ");
        
        size_t tmp = getline(&command, &len, stdin);
        command[tmp - 1] = '\0';
        
        history_handler(&command);
        if (strcmp(command, builtin_functions[EXIT]) == 0)
            break;
        else if (strcmp(command, "") == 0)
            continue;


        execute_line(command);
    }
    if(command) free(command);
    cleanup();
    return 0;
}

void init(){
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGCHLD, signal_handler);
    init_environment();
    init_history();
}

void cleanup(){
    clean_history();
}