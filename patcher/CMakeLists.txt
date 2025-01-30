cmake_minimum_required(VERSION 3.31)

project("LunacyPatcher")
set(LIBRARY_NAME "crack")

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB SRC_FILES ${SRC_PATH}/*.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -std=c++20 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -std=c++20 -DNDEBUG")

set(DOBBY_HOME ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}/libdobby.a)

add_library(
    ${LIBRARY_NAME}
    SHARED
    ${SRC_FILES}
)

target_include_directories(
    ${LIBRARY_NAME}
    PUBLIC
    ${SRC_PATH}
)

target_link_libraries(
    ${LIBRARY_NAME}
    android
    log
    ${DOBBY_HOME}
)