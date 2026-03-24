cmake_minimum_required(VERSION 3.10.0)
project(in-memory-sql-db VERSION 0.1.0 LANGUAGES C CXX)

add_executable(in-memory-sql-db src/main.cpp)

target_include_directories(in-memory-sql-db PRIVATE src/myfunctions.h)

