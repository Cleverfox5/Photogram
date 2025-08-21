cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME Pthotogram)
project(${TARGET_NAME} VERSION 	1.0.0)

set(CMAKE_CXX_STANDARD 17)

if(NOT WIN32)
    message(FATAL_ERROR "Don't support Linux")
endif()

set(DEV_ROOT c:/dev)
set(LIB_PLACE ${DEV_ROOT}/common)

set(nlohmann_json_DIR ${LIB_PLACE}/json/build)
find_package(nlohmann_json 3.12.0 REQUIRED)

find_package(jwt-cpp CONFIG REQUIRED)


file(GLOB MAIN_SRC main.cpp)

source_group("src" FILES ${MAIN_SRC})

add_executable(${TARGET_NAME} ${MAIN_SRC})

target_link_libraries(
    ${TARGET_NAME} 
    PRIVATE 
    nlohmann_json::nlohmann_json
    jwt-cpp::jwt-cpp
)


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
