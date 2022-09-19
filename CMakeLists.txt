cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

set(WAMR_ROOT_DIR wasm)
set (WAMR_BUILD_PLATFORM "linux")
set (WAMR_BUILD_TARGET X86_64)
set (WAMR_BUILD_INTERP 1)
set (WAMR_BUILD_AOT 0)
set (WAMR_BUILD_JIT 1)
set (WAMR_BUILD_LIBC_BUILTIN 1)
set (WAMR_BUILD_LIBC_WASI 0)
set (WAMR_BUILD_APP_FRAMEWORK 1)
set (WAMR_BUILD_APP_LIST WAMR_APP_BUILD_BASE)
set (WAMR_BUILD_MULTI_MODULE 1)
set (WAMR_BUILD_MINI_LOADER 0)
set (WARM_BUILD_SHARED_MEMORY 1)
set (WAMR_BUILD_THREAD_MGR 1)

include (${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(vmlib ${WAMR_RUNTIME_LIB_SOURCE})

find_package(OpenSSL REQUIRED)

if(UNIX AND APPLE)
    include_directories(AFTER /opt/homebrew/opt/openssl/include)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(
    kvvm
    src/table.cpp
    src/query.cpp
    src/schema.cpp
    src/rowbank.cpp
    src/cell.cpp
    src/rowbank.cpp
    src/hash.cpp
    src/blob.cpp
    src/istore.cpp
    src/serialize.cpp
    src/udf.cpp
    src/dbg.cpp
)

add_executable(
  test_serialize
  test/serialize_test.cpp
)
target_link_libraries(
  test_serialize
  gtest_main
  kvvm
  vmlib
  OpenSSL::SSL
)

add_executable(
  test_lowest_level
  test/hash_test.cpp
  test/blob_test.cpp
  test/istore_test.cpp
  test/rowbank_test.cpp
)

target_link_libraries(
  test_lowest_level
  gtest_main
  kvvm
  OpenSSL::SSL
)

add_executable(
  test_table
  test/table_test.cpp
  test/schema_test.cpp
  test/query_test.cpp
)

target_link_libraries(
  test_table
  gtest_main
  kvvm
  OpenSSL::SSL
)

include(GoogleTest)
gtest_discover_tests(test_lowest_level)
gtest_discover_tests(test_table)
gtest_discover_tests(test_serialize)
