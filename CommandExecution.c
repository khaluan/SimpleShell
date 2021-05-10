#include "CommandExecution.h"
void execute_line(char line[]){
    switch(classify_line(line)){
        case FOREGROUND:
            run_foreground(line);
            break;
        case BACKGROUND:
            run_background(line);
            break;
        case PIPE:
            break;
        case REDIRECT:
            break;
        default:
            printf("This is not good\n");
            break;
    }
}

int execute_command(struct Command cmd){
    if (strcmp(cmd.command, builtin_functions[PWD]) == 0)
        pwd();
    else if (strcmp(cmd.command, builtin_functions[CD]) == 0)
        cd(cmd.argv[1]);
    else if(strcmp(cmd.command, builtin_functions[LS]) == 0)
        ls();
    else if (strcmp(cmd.command, builtin_functions[SORT]) == 0)
        sort(cmd.argv[1]);
    else if (strcmp(cmd.command, builtin_functions[HIST]) == 0)
        display_history();
    else if (strcmp(cmd.command, builtin_functions[LAST_CMD]) == 0)
        last_command();
    else{
        pid_t pid = fork();
        if (pid == 0){
            execvp(cmd.command, cmd.argv);
            exit(0);
        }
        return pid;
    } 
    return 0;
}

void run_foreground(char command[]){
    printf("Run in foreground\n");
    struct Command cmd = parse_command(command);
    execute_command(cmd);
    wait(NULL);
}

void run_background(char command[]){
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

    int child_pid = execute_command(cmd);
    add_child_id(child_pid);

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
        fprintf(stderr, "No child pid found\n");
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