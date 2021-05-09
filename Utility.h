#ifndef UTILITY_H_
#define UTILITY_H_
#define MAX_BUFFER 512
#define BUILTIN_SIZE 7

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

struct Command {
    char* command;
    char* argv[20];
};

#endif