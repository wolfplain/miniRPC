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
)

add_library(minirpc SHARED ${SOURCE_LIST})
target_link_libraries(minirpc PRIVATE ssl crypto)
target_link_libraries(minirpc PRIVATE rapidjson)

message("install " ${CMAKE_INSTALL_PREFIX})


# install headers for release
install(TARGETS minirpc
       RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
       LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
       ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

file(GLOB SERVER_HEADER ${SRC_DIR}/server/*.h)
file(GLOB CLIENT_HEADER ${SOURCE_DIR}/client/*.h)
file(GLOB COMMON_HEADER ${SRC_DIR}/common/*.h)
install(FILES ${SERVER_HEADER} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/server/)
install(FILES ${CLIENT_HEADER} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/client/)
install(FILES ${COMMON_HEADER} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/common/)