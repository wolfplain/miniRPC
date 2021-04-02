project(miniRPC)
cmake_minimum_required(VERSION 2.8)
set(TOP_DIR ${CMAKE_CURRENT_LIST_DIR}/../)
set(TESTS_DIR ${TOP_DIR}/tests)

include_directories(
    ${TESTS_DIR}/
)

file(GLOB TESTS_LIST
    ${TESTS_DIR}/*.cpp
    )

add_executable(ut-minirpc ${SOURCE_LIST})
target_link_libraries(minirpc PRIVATE minirpc)
install(TARGETS ut-minirpc DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)