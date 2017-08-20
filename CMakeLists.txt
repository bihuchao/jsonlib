cmake_minimum_required(VERSION 3.11)

set(json_test_num 6)

ADD_SUBDIRECTORY(jsonlib)
ADD_SUBDIRECTORY(jsonlib_test)

make_directory(${CMAKE_BINARY_DIR}/test_jsons)
foreach(id RANGE 1 ${json_test_num})
	configure_file(${CMAKE_SOURCE_DIR}/test_jsons/test${id}.json ${CMAKE_BINARY_DIR}/test_jsons)
endforeach(id)
