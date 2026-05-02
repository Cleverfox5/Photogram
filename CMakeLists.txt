cmake_minimum_required(VERSION 3.20)

project(Photogram)

set(TARGET_CONTENT_SERVER content_server)
set(TARGET_AUTH_SERVER auth_server)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0167 OLD)

if (WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00) #from Boost documentation
endif()

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

#Boost
set(Boost_ROOT ${LIB_PLACE}/boost/boost_1_89_0)
find_package(Boost 1.89 REQUIRED)
link_directories(${Boost_LIBRARY_DIRS})

set(content_server_dir ${CMAKE_SOURCE_DIR}/content_server_src)

#src
file(GLOB_RECURSE ALL_CONTENT_SERVER_SRC ${content_server_dir}/*.[hc]pp)
#file(GLOB_RECURSE ALL_)

source_group(TREE ${content_server_dir} 
    PREFIX "src"
    FILES ${ALL_CONTENT_SERVER_SRC}
)

add_executable(${TARGET_CONTENT_SERVER} ${ALL_CONTENT_SERVER_SRC})

#include surs directories
target_include_directories(${TARGET_CONTENT_SERVER} PRIVATE ${content_server_dir})
target_include_directories(${TARGET_CONTENT_SERVER} PRIVATE ${content_server_dir}/AnotherServerConnection)
target_include_directories(${TARGET_CONTENT_SERVER} PRIVATE ${content_server_dir}/DBclient)
target_include_directories(${TARGET_CONTENT_SERVER} PRIVATE ${content_server_dir}/FormDataParser)
target_include_directories(${TARGET_CONTENT_SERVER} PRIVATE ${content_server_dir}/FormRequest)
target_include_directories(${TARGET_CONTENT_SERVER} PRIVATE ${content_server_dir}/TcpConnection)
target_include_directories(${TARGET_CONTENT_SERVER} PRIVATE ${content_server_dir}/TcpServer)
target_include_directories(${TARGET_CONTENT_SERVER} PRIVATE ${content_server_dir}/FormResponse)

#include lib directories
target_include_directories(${TARGET_CONTENT_SERVER} PRIVATE ${Boost_INCLUDE_DIRS})

#link
target_link_libraries(
    ${TARGET_CONTENT_SERVER} 
    PRIVATE 
    nlohmann_json::nlohmann_json
    jwt-cpp::jwt-cpp
    libpqxx::pqxx
)

#settings for VS
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_CONTENT_SERVER})
