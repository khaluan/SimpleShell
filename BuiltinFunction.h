#ifndef BUILTINFUNCTION_H_
#define BUILTINFUNCTION_H_
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "Utility.h"
#include "HistoryManager.h"
enum builtin {
    PWD, CD, LS, SORT, HIST, EXIT, LAST_CMD
};
const char* builtin_functions[BUILTIN_SIZE];

void ls();
void pwd();
void cd(char* new_dir);
void sort(char* filename);
void display_history();
void last_command();
#endif