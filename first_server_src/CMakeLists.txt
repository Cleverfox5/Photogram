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
set(SODIUM_LIBRARY ${libsodium_DIR}/v143/static/libsodium.lib)




set(second_server_dir ${CMAKE_SOURCE_DIR}/second_server_src)

#src
file(GLOB MAIN_SRC              ${CMAKE_SOURCE_DIR}/*.cpp
                                ${CMAKE_SOURCE_DIR}/*.h
)
#file(GLOB SEC_CONNECT_SRC      ${second_server_dir}/AnotherServerConnection/*.[hc]pp)
#file(GLOB SEC_DB_SRC           ${second_server_dir}/DBclient/*.[hc]pp)
#file(GLOB SEC_FORM_PARS_SRC   ${second_server_dir}/FormDataParser/*.[hc]pp)
#file(GLOB SEC_FORM_REQ_SRC    ${second_server_dir}/FormRequest/*.[hc]pp)
#file(GLOB SEC_FORM_RSP_SRC    ${second_server_dir}/FormResponse/*.[hc]pp)
#file(GLOB SEC_TCP_CONNECT_SRC ${second_server_dir}/TcpConnection/*.[hc]pp)
#file(GLOB SEC_TCP_SERVER_SRC  ${second_server_dir}/TcpServer/*.[hc]pp)

#set(ALL_SECOND_SERVER_SRC
#    ${MAIN_SRC}
#    ${SEC_CONNECT_SRC}
#    ${SEC_DB_SRC}
#    ${SEC_FORM_PARS_SRC}
#    ${SEC_FORM_REQ_SRC}
#    ${SEC_FORM_RSP_SRC}
#    ${SEC_TCP_CONNECT_SRC}
#    ${SEC_TCP_SERVER_SRC}
#)

#source_group(TREE ${second_server_dir} 
#    PREFIX "src"
#    FILES ${ALL_SECOND_SERVER_SRC}
#)

add_executable(${TARGET_NAME} ${MAIN_SRC})

#include surs directories
#target_include_directories(${TARGET_NAME} PRIVATE ${second_server_dir})
#target_include_directories(${TARGET_NAME} PRIVATE ${second_server_dir}/AnotherServerConnection)
#target_include_directories(${TARGET_NAME} PRIVATE ${second_server_dir}/DBclient)
#target_include_directories(${TARGET_NAME} PRIVATE ${second_server_dir}/FormDataParser)
#target_include_directories(${TARGET_NAME} PRIVATE ${second_server_dir}/FormRequest)
#target_include_directories(${TARGET_NAME} PRIVATE ${second_server_dir}/TcpConnection)
#target_include_directories(${TARGET_NAME} PRIVATE ${second_server_dir}/TcpServer)
#target_include_directories(${TARGET_NAME} PRIVATE ${second_server_dir}/FormResponse)

#include lib directories
target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${TARGET_NAME} PRIVATE "${libpqxx_DIR}/include")
target_include_directories(${TARGET_NAME} PRIVATE "${libsodium_DIR}/include")

#link
target_link_libraries(
    ${TARGET_NAME} 
    PRIVATE 
    nlohmann_json::nlohmann_json
    jwt-cpp::jwt-cpp
    libpqxx::pqxx
    ${SODIUM_LIBRARY}
)

#settings for VS
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
