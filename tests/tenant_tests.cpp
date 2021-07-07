#include <chrono>
#include <iostream>

#include <fmt/core.h>
#include <fmt/chrono.h>

#include <gtest/gtest.h>

#include "EntityContext.h"
#include "entity/EntityCollection.h"
#include "error/Exceptions.h"


using namespace BAScloud;

EntityContext BCAPI(BASCLOUD_TEST_URL);


void authenticate() {

    std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cerr <<"CURRENT DATETIME: "<<fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(currentDateTime))<<std::endl;

    BCAPI.authenticateWithUserLogin(BASCLOUD_TEST_ADMIN_EMAIL, BASCLOUD_TEST_PASS);

    std::cerr <<"TOKEN: "<<BCAPI.getToken()<<std::endl;
    std::cerr <<"EXPIRATION DATE: "<<fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(BCAPI.getTokenExpirationDate()))<<std::endl;
}

TEST(BasicTests, TenantCollectionTest) {

    std::cout << "\tRequesting all tenants..." << std::endl;

    EntityCollection<Tenant> tenants = BCAPI.getTenantsCollection({}, -1, -1, [](std::exception& e, json& j) {
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
    EXPECT_EQ(tenant.getUrlName(), "tenant_8eda44f3a00d426fbe17d3260aaec0ed");
    EXPECT_TRUE(tenant.getCreatedDate() > 0);
    EXPECT_TRUE(tenant.getLastUpdatedDate() > 0);
}


TEST(BasicTests, AssociatedUserTest) {

    std::cout << "\tRequesting associated users of tenant..." << std::endl;

    EntityCollection<User> tenant_user = BCAPI.getAssociatedUsers(BASCLOUD_TEST_TENANT_UUID, {}, {}, -1, -1, [](std::exception& e, json& j) {
            throw e;
        });
    std::cout << "\t\tOK." << std::endl;

    EXPECT_TRUE(tenant_user.first.size() >= 1);
}

TEST(BasicTests, AssignUserTest) {
    
    std::vector<std::string> users = {BASCLOUD_TEST_USER2_UUID, BASCLOUD_TEST_USER3_UUID};
    std::vector<std::string> roles = {"admin", "user"};

    std::cout << "\tAssigning users to tenant..." << std::endl;

    BCAPI.assignTenantUsers(BASCLOUD_TEST_TENANT_UUID, users, roles);
    std::cout << "\t\tOK." << std::endl;

    EntityCollection<User> tenant_user = BCAPI.getAssociatedUsers(BASCLOUD_TEST_TENANT_UUID, {}, {}, -1, -1, [](std::exception& e, json& j) {
            throw e;
        });

    EXPECT_TRUE(tenant_user.first.size() >= 1);

    for(const std::string uuid: users) {
        bool found=false;
        for(int i=0; i<tenant_user.first.size(); i++) {
            if(tenant_user.first[i].getUUID() == uuid) {
                found=true;
                break;
            }
        }
        if(!found) EXPECT_TRUE(false);
    }

    std::cout << "\tRemoving users from tenant..." << std::endl;

    BCAPI.removeTenantUsers(BASCLOUD_TEST_TENANT_UUID, users);
    std::cout << "\t\tOK." << std::endl;

    EntityCollection<User> tenant_user2 = BCAPI.getAssociatedUsers(BASCLOUD_TEST_TENANT_UUID, {}, {}, -1, -1, [](std::exception& e, json& j) {
            throw e;
        });

    EXPECT_TRUE(tenant_user2.first.size() == (tenant_user.first.size()-2));

    for(const std::string uuid: users) {
        bool found=false;
        for(int i=0; i<tenant_user2.first.size(); i++) {
            if(tenant_user2.first[i].getUUID() == uuid) {
                found=true;
                break;
            }
        }
        if(found) EXPECT_TRUE(false);
    }
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    authenticate();

    return RUN_ALL_TESTS();
}