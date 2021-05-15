#include "CommandExecution.h"
#include <fcntl.h>

void execute_line(char line[]){
    int pipefd[2];
    pipe(pipefd);

    switch(classify_line(line)){
        case FOREGROUND:
            run_foreground(line, pipefd);
            break;
        case BACKGROUND:
            run_background(line, pipefd);
            break;
        case PIPE:
            run_pipe(line);
            break;
        case REDIRECT:
            run_redirect(line, pipefd);
            break;
        default:
            printf("This is not good\n");
            break;
    }
}

int execute_command(struct Command cmd, int pipefd[2], enum PipeType pipe_type, int redirectType, char * redirectStuff) {
    if (strcmp(cmd.command, builtin_functions[CD]) == 0){
        cd(cmd.argv[1]);
        return 0;
    }

    fflush(stdout);
    pid_t pid = fork();
    if (pid == 0){
        // printf("Command %s with pid %d is running\n", cmd.command, getpid());
        //SOMETHING HERE
        config_pipe(pipefd, pipe_type);
        if (redirectType == 1 /* >*/) {
            int file_desc = open(redirectStuff,O_WRONLY | O_TRUNC);
            if(file_desc < 0)  printf("Error opening the output file\n");
            dup2(file_desc, 1);
        }
        else if (redirectType == 2 /* < */) {
            int file_desc = open(redirectStuff, O_RDONLY);
            if(file_desc < 0) printf("Error opening the input file\n");
            dup2(file_desc, 0);
        }

        if (strcmp(cmd.command, builtin_functions[PWD]) == 0)
            pwd(); 
        else if(strcmp(cmd.command, builtin_functions[LS]) == 0)
            ls();
        else if (strcmp(cmd.command, builtin_functions[SORT]) == 0)
            sort(cmd.argv[1]);
        else if (strcmp(cmd.command, builtin_functions[HIST]) == 0)
            display_history();
        else    
            execvp(cmd.command, cmd.argv);
        exit(0);
    }
    return pid;
}

void config_pipe(int pipefd[2], enum PipeType pipe_type){
    switch(pipe_type){
        case WRITE_PIPE:{
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
            break;
        }
        case READ_PIPE:{
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);
            break;
        }
    }
}

void run_foreground(char command[], int pipefd[2]){
    struct Command cmd = parse_command(command);
    int child = execute_command(cmd, pipefd, NONE, 0, NULL);

    waitpid(child, NULL, 0);
    
}

void run_background(char command[], int pipefd[2]){
    if (is_full()){
        fprintf(stderr, "Too many children process\n");
        return;
    }

    size_t len = strlen(command);
    if (command[len - 1] != '&'){
        fprintf(stderr, "This is not background process\n");
        return;
    }
    else 
        command[len - 1] = '\0';
    
    struct Command cmd = parse_command(command);

    int child_pid = execute_command(cmd, pipefd, NONE, 0, NULL);
    add_child_id(child_pid);

}

void run_redirect(char line[], int pipefd[2]) {
    char redirectSign;
    int i = 0;
    for (; line[i] != '\0'; ++i){
        if (line[i] == '>' || line[i] == '<') {
            redirectSign = line[i];
            break;
        } 
    }
    if (line[i] == '\0') {
        printf("Error in redirection ! \n");
        return;
    }

    ++i;
    char* token = malloc(sizeof(char) * i);
    strncpy(token, line, i);
    token[i - 1] = '\0';
    printf("%s\n", token);
    struct Command cmd1 = parse_command(token);
    free(token);
    
    while(line[i] == ' ') ++ i;
    size_t len = strlen(line) - i;
    token = malloc(sizeof(char) * len);
    strncpy(token, line + i, len);
    printf("%s\n", token);
    struct Command cmd2 = parse_command(token);
    free(token);

    if (redirectSign == '>') {
        int child = execute_command(cmd1, pipefd, NONE, 1, cmd2.command);
        waitpid(child, NULL, 0);
    }
    else if (redirectSign == '<') {
        int child = execute_command(cmd1, pipefd, NONE, 2, cmd2.command);
        waitpid(child, NULL, 0);
    }
}

void run_pipe(char line[]){
    size_t i = 0;
    for (; line[i] != '|'; ++i);

    ++i;
    char* token = malloc(sizeof(char) * i);
    strncpy(token, line, i);
    token[i - 1] = '\0';
    // printf("%s\n", token);
    struct Command cmd1 = parse_command(token);
    free(token);
    
    while(line[i] == ' ') ++ i;
    size_t len = strlen(line) - i;
    token = malloc(sizeof(char) * len);
    strncpy(token, line + i, len);
    // printf("%s\n", token);
    struct Command cmd2 = parse_command(token);
    free(token);

    int pipefd[2];
    pipe(pipefd);
    
    int child2 = execute_command(cmd2, pipefd, READ_PIPE, 0, NULL);
    int child1 = execute_command(cmd1, pipefd, WRITE_PIPE, 0, NULL);
    
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(child2, NULL, 0);    
    waitpid(child1, NULL, 0);    
}

int find_index_by_pid(int child_id){
    for (size_t i = 0; i < MAX_CHILD; ++i)
        if (background_child[i] == child_id)
            return i;
    return -1;
}
bool remove_child_id(int child_id){
    int index = find_index_by_pid(child_id);
    if (index == -1){
        // fprintf(stderr, "No child pid found\n");
        return false;
    }
    background_child[index] = -1;
    return true;
}

void add_child_id(int child_id){
    int index = find_index_by_pid(-1);
    if (index == -1){
        fprintf(stderr, "Too many children\n");
        return;
    }
    background_child[index] = child_id;
}

bool is_full(){
    return find_index_by_pid(-1) == -1;
}

void signal_handler(int signum){
    if (signum == SIGINT){
        printf("Ctr+C founded \n");
    }
    else if (signum == SIGTSTP){
        printf("Ctr+Z founded \n");
    }
    else if (signum == SIGCHLD){
        int status;
        pid_t pid = wait(&status);
        //fprintf(stderr, "Process with pid %d exit\n", pid);
        if (pid > 0 && WIFEXITED(status) && remove_child_id(pid)){
            int exit_code = WEXITSTATUS(status);
            printf("Process %d exit with code %d", pid, exit_code);
        }
    }
    fflush(stdout);
}

void init_environment(){
    for (size_t i = 0; i < MAX_CHILD; ++i)
        background_child[i] = -1;
}