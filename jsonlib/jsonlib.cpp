#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "logging.h"
#include "jsonlib.h"

namespace jsonlib {

// debug for memory leak
int malloc_num = 0;
int free_num = 0;

// static
static char* ReadStrFromFile(const char* filename);
static const char* Skip(const char* str);
static const char* ParseObject(JSON* j, const char* str);
static const char* ParseValue(JSON* j, const char* str);
static const char* ParseString(JSON* j, const char* str);
static const char* ParseNumber(JSON* j, const char* str);
static const char* ParseArray(JSON* j, const char* str);
static void PrintIndent(int n, int level);
static void PrintJSONSub(JSON* j, int level);

// static variables
static const char* JSON_TYPE_NAME[] = {
    "NULL",
    "BOOLEAN",
    "INT",
    "DOUBLE",
    "STRING",
    "ARRAY",
    "OBJECT"
};

// static function
// read string from file
static char* ReadStrFromFile(const char* filename) {
    if (filename==nullptr || *filename=='\0') {
        LOG_FATAL("Filename can't be null");
    }
    FILE* fp = nullptr;
    if ((fp = fopen(filename, "r")) == nullptr) {
        char message[20] = {'\0'};
        sprintf(message, "File %s open fail!", filename);
        LOG_FATAL(message);
    }
    int len = 0;
    char temp;
    while (fread(&temp, 1, 1, fp)) {
        ++len;
    }
    rewind(fp);
    char* str = nullptr;
    if ((str = (char *)malloc(sizeof(char) * (len + 1))) == nullptr) {
        fclose(fp);
        LOG_FATAL("String malloc fail!");
    }
    malloc_num += (len + 1);
    for(int i = 0; i < len; ++i) {
        fread(str+i, 1, 1, fp);
    }
    str[len] = '\0';
    fclose(fp);
    return str;
}

// print indent
static void PrintIndent(int n, int level) {
    for(int i = 0; i< n * level; ++i) {
        printf(" ");
    }
}

// skip control symbol in ASCII ( [0, 32] and 127)
static const char* Skip(const char* str) {
    if (str == nullptr) {
        LOG_WARNING("Input string is nullptr");
        return nullptr;
    }
    while (*str && ((unsigned char)*str <= 32 || (unsigned char)(*str) == 127)) {
        str++;
    }
    return str;
}

// parse driver : all type
static const char* ParseValue(JSON* j, const char* str) {
    if (!str) {
        LOG_WARNING("Input string is nullptr");
        return nullptr;
    }
    if (!j) {
        LOG_WARNING("Input json is nullptr");
        return str;
    }
    str = Skip(str);
    if (!strncmp(str, "null", 4)) {
        j->type = JSON_NULL;
        return str + 4;
    }
    if (!strncmp(str, "false", 5)) {
        j->type = JSON_BOOLEAN;
        j->boolValue = false;
        return str + 5;
    }
    if (!strncmp(str, "true", 4)) {
        j->type = JSON_BOOLEAN;
        j->boolValue = true;
        return str + 4;
    }
    if (*str == '\"') {
        return ParseString(j, str);
    }
    if ((*str >= '0' && *str <= '9') || (*str == '-') || (*str == '.')) {
        return ParseNumber(j, str);
    }
    if (*str == '[') {
        return ParseArray(j, str);
    }
    if (*str == '{') {
        return ParseObject(j, str);
    }
    return str;
}

// parse object
static const char* ParseObject(JSON* j, const char* str) {
    if (!str) {
        LOG_WARNING("Input string is nullptr");
        return nullptr;
    }
    if (!j) {
        LOG_WARNING("Input json is nullptr");
        return str;
    }

    str = Skip(str);
    j->type = JSON_OBJECT;
    if (*str != '{') {
        LOG_FATAL("Illegal string -> not start with { !");
    }
    str = Skip(str + 1);


    bool isFirst = true;
    while (true) {
        if (*str == '\0') {
            LOG_FATAL("Illegal string -> occur \\0 brfore } !");
        } else if (*str == '}') {
            if (isFirst) {
                j->objectPointer = nullptr;
            } else {
                j->next = nullptr;
            }
            break;
        } else if (*str == '\"') {
            if (isFirst) {
                isFirst = false;
                j->objectPointer = JSON_New();
                j = j->objectPointer;
            }
            else {
                j->next = JSON_New();
                j = j->next;
            }

            // parse key ( only string)
            str = Skip(ParseString(j, Skip(str)));
            j->objectName = j->stringValue;
            j->stringValue = nullptr;
            if (*str != ':') {
                LOG_FATAL("Illegal string -> do not have : !");
            }
            else {
                str = Skip(str + 1);
            }
            // parse value
            str = Skip(ParseValue(j, Skip(str)));
        } else if (*str == ',' && !isFirst) {
            str = Skip(str + 1);
        } else {
            LOG_FATAL("Illegal string -> other mistakes !");
        }
    }

    return str + 1;
}

// parse string
static const char* ParseString(JSON* j, const char* str) {
    if (!str) {
        LOG_WARNING("Input string is nullptr");
        return nullptr;
    }
    if (!j) {
        LOG_WARNING("Input json is nullptr");
        return str;
    }
    str = Skip(str);
    if (*str != '\"') {
        LOG_FATAL("Illegal string -> not start with \" !");
    }
    int len = 0;
    const char* temp = str + 1;
    while (*temp != '\"') {
        if (*temp == '\0') {
            LOG_FATAL("Illegal string -> occur \\0 before \"  !");
        }
        ++len;
        ++temp;
    }
    char* result = nullptr;
    if ( (result = (char*)malloc(len + 1)) == nullptr) {
        LOG_FATAL("Result malloc fail !");
    }
    malloc_num += (len + 1);
    temp = str + 1;
    char* temp2 = result;
    while (*temp != '\"' && *temp) {
        *temp2++ = *temp++;
    }
    *temp2 = '\0';

    ++temp;
    j->type = JSON_STRING;
    j->stringValue = result;

    return temp;
}

// parse number
static const char *ParseNumber(JSON* j, const char* str) {
    if (!str) {
        LOG_WARNING("Input string is nullptr");
        return nullptr;
    }
    if (!j) {
        LOG_WARNING("Input json is nullptr");
        return str;
    }
    str = Skip(str);
    if (*str >= '0' && *str <= '9' || *str == '-' || *str == '.') {
        ;
    } else {
        LOG_FATAL("Illegal string -> start with wrong !");
    }

    // judge +/-
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        ++str;
    }
    // judge number
    int integerPart = 0, decimalPart = 0, decimalDigits=0;
    bool onlyHasDot = true;
    bool hasDot = false;
    while (true) {
        if (!hasDot) {
            if (*str <= '9' && *str >='0') {
                integerPart = integerPart * 10 + (*str - '0');
                onlyHasDot = false;
            } else if (*str == '.') {
                hasDot = true;
            } else {
                if (onlyHasDot) {
                    LOG_FATAL("Illegal string ->  only have - !");
                }
                break;
            }
            ++str;
        } else {
            if (*str <= '9' && *str >= '0') {
                decimalPart = decimalPart * 10 + (*str - '0');
                ++decimalDigits;
                onlyHasDot = false;
            } else {
                if (onlyHasDot) {
                    LOG_FATAL("Illegal string ->  segment only has dot !"); // 只有点
                }
                break;
            }
            ++str;
        }
    }

    if (hasDot) {
        j->type = JSON_DOUBLE;
        j->doubleValue=integerPart+pow(10., -decimalDigits)*decimalPart;
        j->doubleValue *= sign;
    } else {
        j->type = JSON_INT;
        j->intValue=integerPart;
        j->intValue *= sign;
    }
    return str;
}

// parse array
static const char* ParseArray(JSON* j, const char* str) {
    if (!str) {
        LOG_WARNING("Input string is nullptr");
        return nullptr;
    }
    if (!j) {
        LOG_WARNING("Input json is nullptr");
        return str;
    }
    str = Skip(str);
    if (*str != '[') {
        LOG_FATAL("Illegal string ->  start wrong !");
    }

    str = Skip(str + 1);
    j->type = JSON_ARRAY;

    bool isFirst = true;
    bool isComma = false;

    while (true) {
        if (*str == '\0') {
            LOG_FATAL("Illegal string -> format wrong !");
        } else if (*str == ']') {
            if (isFirst) {
                j->arrayPointer = nullptr;
            } else{
                j->next = nullptr;
            }
            break;
        } else if (*str == ',') {
            if (!isFirst && !isComma) {
                isComma = true;
                ++str;
            }
            else {
                LOG_FATAL("Illegal string -> , !");// 连续两个逗号 或者第一个就是逗号
            }
        } else {
            if (isFirst) {
                j->arrayPointer = JSON_New();
                j = j->arrayPointer;
                isFirst = false;
            } else {
                j->next = JSON_New();
                j = j->next;
                isComma = false;
            }
            str = Skip(ParseValue(j, Skip(str)));
        }
    }
    return str + 1;
}

// sub function of print json
static void PrintJSONSub(JSON* j, int level) {
    if (j->type == JSON_NULL) {
        printf("NULL%s\n", j->next ? "," : "");
    } else if (j->type == JSON_BOOLEAN) {
        printf("%s%s\n", j->boolValue ? "true" : "false", j->next ? "," : "");
    } else if (j->type == JSON_INT) {
        printf("%d%s\n", j->intValue, j->next ? "," : "");
    } else if (j->type == JSON_DOUBLE) {
        printf("%lf%s\n", j->doubleValue, j->next ? "," : "");
    } else if (j->type == JSON_STRING) {
        printf("\"%s\"%s\n", j->stringValue, j->next ? "," : "");
    } else if (j->type == JSON_ARRAY) {
        printf("[\n");
        j = j->arrayPointer;
        while (j) {
            PrintIndent(4, level);
            PrintJSONSub(j, level + 1);
            j = j->next;
        }
        PrintIndent(4, level - 1);
        printf("]\n");
    } else if (j->type == JSON_OBJECT) {
        printf("{\n");
        j = j->objectPointer;
        while (j) {
            PrintIndent(4, level);
            printf("\"%s\" : ", j->objectName);
            PrintJSONSub(j, level + 1);
            j = j->next;
        }
        PrintIndent(4, level-1);
        printf("}\n");
    }
}

// extern function
// parse json object from string
JSON* ParseFromStr(const char* str) {
    if (!str) {
        LOG_WARNING("Input string is nullptr !");
        return nullptr;
    }
    str = Skip(str);
    if (*str == '\0') { return nullptr; }

    if (*str == '{') {
        JSON* j = JSON_New();
        str = ParseObject(j, str);
        str = Skip(str);
        if (*str != '\0') {
            LOG_FATAL("The string is not end!");
        }
        return j;
    } else {
        LOG_FATAL("The first char is not { !");
    }
    return nullptr;
}

// parse json object from file
JSON* ParseFromFile(const char* filename) {
    char* str = ReadStrFromFile(filename);
    JSON* j = ParseFromStr(str);

    free_num += (strlen(str) + 1);
    free(str);

    return j;
}

// print json object
void PrintJSON(JSON* j) {
    if (!j) {
        LOG_WARNING("Input json is nullptr !");
    }
    PrintJSONSub(j, 1);
}

// constructor
JSON* JSON_New() {
    JSON* j = nullptr;
    if ((j = (JSON*)malloc(sizeof(JSON))) == nullptr) {
        LOG_FATAL("JSON malloc fail");
    }
    malloc_num += sizeof(JSON);
    memset(j, 0, sizeof(JSON));

    return j;
}

// destructor
void JSON_Delete(JSON* j) {
    if (j) {
        if (j->stringValue) {
            free_num += (strlen(j->stringValue) + 1);
            free(j->stringValue);
        }
        if (j->objectName) {
            free_num += (strlen(j->objectName) + 1);
            free(j->objectName);
        }
        free_num += sizeof(JSON);
        free(j);
    }
}

// get json node type
const char* JSON_GetType(JSON* j) {
    if (j == nullptr) {
        LOG_WARNING("Input json is nullptr !");
    }
    return JSON_TYPE_NAME[j->type];
}

// free json
void FreeJSON(JSON* j) {
    if (j == nullptr) {
        LOG_WARNING("Input json is nullptr !");
        return ;
    }
    if (j->type == JSON_ARRAY) {
        JSON* temp = j;
        j = j->arrayPointer;
        JSON_Delete(temp);
        while (j) {
            temp = j;
            j = j->next;
            FreeJSON(temp);
        }
    } else if (j->type == JSON_OBJECT) {
        JSON* temp = j;
        j = j->objectPointer;
        JSON_Delete(temp);
        while (j) {
            temp = j;
            j = j->next;
            FreeJSON(temp);
        }
    } else{
        JSON_Delete(j);
    }
}

// show wheather have memory leak
void ShowWheatherMemoryLeak() {
    printf("Malloc: %d\nFree  : %d\nisOK: %s\n", malloc_num, free_num,
        (malloc_num == free_num) ? "Yes" : "No");
}

}; // namespace jsonlib
