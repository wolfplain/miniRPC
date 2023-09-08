project(miniRPC)
cmake_minimum_required(VERSION 3.22)

add_subdirectory(rapidjson)

if(TEST_ON)
add_subdirectory(googletests)
endif()