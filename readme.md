# jsonlib
## Introduction:
[JSON](http://json.org/ ), also known as JavaScript Object Notation, is a lightweight data-interchange format. It is easy for humans to read and write. At the same time, it is easy for machines to parse and generate. There is a detailed data format in [RFC7159](https://tools.ietf.org/html/rfc7159 ).

[jsonlib](http://github.com/bihuchao/jsonlib ) is simpy library in C for people to use json in C language.
    
## Usage:
### Static Link Library
To use this static link library in your project, just add simply include statement in front of your code and link generated **lib** file to your project.

```C++
#include "jsonlib_lib.h"
```

### Dynamic Link Library
For dynamic link library, you should add another include statement in front of your code and place generated **dll** file to your project.

```C++
#include "jsonlib_dll.h"
```

Just enjoy it!

## Example:
To test with examples in ./test_jsons/, you can run:

```bash        
mkdir build && cd build
cmake ../ -G "{your_prefer_compiler}"
make
./{path_to_exe}.exe
```

## Bug
If you find some bugs, please email to bihuchao@qq.com.
    
-EOF-