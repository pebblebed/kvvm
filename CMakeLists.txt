cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)

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
)

add_executable(
  testino
  test/hello_test.cpp
  test/hash_test.cpp
)
target_link_libraries(
  testino
  gtest_main
  kvvm
  OpenSSL::SSL
)

include(GoogleTest)
gtest_discover_tests(testino)

