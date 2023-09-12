project(miniRPC)
cmake_minimum_required(VERSION 3.22)
set(TOP_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -Werror -Wall -fPIC -fPIE)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Werror)  # -g   -Wall -fPIC -fPIE
add_compile_options(-g) #  -Wall -Wall -fPIC -fPIE

set(CMAKE_INSTALL_PREFIX ${TOP_DIR}/build/miniRPC/)

add_subdirectory(thirdparty)
add_subdirectory(source)
if(TEST_ON)
    add_subdirectory(tests)
endif()
