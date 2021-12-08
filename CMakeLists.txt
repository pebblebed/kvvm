cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)

include_directories(AFTER src/hps/src)
if(UNIX AND APPLE)
    include_directories(AFTER /opt/homebrew/opt/openssl/include)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(
    kvvm
    src/hash.cpp
    src/istore.cpp
    src/table.cpp
    src/serialize.cpp
)

add_executable(
  test_lowest_level
  test/serialize_test.cpp
  test/hash_test.cpp
  test/blob_test.cpp
  test/istore_test.cpp
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

