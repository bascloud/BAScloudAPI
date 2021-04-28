cmake_minimum_required(VERSION 3.11-3.18)

project(tests)

# Information UUID constants used in testing
add_definitions(-DBASCLOUD_TEST_URL="")
add_definitions(-DBASCLOUD_TEST_EMAIL="")
add_definitions(-DBASCLOUD_TEST_PASS="")

add_definitions(-DBASCLOUD_TEST_USER_UUID="")
add_definitions(-DBASCLOUD_TEST_TENANT_UUID="")
add_definitions(-DBASCLOUD_TEST_PROPERTY_UUID="")
add_definitions(-DBASCLOUD_TEST_READING_UUID="")
add_definitions(-DBASCLOUD_TEST_DEVICE_UUID="")
add_definitions(-DBASCLOUD_TEST_CONNECTOR_UUID="")
add_definitions(-DBASCLOUD_TEST_STEPOINT_UUID="")


if(BASCLOUD_TEST_URL STREQUAL "")
    message( WARNING "BASCLOUD_TEST_* variables are not defined. Tests are NOT build." )
    return()
endif()


if(CODE_COVERAGE)
    add_code_coverage_all_targets(EXCLUDE build/* test/*)
endif()

macro(add_api_test _TEST_NAME)
    add_executable(${_TEST_NAME}_tests ${_TEST_NAME}_tests.cpp)
    target_link_libraries(${_TEST_NAME}_tests PRIVATE BASCloudAPI 
        GTest::GTest 
        fmt::fmt
        nlohmann_json::nlohmann_json 
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:OpenSSL::Crypto>
        cpr::cpr)
    add_test(NAME ${_TEST_NAME}_tests COMMAND ${_TEST_NAME}_tests)
    # Group under the "tests" project folder in IDEs such as Visual Studio.
    set_property(TARGET ${_TEST_NAME}_tests PROPERTY FOLDER "tests")
    if(CODE_COVERAGE)
        target_code_coverage(${_TEST_NAME}_tests AUTO ALL EXCLUDE build/* test/* OBJECTS BASCloudAPI)
    endif()
endmacro()

add_api_test(util)
add_api_test(auth)
add_api_test(user)
add_api_test(tenant)
add_api_test(property)
add_api_test(connector)
add_api_test(device)
add_api_test(reading)
add_api_test(setpoint)