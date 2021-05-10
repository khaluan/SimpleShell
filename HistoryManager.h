#ifndef HISTORYMANAGER_H_
#define HISTORYMANAGER_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "BuiltinFunction.h"
char ** history;

void init_history();
void history_handler(char** line);
void push_history(char* newHistory);

void clean_history();
#endif