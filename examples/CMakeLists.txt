cmake_minimum_required(VERSION 3.11-3.18)

project(examples)

macro(add_api_example _EXAMPLE_NAME)
    add_executable(${_EXAMPLE_NAME} ${_EXAMPLE_NAME}.cpp)
    target_link_libraries(${_EXAMPLE_NAME} PRIVATE BAScloudAPI 
        fmt::fmt
        nlohmann_json::nlohmann_json 
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:OpenSSL::Crypto>
        cpr::cpr)
    target_compile_features(${_EXAMPLE_NAME} PUBLIC cxx_std_14)
endmacro()

add_api_example(main)
add_api_example(login)
add_api_example(create_device)
add_api_example(create_reading)
add_api_example(create_setpoint)

add_api_example(collect_devices)
add_api_example(collect_readings)
add_api_example(collect_setpoints)
