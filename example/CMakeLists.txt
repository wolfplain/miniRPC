project(miniRPC-example)
cmake_minimum_required(VERSION 3.22)

set(TOP_DIR ${CMAKE_CURRENT_LIST_DIR}/../)

# user can install rapidjson to system
include(${TOP_DIR}/thirdparty/rapidjson/CMakeLists.txt)  

# set the path to the miniRPC headers directory
set(miniRPC ${TOP_DIR}/build/miniRPC)

file (GLOB SRCS *.cpp)

add_executable(example ${SRCS})
target_include_directories(example PRIVATE ${miniRPC}/include)
target_link_libraries(example PRIVATE ${miniRPC}/lib/libminirpc.so)
target_link_libraries(example PRIVATE rapidjson)
