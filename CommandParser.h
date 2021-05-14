#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utility.h"
#include "BuiltinFunction.h"

enum LineType {
    FOREGROUND, BACKGROUND, PIPE, REDIRECT
};
enum LineType classify_line(char command[]);
struct Command parse_command(char command[]);
bool is_builtin(char command[]);
#endif