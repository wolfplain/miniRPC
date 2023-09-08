project(miniRPC)
cmake_minimum_required(VERSION 3.22)
set(TOP_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -Werror -Wall -fPIC -fPIE)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Werror)  # -g   -Wall -fPIC -fPIE
add_compile_options(-g) #  -Wall -Wall -fPIC -fPIE

if(TEST_ON)
    set(CMAKE_INSTALL_PREFIX ${TOP_DIR}/build/miniRPC/tests/)
    add_subdirectory(tests)
else()
    set(CMAKE_INSTALL_PREFIX ${TOP_DIR}/build/miniRPC/)
endif()

add_subdirectory(thirdparty)
add_subdirectory(source)

