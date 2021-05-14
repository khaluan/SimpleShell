#include "Utility.h"
char* str_replace(char* string, const char* find, const char* replace){
    char* tmp = string;
    char buffer[MAX_BUFFER];
    char small_buffer[MAX_BUFFER];
    char* pos;
    while ((pos = strstr(tmp, find))){
        strncpy(small_buffer, tmp, pos - tmp);
        strcat(buffer, small_buffer);
        strcat(buffer, replace);
        tmp = pos + strlen(find);
    }
    strcat(buffer, tmp);
    return strdup(buffer);
}

int cmp_str(const void* a, const void* b){
    return strcmp(*(char**)a, *(char**)b);
}