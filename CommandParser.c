#include "CommandParser.h"
struct Command parse_command(char command[]){
    struct Command cmd;
    char* token;
    token = strtok(command, " ");
    cmd.command = strdup(token); 
    
    size_t i = 0;
    do {
        cmd.argv[i] = strdup(token);
        token = strtok(NULL, " ");
        ++i;
    } while (token != NULL);

    cmd.argv[i] = NULL;
    return cmd;
}