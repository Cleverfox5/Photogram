cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME Pthotogram_1)
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
set(Boost_ROOT ${LIB_PLACE}/boost/boost_1_89_0)
find_package(Boost 1.89 REQUIRED)
link_directories(${Boost_LIBRARY_DIRS})


#libpqxx
set(libpqxx_DIR "${LIB_PLACE}/libpqxx")
list(APPEND CMAKE_PREFIX_PATH "${libpqxx_DIR}/lib/cmake")
find_package(libpqxx REQUIRED)

#libsodium
set(libsodium_DIR "${LIB_PLACE}/libsodium")
set(SODIUM_LIB_DIR "${libsodium_DIR}/x64/Release/v143/static")
set(SODIUM_LIB ${SODIUM_LIB_DIR}/libsodium.lib)

set(first_server_src_dir ${CMAKE_SOURCE_DIR}/data)

#src
file(GLOB MAIN_SRC              ${CMAKE_SOURCE_DIR}/*.cpp
                                ${CMAKE_SOURCE_DIR}/*.h
)



file(GLOB ALL_FIRST_SERVER_SRC ${first_server_src_dir}/Request.cpp
                               ${first_server_src_dir}/Request.h
                               ${first_server_src_dir}/FormData.cpp
                               ${first_server_src_dir}/FormData.h
                               ${first_server_src_dir}/HashTools.cpp
                               ${first_server_src_dir}/HashTools.h
                               ${first_server_src_dir}/JwtToken.cpp
                               ${first_server_src_dir}/JwtToken.h
                               ${first_server_src_dir}/WorkWithPostgreSQL.cpp
                               ${first_server_src_dir}/WorkWithPostgreSQL.h
)

source_group(TREE ${first_server_src_dir} 
    PREFIX "src"
    FILES ${ALL_FIRST_SERVER_SRC}
)

add_executable(${TARGET_NAME} ${MAIN_SRC} ${ALL_FIRST_SERVER_SRC})

#include surs directories
target_include_directories(${TARGET_NAME} PRIVATE ${first_server_src_dir})

#include lib directories
target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE "${libpqxx_DIR}/include")
target_include_directories(${TARGET_NAME} PRIVATE "${libsodium_DIR}/include")

target_link_libraries(
    ${TARGET_NAME} 
    PRIVATE 
    nlohmann_json::nlohmann_json
    jwt-cpp::jwt-cpp
    libpqxx::pqxx
    ${SODIUM_LIB}
)

#settings for VS
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
target_compile_definitions(${TARGET_NAME} PRIVATE SODIUM_STATIC)
