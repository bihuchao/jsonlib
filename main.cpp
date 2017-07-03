#include <cstdio>
#include <cstring>
#include "jsonlib.h"
#include "exception.h"

int main()
{
    // Parse From string
    const char *str = "{\"Name\":\"zhangsan\", \"Age\":12}";
    JSON *example1 = ParseFromStr(str);
    PrintJSON(example1);
    FreeJSON(example1);

    // parse from file
    for(int i=1; i<=6; i++){
        char filename[30];
        sprintf(filename, "../test/test%d.json", i);
        JSON *example = ParseFromFile(filename);
        PrintJSON(example);
        FreeJSON(example);
    }

    ShowWheatherMemoryLeak();

    return 0;
}