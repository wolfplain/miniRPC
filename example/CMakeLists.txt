project(miniRPC-example)
cmake_minimum_required(VERSION 3.22)

set(TOP_DIR ${CMAKE_CURRENT_LIST_DIR}/../)

# user can install rapidjson to system
include(${TOP_DIR}/thirdparty/rapidjson/CMakeLists.txt)  

# set the path to the miniRPC headers directory
set(miniRPC ${TOP_DIR}/build/miniRPC)

include_directories(
    ${TOP_DIR}/example/
)

file (GLOB SRCS *.cpp)

add_library(mathservice SHARED ${SRCS})
target_include_directories(mathservice PRIVATE ${miniRPC}/include)
target_link_libraries(mathservice PRIVATE ${miniRPC}/lib/libminirpc.so)
target_link_libraries(mathservice PRIVATE rapidjson)


add_executable(example main.cpp)
target_include_directories(example PRIVATE ${miniRPC}/include)
target_link_libraries(example PRIVATE ${miniRPC}/lib/libminirpc.so)
target_link_libraries(example PRIVATE rapidjson mathservice)