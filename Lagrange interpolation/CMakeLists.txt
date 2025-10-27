# 1. 設置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 2. 設置專案名稱，並指定語言為 C
project(Language C)

add_executable(Language 
    src/main.c
)

target_include_directories(Language PUBLIC include)