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

enum LineType classify_line(char line[]){
    size_t len = strlen(line);
    size_t i;
    for (i = 0; i < len; ++i){
        if (line[i] == '&')
            return BACKGROUND;
        else if (line[i] == '<' || line[i] == '>'){
            return REDIRECT;
        }
        else if (line[i] == '|')
            return PIPE;
    }
    return FOREGROUND;
}