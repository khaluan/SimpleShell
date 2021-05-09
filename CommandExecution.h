#ifndef COMMANDEXECUTION_H_
#define COMMANDEXECUTION_H_
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "CommandParser.h"


void execute_command(char command[]);

#endif