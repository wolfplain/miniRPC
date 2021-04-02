#!/bin/bash
set -e
WORK_PATH=$(pwd)
BUILD_PATH=${WORK_PATH}/build
SOURCE_BUILD_PATH=${BUILD_PATH}/source
TESTS_BUILD_PATH=${BUILD_PATH}/tests

build_source()
{
    mkdir -pv ${SOURCE_BUILD_PATH}
    cd ${SOURCE_BUILD_PATH}
    cmake ${WORK_PATH}
    make -j
    make install
}

build_tests()
{
    mkdir -pv ${TESTS_BUILD_PATH}
    cd ${TESTS_BUILD_PATH}
    CMAKE_TEST_ARG= "-DTEST_ON=1"
    cmake ${CMAKE_TEST_ARG} ${WORK_PATH}
    make -j
}

build_source
build_tests