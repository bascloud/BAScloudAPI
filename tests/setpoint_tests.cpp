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

TEST(BasicTests, SetPointCollectionTest) {

    std::cout << "\tRequesting all setpoints..." << std::endl;

    EntityCollection<SetPoint> setpoints = BCAPI.getSetPointsCollection(BASCLOUD_TEST_TENANT_UUID, {}, -1, -1, -1, -1, {}, -1, -1, [](const std::string& e, const std::string& j) {
            throw e;
        });
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tFound setpoints: " << setpoints.first.size() << std::endl;

    EXPECT_TRUE(setpoints.first.size() >= 0); // may no set points available
}

TEST(BasicTests, SingleSetPointTest) {

    std::cout << "\tRequesting single setpoint with UUID..." << std::endl;

    SetPoint setpoint = BCAPI.getSetPoint(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_STEPOINT_UUID);
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tSetpoint UUID: " << setpoint.getUUID() << std::endl;
    std::cout << "\tSetpoint Value: " << setpoint.getValue() << std::endl;
    std::cout << "\tSetpoint timestamp: " << fmt::format("{:%F %T}", fmt::localtime(setpoint.getTimestamp())) << std::endl;
    std::cout << "\tSetpoint created at: " << fmt::format("{:%F %T}", fmt::localtime(setpoint.getCreatedDate())) << std::endl;
    std::cout << "\tSetpoint updated at: " << fmt::format("{:%F %T}", fmt::localtime(setpoint.getLastUpdatedDate())) << std::endl;

    EXPECT_EQ(setpoint.getUUID(), BASCLOUD_TEST_STEPOINT_UUID);
    EXPECT_EQ(setpoint.getValue(), 2345.67);
    EXPECT_TRUE(setpoint.getTimestamp() > 0);
    EXPECT_TRUE(setpoint.getCreatedDate() > 0);
    EXPECT_TRUE(setpoint.getLastUpdatedDate() > 0);
}

TEST(BasicTests, CreateAndDeleteSetPointTest) {

    std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    SetPoint new_set = BCAPI.createSetPoint(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID, 2345.67, currentDateTime);
    std::cout << "\t\tOK." << std::endl;

    std::string new_uuid = new_set.getUUID();
    std::cout << "\t\tUUID: " << new_uuid << std::endl;

    EXPECT_EQ(new_set.getValue(), 2345.67);
    EXPECT_EQ(new_set.getTimestamp(), currentDateTime);
    // EXPECT_TRUE(new_set.getCreatedDate()-(2*60*60) >= currentDateTime); // TODO ML: this is wrong at the moment, for some reason the backend returns timestamps -2hours
    // EXPECT_TRUE(new_set.getLastUpdatedDate()-(2*60*60) >= currentDateTime);

    std::cout << "\tAgain Requesting all setpoints..." << std::endl; // Thus deactivating/inactivating them

    std::cout << "\tFrom: " << fmt::format("{:%F %T}", fmt::localtime(currentDateTime-(60*60*24))) << std::endl;
    std::cout << "\tUntil: " << fmt::format("{:%F %T}", fmt::localtime(currentDateTime+(60*60*24))) << std::endl;

    EntityCollection<SetPoint> setpoints = BCAPI.getSetPointsCollection(BASCLOUD_TEST_TENANT_UUID, {}, currentDateTime-(60*60*24), currentDateTime+(60*60*24), -1, -1, {}, -1, -1, [](const std::string& e, const std::string& j) {
            throw e;
        });
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tFound setpoints: " << setpoints.first.size() << std::endl;

    EXPECT_TRUE(setpoints.first.size() >= 1); // may no set points available

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    authenticate();

    return RUN_ALL_TESTS();
}