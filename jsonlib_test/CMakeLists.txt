cmake_minimum_required(VERSION 3.11)

PROJECT(jsonlib_test)
INCLUDE_DIRECTORIES(../jsonlib)
LINK_LIBRARIES(jsonlib)
ADD_EXECUTABLE(jsonlib_test jsonlib_test.cpp)
