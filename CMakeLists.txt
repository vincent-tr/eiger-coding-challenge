cmake_minimum_required(VERSION 3.0)

project(EigerCodingChallenge)

set(CMAKE_CXX_STANDARD 20)

file (GLOB_RECURSE CC_FILES FOLLOW_SYMLINKS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

# remove test files
list(FILTER CC_FILES EXCLUDE REGEX "^(.*)\\.test\\.cc$")

file (GLOB_RECURSE TEST_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.test.cc
)

file (GLOB_RECURSE SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

include_directories(
    "/usr/local/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#     list(APPEND CFLAGS "-D_GNU_SOURCE")
# endif()


################################################################################
# Main binary
################################################################################

add_executable(eiger-coding-challenge ${CC_FILES})

target_link_libraries(eiger-coding-challenge ${LDFLAGS})
target_compile_options(eiger-coding-challenge PUBLIC ${CFLAGS})

################################################################################
# Test
################################################################################

find_package(PkgConfig)
pkg_search_module(GTEST REQUIRED gtest_main)

# Remove main from CC_FILES (gtest already have its own main)
set(CC_FILES_NO_MAIN ${CC_FILES})
list(FILTER CC_FILES_NO_MAIN EXCLUDE REGEX "^(.*)src/main\\.cc$")

enable_testing()

add_executable(test-binary ${CC_FILES_NO_MAIN} ${TEST_FILES})

target_link_libraries(test-binary ${LDFLAGS} ${GTEST_LDFLAGS})
target_compile_options(test-binary PUBLIC ${CFLAGS} ${GTEST_CFLAGS})

include(GoogleTest)
gtest_discover_tests(test-binary)
