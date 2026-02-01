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
link_directories(${Boost_LIBRARY_DIRS})

#src
file(GLOB MAIN_SRC second_server_src/*.cpp
                   second_server_src/*.h
)

source_group("src" FILES ${MAIN_SRC})

add_executable(${TARGET_NAME} ${MAIN_SRC})

target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

set(libpqxx_DIR "${LIB_PLACE}/libpqxx")
list(APPEND CMAKE_PREFIX_PATH "${libpqxx_DIR}/7.5.3_x64_msvc-14.1/lib/cmake")

find_package(libpqxx REQUIRED)

#link
target_link_libraries(
    ${TARGET_NAME} 
    PRIVATE 
    nlohmann_json::nlohmann_json
    jwt-cpp::jwt-cpp
    libpqxx::pqxx
)

target_include_directories(${TARGET_NAME} PRIVATE "${libpqxx_DIR}/7.5.3_x64_msvc-14.1/include")

#settings for VS
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
