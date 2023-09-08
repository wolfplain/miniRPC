project(miniRPC)
cmake_minimum_required(VERSION 3.22)
set(TOP_DIR ${CMAKE_CURRENT_LIST_DIR}/../)
set(TESTS_DIR ${TOP_DIR}/tests/)

include_directories(
    ${TESTS_DIR}
    ${TOP_DIR}/source/
)

file(GLOB TESTS_SOURCE_LIST
    ${TESTS_DIR}/*.cpp
    ${TESTS_DIR}/server/*.cpp
    ${TESTS_DIR}/log/*.cpp
)

add_executable(ut-miniRPC ${TESTS_SOURCE_LIST})
target_link_libraries(ut-miniRPC PRIVATE minirpc)
target_link_libraries(ut-miniRPC PRIVATE gtest)


install(TARGETS ut-miniRPC DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)