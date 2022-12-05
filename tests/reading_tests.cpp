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

TEST(BasicTests, ReadingCollectionTest) {

    std::cout << "\tRequesting all readings..." << std::endl;

    EntityCollection<Reading> readings = BCAPI.getReadingsCollection(BASCLOUD_TEST_TENANT_UUID, {}, -1, -1, -1, std::numeric_limits<double>::quiet_NaN(), {}, -1, -1, [](const std::string& e, const std::string& j) {
            throw e;
        });
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tFound readings: " << readings.first.size() << std::endl;

    EXPECT_TRUE(readings.first.size() >= 1);
}

TEST(BasicTests, ReadingCollectionPagingTest) {

    std::cout << "\tRequesting paged readings..." << std::endl;

    EntityCollection<Reading> readings = BCAPI.getReadingsCollection(BASCLOUD_TEST_TENANT_UUID, PagingOption(1000), -1, -1, -1, std::numeric_limits<double>::quiet_NaN(), BASCLOUD_TEST_DEVICE_UUID, -1, -1, [](const std::string& e, const std::string& j) {
            throw e;
        });
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tFound readings: " << readings.first.size() << std::endl;
    std::cout << "\tFound total pages: " << readings.second.totalPages << std::endl;

    EXPECT_TRUE(readings.first.size() >= 1);

    int page_cnt = 1;
    while(!readings.second.nextPagePointer.empty()) {
        readings = BCAPI.getReadingsCollection(BASCLOUD_TEST_TENANT_UUID, PagingOption(1000, PagingOption::Direction::NEXT, readings.second.nextPagePointer), -1, -1, -1, std::numeric_limits<double>::quiet_NaN(), BASCLOUD_TEST_DEVICE_UUID, -1, -1, [](const std::string& e, const std::string& j) {
            throw e;
        });

        std::cout << "\tFound readings: " << readings.first.size() << std::endl;
        EXPECT_TRUE(readings.first.size() >= 1);

        page_cnt++;
        
        std::cout << "\tFound paging links: Prev:" << readings.second.previousPagePointer << " Next:" << readings.second.nextPagePointer << std::endl;
    }

    EXPECT_TRUE(readings.second.totalPages == page_cnt);
}

TEST(BasicTests, SingleReadingTest) {

    std::cout << "\tRequesting single reading with UUID..." << std::endl;

    Reading reading = BCAPI.getReading(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_READING_UUID);
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tReading UUID: " << reading.getUUID() << std::endl;
    std::cout << "\tReading Value: " << reading.getValue() << std::endl;
    std::cout << "\tReading timestamp: " << fmt::format("{:%F %T}", fmt::localtime(reading.getTimestamp())) << std::endl;
    std::cout << "\tReading created at: " << fmt::format("{:%F %T}", fmt::localtime(reading.getCreatedDate())) << std::endl;
    std::cout << "\tReading updated at: " << fmt::format("{:%F %T}", fmt::localtime(reading.getLastUpdatedDate())) << std::endl;


    EXPECT_EQ(reading.getUUID(), BASCLOUD_TEST_READING_UUID);
    EXPECT_EQ(reading.getValue(), 1234.56);
    EXPECT_TRUE(reading.getTimestamp() > 0);
    EXPECT_TRUE(reading.getCreatedDate() > 0);
    EXPECT_TRUE(reading.getLastUpdatedDate() > 0);
}


TEST(BasicTests, AssociatedDeviceTest) {

    std::cout << "\tRequesting associated device of the reading again..." << std::endl;

    Device read_device = BCAPI.getAssociatedReadingsDevice(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_READING_UUID);
    std::cout << "\t\tOK." << std::endl;

    EXPECT_EQ(read_device.getUUID(), BASCLOUD_TEST_DEVICE_UUID);
}


TEST(BasicTests, CreateAndDeleteReadingTest) {

    std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    Reading new_read = BCAPI.createReading(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID, 2345.67, currentDateTime);
    std::cout << "\t\tOK." << std::endl;

    std::string new_uuid = new_read.getUUID();

    EXPECT_EQ(new_read.getValue(), 2345.67);
    EXPECT_EQ(new_read.getTimestamp(), currentDateTime);
    // EXPECT_TRUE(new_read.getCreatedDate() >= currentDateTime); // TODO ML: this is wrong at the moment, for some reason the backend returns timestamps -2hours
    // EXPECT_TRUE(new_read.getLastUpdatedDate() >= currentDateTime);

    
    std::cout << "\tRequesting deletion of created reading again... " << std::endl;

    BCAPI.deleteReading(BASCLOUD_TEST_TENANT_UUID, new_uuid);
    std::cout << "\t\tOK." << std::endl;

    try {
        Reading non_read = BCAPI.getReading(BASCLOUD_TEST_TENANT_UUID, new_uuid);
    } catch(NotFoundRequest& e) {
        EXPECT_TRUE(true);
    } catch(...) {
        EXPECT_TRUE(false);
    }

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    authenticate();

    return RUN_ALL_TESTS();
}