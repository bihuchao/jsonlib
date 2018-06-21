#ifndef _JSONLIB_LIB_H
#define _JSONLIB_LIB_H

#include "jsonlib.h"

JSON *JSON_New();
void JSON_Delete(JSON *j);
const char *JSON_GetType(JSON *j);
JSON *ParseFromStr(const char *str);
JSON *ParseFromFile(const char *filename);
void PrintJSON(JSON *j);
void FreeJSON(JSON *j);
void ShowWheatherMemoryLeak();

#endif