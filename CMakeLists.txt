cmake_minimum_required(VERSION 3.13)

project(RageOpenRDR2)

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.cpp")
list(FILTER PROJECT_SOURCE_FILES EXCLUDE REGEX "/vendor/")

macro(GroupSources curdir groupindex)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)

    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            GroupSources(${curdir}/${child} ${groupindex}/${child})
        else()
            string(REPLACE "/" "\\" groupname ${groupindex})
            source_group(${groupname} FILES ${curdir}/${child})
        endif()
    endforeach()
endmacro()

GroupSources(${CMAKE_SOURCE_DIR}/src "Source Files")

add_subdirectory("src/vendor/minhook")
add_subdirectory("src/vendor/spdlog")

add_library(RageOpenRDR2 SHARED ${PROJECT_SOURCE_FILES})
add_dependencies(RageOpenRDR2 minhook spdlog)

include_directories(
    ./src
)

set_target_properties(RageOpenRDR2 PROPERTIES
    OUTPUT_NAME "RageOpenRDR2"
    SUFFIX ".asi"
    CXX_STANDARD 20
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/BIN/"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/BIN/Debug/"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/BIN/Release/"
)

set_target_properties( minhook
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LIB"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LIB"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LIB"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/LIB"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/LIB"
)

set_target_properties( spdlog
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LIB"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LIB"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/LIB"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/LIB"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/LIB"
)

set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo)
if(ENABLE_LOGS)
    target_compile_definitions(RageOpenRDR2 PRIVATE _LOG)
endif()

target_link_directories(RageOpenRDR2 PUBLIC "${CMAKE_BINARY_DIR}/LIB")
target_link_libraries(RageOpenRDR2 version.lib debug minhook.x64d.lib optimized minhook.x64.lib)
target_link_libraries(RageOpenRDR2 spdlog::spdlog)