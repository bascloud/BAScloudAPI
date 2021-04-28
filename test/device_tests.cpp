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

TEST(BasicTests, DeviceCollectionTest) {

    std::cout << "\tRequesting all devices..." << std::endl;

    EntityCollection<Device> devices = BCAPI.getDevicesCollection(BASCLOUD_TEST_TENANT_UUID, {}, {}, {}, {}, [](std::exception& e, json& j) {
            throw e;
        });
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tFound devices: " << devices.first.size() << std::endl;

    EXPECT_TRUE(devices.first.size() >= 1);
}

TEST(BasicTests, SingleDeviceTest) {

    std::cout << "\tRequesting single device with UUID..." << std::endl;

    Device device = BCAPI.getDevice(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID);
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tDevice UUID: " << device.getUUID() << std::endl;
    std::cout << "\tDevice AKS ID: " << device.getAksID() << std::endl;
    std::cout << "\tDevice Description: " << device.getDescription() << std::endl;
    std::cout << "\tDevice Unit: " << device.getUnit() << std::endl;
    std::cout << "\tDevice created at: " << fmt::format("{:%F %T}", fmt::localtime(device.getCreatedDate())) << std::endl;
    std::cout << "\tDevice updated at: " << fmt::format("{:%F %T}", fmt::localtime(device.getLastUpdatedDate())) << std::endl;

    EXPECT_EQ(device.getUUID(), BASCLOUD_TEST_DEVICE_UUID);
    EXPECT_EQ(device.getAksID(), "nils-sdfdsfsdfsdfd");
    EXPECT_EQ(device.getDescription(), "some description");
    EXPECT_EQ(device.getUnit(), "Celsius");
    EXPECT_TRUE(device.getCreatedDate() > 0);
    EXPECT_TRUE(device.getLastUpdatedDate() > 0);
}


TEST(BasicTests, AssociatedConnectorTest) {

    std::cout << "\tRequesting associated connector of the device again..." << std::endl;

    Connector device_conn = BCAPI.getAssociatedConnector(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID);
    std::cout << "\t\tOK." << std::endl;

    EXPECT_EQ(device_conn.getUUID(), BASCLOUD_TEST_CONNECTOR_UUID);
}

TEST(BasicTests, AssociatedReadingsTest) {

    EntityCollection<Reading> device_readings = BCAPI.getAssociatedReadings(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID, {}, [](std::exception& e, json& j) {
            throw e;
        });

    std::cout <<"\t\tFound device with " << device_readings.first.size() << " readings" << std::endl;

    EXPECT_TRUE(device_readings.first.size() >= 1);
}

TEST(BasicTests, AssociatedSetPointsTest) {

    EntityCollection<SetPoint> device_setpoint = BCAPI.getAssociatedSetPoints(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID, {}, [](std::exception& e, json& j) {
            throw e;
        });

    std::cout <<"\t\tFound device with " << device_setpoint.first.size() << " setpoints" << std::endl;

    EXPECT_TRUE(device_setpoint.first.size() >= 0); // May not any setpoints for it
}

// Delete devices not working atm
// TEST(BasicTests, CreateUpdateAndDeleteDeviceTest) {
//     std::cout << "\tCreating new device..." << std::endl;

//     Device new_device = BCAPI.createDevice(BASCLOUD_TEST_TENANT_UUID, DBASCLOUD_TEST_CONNECTOR_UUID, "AKS007", "TestDevice", "m3");
//     std::cout << "\t\tOK." << std::endl;

//     std::string new_uuid = new_device.getUUID();

//     EXPECT_EQ(new_device.getAksID(), "AKS007");
//     EXPECT_EQ(new_device.getDescription(), "TestDevice");
//     EXPECT_EQ(new_device.getUnit(), "m3");
//     EXPECT_TRUE(new_device.getCreatedDate() > 0);
//     EXPECT_TRUE(new_device.getLastUpdatedDate() > 0);

//     std::cout << "\tUpdating newly created device..." << std::endl;

//     Device up_re_device = BCAPI.updateDevice(BASCLOUD_TEST_TENANT_UUID, new_uuid, "AKS008");
//     std::cout << "\t\tOK." << std::endl;

//     std::cout << new_uuid << " - \n" << up_re_device.getUUID() << std::endl;

//     EXPECT_EQ(up_re_device.getAksID(), "AKS008");

//     std::cout << "\tDeleting newly created device..." << std::endl;

//     BCAPI.deleteDevice(BASCLOUD_TEST_TENANT_UUID, new_uuid);
//     std::cout << "\t\tOK." << std::endl;

//     try {
//         Device non_dev = BCAPI.getDevice(BASCLOUD_TEST_TENANT_UUID, new_uuid);
//     } catch(NotFoundRequest& e) {
//         EXPECT_TRUE(true);
//     } catch(...) {
//         EXPECT_TRUE(false);
//     }

// }

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    authenticate();

    return RUN_ALL_TESTS();
}