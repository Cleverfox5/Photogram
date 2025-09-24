cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME Pthotogram)
project(${TARGET_NAME} VERSION 	1.0.0)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0167 OLD)

if(NOT WIN32)
    message(FATAL_ERROR "Don't support Linux")
endif()

#set paths
set(DEV_ROOT c:/dev)
set(LIB_PLACE ${DEV_ROOT}/common)

#nlohmann_json
set(nlohmann_json_DIR ${LIB_PLACE}/json/build)
find_package(nlohmann_json 3.12.0 REQUIRED)

#jwt-cpp
find_package(jwt-cpp CONFIG REQUIRED)

#Boost
set(Boost_ROOT "c:/Program Files/boost/boost_1_89_0")
find_package(Boost 1.89 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#if (Boost_FOUND)
#    message(STATUS "Location: ${Boost_LIBRARY_DIRS}")

#src
file(GLOB MAIN_SRC main.cpp)

source_group("src" FILES ${MAIN_SRC})

add_executable(${TARGET_NAME} ${MAIN_SRC})

target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

#link
target_link_libraries(
    ${TARGET_NAME} 
    PRIVATE 
    nlohmann_json::nlohmann_json
    jwt-cpp::jwt-cpp
)

#settings for VS
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
