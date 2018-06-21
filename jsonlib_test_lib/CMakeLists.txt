cmake_minimum_required(VERSION 3.11)

PROJECT(jsonlib_test_lib)
INCLUDE_DIRECTORIES(../jsonlib)
LINK_LIBRARIES(jsonlib_lib)
ADD_EXECUTABLE(jsonlib_test_lib jsonlib_test_lib.cpp)
