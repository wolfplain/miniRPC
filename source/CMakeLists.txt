project(miniRPC)
cmake_minimum_required(VERSION 3.22)
set(TOP_DIR ${CMAKE_CURRENT_LIST_DIR}/../)
set(SRC_DIR ${TOP_DIR}/source/)

include_directories(
    "${SRC_DIR}/"
)

file(GLOB SOURCE_LIST
    ${SRC_DIR}/server/*.cpp
    ${SRC_DIR}/common/*.cpp
    ${SRC_DIR}/event/*.cpp
    ${SRC_DIR}/rpc/*.cpp
)

add_library(minirpc SHARED ${SOURCE_LIST})
add_dependencies(minirpc rapidjson)
target_link_libraries(minirpc PRIVATE ssl crypto)
target_link_libraries(minirpc PUBLIC rapidjson)

add_executable(miniRPC ${SRC_DIR}/main.cpp)
target_link_libraries(miniRPC PRIVATE minirpc)

# install headers for release
install(TARGETS minirpc
       RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
       LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
       ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

file(GLOB SERVER_HEADER ${SRC_DIR}/server/*.h)
file(GLOB CLIENT_HEADER ${SOURCE_DIR}/client/*.h)
file(GLOB COMMON_HEADER ${SRC_DIR}/common/*.h)
file(GLOB RPC_HEADER ${SRC_DIR}/rpc/*.h)
file(GLOB EVENT_HEADER ${SRC_DIR}/event/*.h)
install(FILES ${SERVER_HEADER} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/server/)
install(FILES ${CLIENT_HEADER} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/client/)
install(FILES ${COMMON_HEADER} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/common/)
install(FILES ${EVENT_HEADER} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/event/)