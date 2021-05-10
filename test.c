#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char ** history;
int historySize = 10;

void init(){
    history = (char **) malloc(historySize * sizeof(char *));

    for (int i = 0; i < historySize; ++i) {
        history[i] = NULL;
    }

}

void push_history(char * newHistory) {

    for (int i = historySize - 1; i > 0; --i) {
        if (history[i - 1] == NULL) continue;
        else if (history[i] != NULL) free(history[i]);

        history[i] = history[i - 1];
    }
    history[0] = newHistory;
}

void clean(){
    for (int i = 0; i < historySize; ++i) {
        if (history[i] != NULL) free(history[i]);
    }

    free(history);
}

char * take_an_input() {
    int count = 0, bufferSize = 64;
    char buffer[bufferSize + 1], *res = NULL;

    fflush(stdin);
    fgets(buffer, bufferSize , stdin);

    for (count = 0; count < bufferSize && buffer[count] != '\0'; ++count);

    if (count > bufferSize) {
        printf("Input buffer does not have terminating null !");
        exit(0);
    }

    res = (char *) malloc(count * sizeof(char));

    for (int i = 0; i <= count; ++i) {
        res[i] = buffer[i];
    }    
    
    return res;
}

int string_compare(char * s1, char * s2) {
    if (sizeof(*s1) != sizeof(*s2)) return 0;

    for (int i = 0; s1[i] != '\0' && s2[i] != '\0'; ++i) {
        if (s1[i] != s2[i]) return 0; 
    }

    return 1;
}

char * read_input() {
        char * input = take_an_input();

        if (string_compare(input, "!!") == 1) {
           if (history[0] == NULL) {
                printf("No commands in history ! \n");
            }
            else { 
                input = history[0];
                printf("nhantest> ");
                printf("%s", input);
            }
            free(input);
        }
        else {
            push_history(input);

        }
        
        return input;
}

int main() {

    char * input = NULL;
    init();
    
    //printf("I am in line %d! ", __LINE__);

    while (1) {
        printf("nhantest> ");
        
        input = read_input();

        //sample input

        if (string_compare(input, "exit") == 1) {
            break;
        }
        else if (string_compare(input, "history") == 1) {
            for (int i = historySize - 1; i >= 0; --i) {
                if (history[i] != NULL) printf("%s", history[i]);
            }
        }

    }
    
    clean();

    return 0;
}
