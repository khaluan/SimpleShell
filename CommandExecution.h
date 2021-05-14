#ifndef COMMANDEXECUTION_H_
#define COMMANDEXECUTION_H_
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "CommandParser.h"
#include "BuiltinFunction.h"
#include "Utility.h"

int background_child[MAX_CHILD];

int find_index_by_pid(int child_id);
bool remove_child_id(int child_id);
void add_child_id(int child_id);
bool is_full();

void execute_line(char line[]);
int execute_command(struct Command cmd, int pipefd[2], enum PipeType pipe_type);


void run_foreground(char command[], int pipefd[2]);
void run_background(char command[], int pipefd[2]);
void config_pipe(int pipefd[2], enum PipeType pipe_type);
void end_pipe(int pipefd[2], enum PipeType pipe_type);
void run_pipe(char line[]);

void init_environment();
void signal_handler(int signum);
#endif