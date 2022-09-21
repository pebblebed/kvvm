cmake_minimum_required(VERSION 3.23)

project(udfs)
include(ExternalProject)

# Our wasm UDFs
include(ExternalProject)
function(AddUDF NAME DIR)
ExternalProject_Add(
  ${NAME}
  PREFIX ${DIR}
  SOURCE_DIR ${DIR}
  BINARY_DIR ${DIR}
  DOWNLOAD_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND npm run asbuild
  TEST_COMMAND npm test
)
endfunction()
AddUDF(hello-world-udf ${CMAKE_SOURCE_DIR}/wasi-hello-world)
