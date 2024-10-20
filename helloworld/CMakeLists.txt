cmake_minimum_required(VERSION 3.10)

project(helloworld)
find_package(fmt CONFIG REQUIRED)
set(CMAKE_C_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.41.34120/bin/Hostx64/x64/cl.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.41.34120/bin/Hostx64/x64/cl.exe")

add_subdirectory(fmt)

add_executable(helloworld helloworld.cpp)

target_link_libraries(helloworld PRIVATE fmt::fmt)