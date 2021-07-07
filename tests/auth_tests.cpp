#include <chrono>
#include <iostream>

#include <fmt/core.h>
#include <fmt/chrono.h>

#include <gtest/gtest.h>

#include "EntityContext.h"
#include "error/Exceptions.h"


TEST(BasicTests, ValidAuthTest) {

    std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cerr <<"CURRENT DATETIME: "<<currentDateTime<<std::endl;

    std::cerr <<"CURRENT DATETIME: "<<fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(currentDateTime))<<std::endl;

    BAScloud::EntityContext BCAPI(BASCLOUD_TEST_URL);

    BCAPI.authenticateWithUserLogin(BASCLOUD_TEST_EMAIL, BASCLOUD_TEST_PASS);

    std::cerr <<"TOKEN: "<<BCAPI.getToken()<<std::endl;
    std::cerr <<"EXPIRATION DATE: "<<fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(BCAPI.getTokenExpirationDate()))<<std::endl;

    EXPECT_EQ(BCAPI.isAuthenticated(), true);
    EXPECT_TRUE(BCAPI.getTokenExpirationDate() > currentDateTime);
    EXPECT_TRUE(BCAPI.getToken().length() > 0);
}

TEST(BasicTests, ConnectorAuthenticationTest) {

    std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cerr <<"CURRENT DATETIME: "<<currentDateTime<<std::endl;

    BAScloud::EntityContext BCAPI(BASCLOUD_TEST_URL);
    BCAPI.authenticateWithUserLogin(BASCLOUD_TEST_ADMIN_EMAIL, BASCLOUD_TEST_PASS);

    std::cout << "\tRequesting API Token for connector..." << std::endl;

    std::string connectorToken = BCAPI.getNewConnectorAuthToken(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_CONNECTOR_UUID);
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tConnector Auth. Token: " << connectorToken << std::endl;

    std::cout << "\tAuthenticating with Connector Token..." << std::endl;

    BCAPI.authenticateWithConnectorToken(connectorToken);

    std::cerr <<"TOKEN: "<<BCAPI.getToken()<<std::endl;
    std::cerr <<"EXPIRATION DATE: " << fmt::format("{:%Y-%m-%d %H:%M:%S}", fmt::localtime(BCAPI.getTokenExpirationDate()))<<std::endl;

    EXPECT_EQ(BCAPI.isAuthenticated(), true);
    EXPECT_TRUE(BCAPI.getTokenExpirationDate() > currentDateTime);
    EXPECT_TRUE(BCAPI.getToken().length() > 0);
}

TEST(BasicTests, InvalidAuthTest) {
    // Invalid authentication should return unauthorized by the api and throw exception accordingly
    BAScloud::EntityContext BCAPI(BASCLOUD_TEST_URL);

    try {
        BCAPI.authenticateWithUserLogin("test@test.de", "test");
    } catch(BAScloud::UnauthorizedRequest& e) {
        EXPECT_EQ(BCAPI.isAuthenticated(), false);
        EXPECT_TRUE(BCAPI.getTokenExpirationDate() == -1);
        EXPECT_TRUE(BCAPI.getToken().length() == 0);
    } catch (...) {
        // FAIL() << "Returned wrong exception while authentication call.";
        throw;
    }
}

TEST(BasicTests, FailedConnectionTest) {
    // Invalid authentication should return unauthorized by the api and throw exception accordingly
    BAScloud::EntityContext BCAPI("https://test.local");

    try {
        BCAPI.authenticateWithUserLogin("test@test.de", "test");
    } catch(BAScloud::ConnectionError& e) {
        EXPECT_EQ(BCAPI.isAuthenticated(), false);
        EXPECT_TRUE(BCAPI.getTokenExpirationDate() == -1);
        EXPECT_TRUE(BCAPI.getToken().length() == 0);
    } catch (...) {
        // FAIL() << "Returned wrong exception while authentication call.";
        throw;
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}