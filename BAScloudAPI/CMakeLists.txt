cmake_minimum_required(VERSION 3.11-3.18)


project(BAScloudAPI
        VERSION 0.2.3
        DESCRIPTION "BAScloud API Library"
        LANGUAGES CXX C)

add_library(BAScloudAPI 
    include/APIContext.h include/EntityContext.h include/entity/Entity.h include/entity/EntityDateMixin.h 
    include/entity/EntityTenantMixin.h include/entity/SetPoint.h include/entity/Reading.h include/entity/Device.h 
    include/entity/Connector.h include/entity/Property.h include/entity/Tenant.h include/entity/User.h
    include/Util.h include/Paging.h include/entity/EntityCollection.h
    src/APIContext.cpp src/EntityContext.cpp src/entity/User.cpp src/entity/Tenant.cpp src/entity/Property.cpp 
    src/entity/Connector.cpp src/entity/Device.cpp src/entity/Reading.cpp src/entity/SetPoint.cpp)

if(BAScloudAPI_BUILD_CODE_COVERAGE)
    target_code_coverage(BAScloudAPI)
endif()

target_link_libraries(BAScloudAPI
    PRIVATE 
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:OpenSSL::Crypto>
    cpr::cpr
    nlohmann_json::nlohmann_json
    fmt::fmt)

# PUBLIC needed to make library includes available elsewhere in project
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Tell compiler to use C++20 features. The code doesn't actually use any of them.
# ML: removed for now, needs cross platform handling
target_compile_features(BAScloudAPI PUBLIC cxx_std_14)
