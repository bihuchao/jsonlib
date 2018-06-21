cmake_minimum_required(VERSION 3.11)

PROJECT(jsonlib_test_dll)

INCLUDE_DIRECTORIES(../jsonlib)

ADD_EXECUTABLE(jsonlib_test_dll jsonlib_test_dll.cpp)

add_dependencies(jsonlib_test_dll jsonlib_dll)
