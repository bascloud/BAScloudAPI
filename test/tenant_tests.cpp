#include <chrono>
#include <iostream>

#include <fmt/core.h>
#include <fmt/chrono.h>

#include <gtest/gtest.h>

#include "EntityContext.h"
#include "entity/EntityCollection.h"
#include "error/Exceptions.h"


using namespace BASCloud;

EntityContext BCAPI(BASCLOUD_TEST_URL);


void authenticate() {

    std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cerr <<"CURRENT DATETIME: "<<fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(currentDateTime))<<std::endl;

    BCAPI.authenticateWithUserLogin(BASCLOUD_TEST_EMAIL, BASCLOUD_TEST_PASS);

    std::cerr <<"TOKEN: "<<BCAPI.getToken()<<std::endl;
    std::cerr <<"EXPIRATION DATE: "<<fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(BCAPI.getTokenExpirationDate()))<<std::endl;
}

TEST(BasicTests, TenantCollectionTest) {

    std::cout << "\tRequesting all tenants..." << std::endl;

    EntityCollection<Tenant> tenants = BCAPI.getTenantsCollection({}, [](std::exception& e, json& j) {
            throw e;
        });
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tFound tenants: " << tenants.first.size() << std::endl;

    EXPECT_TRUE(tenants.first.size() >= 1);
}

TEST(BasicTests, SingleTenantTest) {

    std::cout << "\tRequesting single tenant with UUID..." << std::endl;

    Tenant tenant = BCAPI.getTenant(BASCLOUD_TEST_TENANT_UUID);
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tTenant UUID: " << tenant.getUUID() << std::endl;
    std::cout << "\tTenant Name: " << tenant.getName() << std::endl;
    std::cout << "\tTenant URL Name: " << tenant.getUrlName() << std::endl;
    std::cout << "\tTenant created at: " << fmt::format("{:%F %T}", fmt::localtime(tenant.getCreatedDate())) << std::endl;
    std::cout << "\tTenant updated at: " << fmt::format("{:%F %T}", fmt::localtime(tenant.getLastUpdatedDate())) << std::endl;

    EXPECT_EQ(tenant.getUUID(), BASCLOUD_TEST_TENANT_UUID);
    EXPECT_EQ(tenant.getName(), "testing");
    EXPECT_EQ(tenant.getUrlName(), "testing");
    EXPECT_TRUE(tenant.getCreatedDate() > 0);
    EXPECT_TRUE(tenant.getLastUpdatedDate() > 0);
}


TEST(BasicTests, AssociatedUserTest) {

    std::cout << "\tRequesting associated users of tenant..." << std::endl;

    EntityCollection<User> tenant_user = BCAPI.getAssociatedUsers(BASCLOUD_TEST_TENANT_UUID, {}, [](std::exception& e, json& j) {
            throw e;
        });
    std::cout << "\t\tOK." << std::endl;

    EXPECT_TRUE(tenant_user.first.size() >= 1);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    authenticate();

    return RUN_ALL_TESTS();
}