#include "BuiltinFunction.h"

const char* builtin_functions[BUILTIN_SIZE] = {
    "pwd ", "cd", "ls", "sort", 
    "history", "exit", "!!"
};

void ls(){
    char current_directory[256];
    getcwd(current_directory, 256);

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (current_directory)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            printf ("%s\n", ent->d_name);
        }
        closedir (dir);
    } else {
        fprintf(stderr, "%s", "ls error found \n");
        return;
    }
    return;
}

void pwd(){
    char current_directory[256];
    getcwd(current_directory, 256);
    printf("%s\n", current_directory);
}

void cd(char* new_dir){
    chdir(new_dir);
}
