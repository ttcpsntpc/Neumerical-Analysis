# 1. 設置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 2. 設置專案名稱，並指定語言為 C
project(Language CXX)

set(OpenCV_ROOT "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Language 
    src/opencv.cpp    
)

target_include_directories(Language PUBLIC include)

target_link_libraries(Language PRIVATE ${OpenCV_LIBS})