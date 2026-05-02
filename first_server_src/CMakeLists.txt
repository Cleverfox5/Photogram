cmake_minimum_required(VERSION 3.20)

set(TARGET_NAME Pthotogram_1)
project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0167 OLD)

#set paths
if (WIN32)
    set(DEV_ROOT c:/dev)
elseif(UNIX)
    set(DEV_ROOT /usr/lib)
else()
    message(FATAL_ERROR "Don't support this OS")
endif()
set(LIB_PLACE ${DEV_ROOT}/common)

#nlohmann_json
set(nlohmann_json_DIR ${LIB_PLACE}/json/build)
find_package(nlohmann_json 3.12.0 REQUIRED)

#jwt-cpp
set(OPENSSL_ROOT_DIR ${LIB_PLACE}/OpenSSL)
set(jwt-cpp_DIR ${LIB_PLACE}/jwt-cpp/cmake)
find_package(jwt-cpp CONFIG REQUIRED)

#libpqxx
set(libpqxx_DIR "${LIB_PLACE}/libpqxx/lib/cmake/libpqxx")
find_package(libpqxx REQUIRED)

#libsodium
set(libsodium_DIR "${LIB_PLACE}/libsodium")
set(SODIUM_LIB_DIR "${libsodium_DIR}/x64/Release/v143/static")
set(SODIUM_LIB ${SODIUM_LIB_DIR}/libsodium.lib)

set(content_server_src_dir ${CMAKE_SOURCE_DIR}/src)

#src
file(GLOB ALL_CONTENT_SERVER_SRC ${content_server_src_dir}/*.[hc]pp)

source_group(TREE ${content_server_src_dir} 
    PREFIX "src"
    FILES ${ALL_CONTENT_SERVER_SRC}
)

add_executable(${TARGET_NAME} ${ALL_CONTENT_SERVER_SRC})

#include surs directories
target_include_directories(${TARGET_NAME} PRIVATE ${content_server_src_dir})

#include lib directories
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
