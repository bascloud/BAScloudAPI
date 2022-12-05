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

    BCAPI.authenticateWithUserLogin(BASCLOUD_TEST_EMAIL, BASCLOUD_TEST_PASS);

    std::cerr <<"TOKEN: "<<BCAPI.getToken()<<std::endl;
    std::cerr <<"EXPIRATION DATE: "<<fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(BCAPI.getTokenExpirationDate()))<<std::endl;
}

TEST(BasicTests, UserCollectionTest) {

    std::cout << "\tRequesting all users..." << std::endl;

    EntityCollection<User> users = BCAPI.getUsersCollection({}, {}, -1, -1, [](const std::string& e, const std::string& j) {
            throw e;
        });

    EXPECT_TRUE(users.first.size() >= 1);
}

TEST(BasicTests, SingleUserTest) {

    std::cout << "\tRequesting single users with UUID..." << std::endl;

    User currentUser = BCAPI.getUser(BASCLOUD_TEST_USER_UUID);
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tUser UUID: " << currentUser.getUUID() << std::endl;
    std::cout << "\tUser Email: " << currentUser.getEmail() << std::endl;
    std::cout << "\tUser created at: " << fmt::format("{:%F %T}", fmt::localtime(currentUser.getCreatedDate())) << std::endl;
    std::cout << "\tUser updated at: " << fmt::format("{:%F %T}", fmt::localtime(currentUser.getLastUpdatedDate())) << std::endl;

    EXPECT_EQ(currentUser.getUUID(), BASCLOUD_TEST_USER_UUID);
    EXPECT_EQ(currentUser.getEmail(), BASCLOUD_TEST_EMAIL);
    EXPECT_TRUE(currentUser.getCreatedDate() > 0);
    EXPECT_TRUE(currentUser.getLastUpdatedDate() > 0);
}

// Need the authority to delete it again
// TEST(BasicTests, CreateUserTest) {

//     std::cout << "\tCreating a new user..." << std::endl;

//     User new_user = BCAPI.createNewUser("lode@accessec.com", "12346");
//     std::cout << "\t\tOK." << std::endl;

//     EXPECT_EQ(new_user.getUUID().length(), 36);
//     EXPECT_EQ(new_user.getEmail(), "lode@accessec.com");
//     EXPECT_TRUE(new_user.getCreatedDate() > 0);
//     EXPECT_TRUE(new_user.getLastUpdatedDate() > 0);
// }

TEST(BasicTests, AssociatedTenantTest) {

    std::cout << "\tRequesting associated tenant..." << std::endl;

    Tenant assoc_tenant = BCAPI.getAssociatedTenant(BASCLOUD_TEST_USER_UUID);
    std::cout << "\t\tOK." << std::endl;

    EXPECT_EQ(assoc_tenant.getUUID(), BASCLOUD_TEST_TENANT_UUID);
    EXPECT_EQ(assoc_tenant.getName(), "testing");
    EXPECT_EQ(assoc_tenant.getUrlName(), "tenant_8eda44f3a00d426fbe17d3260aaec0ed");
    EXPECT_TRUE(assoc_tenant.getCreatedDate() > 0);
    EXPECT_TRUE(assoc_tenant.getLastUpdatedDate() > 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    authenticate();

    return RUN_ALL_TESTS();
}