#include <stdio.h>
#include <string.h>
#include "jsonlib.h"

int main(int argc, char** argv) {
    // parse from string
    const char* str = "{\"Name\":\"zhangsan\", \"Age\":12}";
    jsonlib::JSON* example1 = jsonlib::ParseFromStr(str);
    jsonlib::PrintJSON(example1);
    jsonlib::FreeJSON(example1);

    // parse from file
    for(int i = 1; i <= 6; i++){
        char filename[30] = {'\0'};
        sprintf(filename, "../test_datas/test%d.json", i);
        jsonlib::JSON* example = jsonlib::ParseFromFile(filename);
        jsonlib::PrintJSON(example);
        jsonlib::FreeJSON(example);
    }

    // judge memory leak
    jsonlib::ShowWheatherMemoryLeak();

    return 0;
}
