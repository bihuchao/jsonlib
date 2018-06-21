cmake_minimum_required(VERSION 3.11)

PROJECT(jsonlib_lib)
ADD_LIBRARY(jsonlib_lib STATIC jsonlib.cpp jsonlib_lib.h jsonlib.h exception.cpp exception.h)

PROJECT(jsonlib_dll)
ADD_LIBRARY(jsonlib_dll SHARED jsonlib.cpp jsonlib_lib.h jsonlib_dll.cpp jsonlib.h exception.cpp exception.h )