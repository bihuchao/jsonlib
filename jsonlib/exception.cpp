#include "exception.h"
#include <cstdio>
#include <cstdlib>

// Error
void RaiseError(const char *func, const char *info){
    printf("[  Error  ] : %s - %s\n", func, info);
    exit(1);
}

// Warning 
void RaiseWarning(const char *func, const char *info){
    printf("[ Warning ] : %s - %s\n", func, info);
}