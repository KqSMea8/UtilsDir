CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(zet_schema1)

FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
  SET(DOXYGEN_INPUT "${PROJECT_NAME}.doxyfile")
  SET(DOXYGEN_OUTPUT "html")
  ADD_CUSTOM_COMMAND(
    OUTPUT ${DOXYGEN_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${DOXYGEN_INPUT}
  )
  ADD_CUSTOM_TARGET(docs ALL DEPENDS ${DOXYGEN_OUTPUT})
ENDIF(DOXYGEN_FOUND)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wfatal-errors")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

### zet_schema1 library

ADD_LIBRARY(zet_schema1 STATIC zet_schema1.cpp zet_schema1.h zet_struct1.h)

### zet_structor compiler

ADD_EXECUTABLE(zet_structor zet_structor.cpp zet_structor.txt)
TARGET_LINK_LIBRARIES(zet_structor zet_schema1)

### google test

SET(gtest_force_shared_crt ON CACHE BOOL "Multithreaded DLL")
ADD_SUBDIRECTORY(gtest-1.7.0)
ENABLE_TESTING()
INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

### test zet_schema1

ADD_EXECUTABLE(test_zet_schema1 test_zet_schema1.cpp)
TARGET_LINK_LIBRARIES(test_zet_schema1 zet_schema1 gtest gtest_main)
ADD_TEST(test_zet_schema1 test_zet_schema1)
