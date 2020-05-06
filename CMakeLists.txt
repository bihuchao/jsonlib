cmake_minimum_required(VERSION 3.10.0)
project(jsonlib)

# jsonlib
file(GLOB json_lib_srcs jsonlib/*.cpp jsonlib/*.h)
add_library(jsonlib ${json_lib_srcs})
target_include_directories(jsonlib
	INTERFACE jsonlib
)

# examples
add_executable(parse_json example/parse_json.cpp)
target_link_libraries(parse_json
	PUBLIC jsonlib
)

# test_datas
make_directory(${CMAKE_BINARY_DIR}/test_datas)
file(GLOB test_datas test_datas/*.json)
foreach(test_data ${test_datas})
	configure_file(${test_data} ${CMAKE_BINARY_DIR}/test_datas)
endforeach(test_data)
