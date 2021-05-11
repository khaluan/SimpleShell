#include "HistoryManager.h"


void history_handler(char** line){
    if (strcmp(*line, builtin_functions[LAST_CMD]) == 0){
        free(*line);
        *line = strdup(history[0]);
        return;      
    }
    else
        push_history(*line);
    
}

void init_history(){
    history = (char **) malloc(HISTORY_SIZE * sizeof(char *));

    for (int i = 0; i < HISTORY_SIZE; ++i) 
        history[i] = NULL;
}

void push_history(char* newHistory){
        
    if (history[HISTORY_SIZE - 1] != NULL) {
        free(history[HISTORY_SIZE - 1]);
        history[HISTORY_SIZE - 1] = NULL;
    }

    for (int i = HISTORY_SIZE - 1; i > 0; --i) {
        if (history[i - 1] == NULL) continue;
        history[i] = history[i - 1];
    }

    history[0] = strdup(newHistory);
}

void clean_history(){
    for (int i = 0; i < HISTORY_SIZE; ++i) {
        if (history[i] != NULL) free(history[i]);
    }

    free(history);
}