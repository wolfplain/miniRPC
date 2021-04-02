project(miniRPC)
cmake_minimum_required(VERSION 2.8)
set(TOP_DIR ${CMAKE_CURRENT_LIST_DIR}/../)
set(SOURCE_DIR ${TOP_DIR}/source)

include_directories(
    ${SOURCE_DIR}/
)

file(GLOB SOURCE_LIST
    ${SOURCE_DIR}/server/*.cpp
    ${SOURCE_DIR}/client/*.cpp
    ${SOURCE_DIR}/common/*.cpp
    )

add_library(minirpc SHARED ${SOURCE_LIST})
target_link_libraries(minirpc PRIVATE ssl crypto)

install(TARGETS minirpc
       RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
       LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
       ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

file(GLOB SERVER_HEADER ${SOURCE_DIR}/server/*.h)
file(GLOB CLIENT_HEADER ${SOURCE_DIR}/client/*.h)
file(GLOB COMMON_HEADER ${SOURCE_DIR}/common/*.h)
install(FILES ${SERVER_HEADER} DESTINATION “/include/server/”)
install(FILES ${SERVER_HEADER} DESTINATION “/include/client/”)
install(FILES ${SERVER_HEADER} DESTINATION “/include/common/”)