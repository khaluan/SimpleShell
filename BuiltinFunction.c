#include "BuiltinFunction.h"

const char* builtin_functions[BUILTIN_SIZE] = {
    "pwd", "cd", "ls", "sort", 
    "history", "exit", "!!"
};

void ls(){
    printf("This should be printed in ls\n");
    char current_directory[256];
    getcwd(current_directory, 256);

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (current_directory)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            printf("%s\n", ent->d_name);
        }
        closedir (dir);
    } else {
        fprintf(stderr, "%s", "ls error found \n");
        return;
    }
    return;
}

void pwd(){
    printf("This should be printed in pwd\n");
    char current_directory[256];
    getcwd(current_directory, 256);
    printf("%s\n", current_directory);
}

void cd(char* new_dir){
    printf("[cd] %s\n", new_dir);
    char s[100];
  
    // printing current working directory
    printf("%s\n", getcwd(s, 100));
  
    // using the command
    chdir(new_dir);
  
    // printing current working directory
    printf("%s\n", getcwd(s, 100));
  
}

void sort(char* filename){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr,  "File %s not found\n", filename);
        return;
    }
        
    size_t cnt = 0;
    while ((read = getline(&line, &len, fp)) != -1) 
        ++cnt;

    fclose(fp);
    
    char** lines;
    lines = malloc(cnt * sizeof(char*));

    fp = fopen(filename, "r");
    for (size_t i = 0; i < cnt; ++i){
        read = getline(&line, &len, fp);
        if (line[read- 1] == '\n') line[read-1] = '\0';
        lines[i] = strdup(line);
    }
    fclose(fp);

    qsort(lines, cnt, sizeof(char*), cmp_str);
    for (int i = 0; i < cnt; ++i){
        printf("%s\n", lines[i]);
        fflush(stdout);
        free(lines[i]);
    }
    free(lines);
    return;
}

void display_history(){
    for (int i = HISTORY_SIZE - 1; i >= 0; --i)
        if (history[i] != NULL) 
            printf("%s\n", history[i]);
}

void last_command(){
    return;
}