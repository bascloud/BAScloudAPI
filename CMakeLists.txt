cmake_minimum_required(VERSION 3.11-3.18)

project(
    BAScloudAPIProject
    LANGUAGES CXX C)


if(NOT "$ENV{RASPBERRY_VERSION}" STREQUAL "")
  message("FOUND ENV RASPBERRY_VERSION $ENV{RASPBERRY_VERSION}")
	if($ENV{RASPBERRY_VERSION} VERSION_GREATER 3)
		set(RASPBERRY_VERSION 3)
	else()
		set(RASPBERRY_VERSION $ENV{RASPBERRY_VERSION})
	endif()
endif()

if(DEFINED RASPBERRY_VERSION)
  set(CMAKE_SYSTEM_NAME "Linux")
  if(RASPBERRY_VERSION VERSION_GREATER 1)
    set(CMAKE_SYSTEM_PROCESSOR "armv7")
    set(ARCH "armv7")
  else()
    set(CMAKE_SYSTEM_PROCESSOR "arm")
    set(ARCH "arm")
  endif()
  message("SET ARCHITECTURE ${ARCH}")
endif()

#set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "")
#SET(OPENSSL_ROOT_DIR /Users/stefan/Documents/Development/openssl_x86_64)
#SET(OPENSSL_ROOT_DIR /Users/stefan/opt/anaconda3/pkgs/openssl-1.1.1d-h1de35cc_4/lib)

# Only for linux/macos systems with curl dependency openssl
# Path for SSL certificates needs to be updated, path taken from "curl-config --ca"
# CACHE INTERNAL "" FORCE seems to solve problem with bad cert path
set(CURL_CA_BUNDLE "/etc/ssl/certs/ca-certificates.crt" CACHE INTERNAL "" FORCE)
set(CURL_CA_PATH "auto" CACHE INTERNAL "" FORCE)

# Deactivate test building as tests are build for each cmake reconfigure due to FetchContent
set(BUILD_TESTING OFF CACHE INTERNAL "" FORCE)
set(CURL_DISABLE_TESTS ON)
set(CPR_BUILD_TESTS OFF)
set(CPR_BUILD_TESTS_SSL OFF)


# Build options
option(BASCLOUDAPI_BUILD_TESTS "Build library tests" ON)
option(BASCLOUDAPI_BUILD_EXAMPLES "Build examples" ON)
option(BASCLOUDAPI_BUILD_DOC "Build documentation" OFF)
option(BASCLOUDAPI_BUILD_PYTHON "Build library python bindings" ON)
option(BASCLOUDAPI_BUILD_CODE_COVERAGE "Build code-coverage" OFF)


if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(BASCLOUDAPI_BUILD_CODE_COVERAGE OFF)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  add_definitions(-DNOMINMAX)
  # Bug in cmake? needs to be defined else cmake config fails
  set(CMAKE_SHARED_LINKER_FLAGS_THREADSAN "")
  set(CMAKE_SHARED_LINKER_FLAGS_ADDRSAN "")
  set(CMAKE_SHARED_LINKER_FLAGS_ALLSAN "")
else()
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/code-coverage.cmake)
endif()

if(BASCLOUDAPI_BUILD_CODE_COVERAGE)
  set(CODE_COVERAGE ON)
else()
  set(CODE_COVERAGE ON)
endif()

include(FetchContent)

FetchContent_Declare(cpr 
  GIT_REPOSITORY https://github.com/whoshuu/cpr.git
  GIT_TAG 1.6.0)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.7.3)
FetchContent_MakeAvailable(json)

# Fmt formatting library as a drop in replacement for std::format (not yet supported)
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG 7.1.3)
FetchContent_MakeAvailable(fmt)

# GTest configuration
if(BASCLOUDAPI_BUILD_TESTS)
  FetchContent_Declare(googletest
                        URL                    https://github.com/google/googletest/archive/release-1.10.0.tar.gz
                        URL_HASH               SHA256=9dc9157a9a1551ec7a7e43daea9a694a0bb5fb8bec81235d8a1e6ef64c716dcb # the file hash for release-1.10.0.tar.gz
                        USES_TERMINAL_DOWNLOAD TRUE)   # <---- This is needed only for Ninja to show download progress
  FetchContent_MakeAvailable(googletest)
  
  add_library(gtest_int INTERFACE)
  target_link_libraries(gtest_int INTERFACE gtest)
  target_include_directories(gtest_int INTERFACE ${googletest_SOURCE_DIR}/include)

  add_library(GTest::GTest ALIAS gtest_int)
  
  # Group under the "tests/gtest" project folder in IDEs such as Visual Studio.
  set_property(TARGET gtest PROPERTY FOLDER "tests/gtest")
  set_property(TARGET gtest_main PROPERTY FOLDER "tests/gtest")
endif()



# check if Doxygen is installed
find_package(Doxygen)
if(DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs_doxygen/ DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/docs)
else()
  message("Doxygen need to be installed to generate the doxygen documentation. Documentation will not be generated.")
endif()

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  find_package(OpenSSL REQUIRED)
endif()

add_subdirectory(BAScloudAPI)

if(BASCLOUDAPI_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(BASCLOUDAPI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BASCLOUDAPI_BUILD_PYTHON)
  add_subdirectory(pybind11)
  pybind11_add_module(pyBAScloudAPI pyBAScloudAPI/src/main.cpp)
  
  target_link_libraries(pyBAScloudAPI PRIVATE BAScloudAPI 
    fmt::fmt
    nlohmann_json::nlohmann_json 
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:OpenSSL::Crypto>
    cpr::cpr)
endif()

