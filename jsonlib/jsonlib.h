#ifndef _JSONLIB_H
#define _JSONLIB_H

typedef enum JSON_TYPE{
    JSON_NULL = 0,
    JSON_BOOLEAN = 1,
    JSON_INT = 2,
    JSON_DOUBLE = 3,
    JSON_STRING = 4,
    JSON_ARRAY = 5,
    JSON_OBJECT = 6
}JSON_TYPE;

typedef struct JSON{
    JSON_TYPE type; // null / boolean / int / double / string / array / object

    bool boolValue;
    int intValue;
    double doubleValue;
    char *stringValue;

    char *objectName;

    struct JSON *arrayPointer, *objectPointer;
    struct JSON *next;
}JSON;

#endif
