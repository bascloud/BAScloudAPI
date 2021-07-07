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

TEST(BasicTests, ConnectorCollectionTest) {

    std::cout << "\tRequesting all connectors..." << std::endl;

    EntityCollection<Connector> connectors = BCAPI.getConnectorsCollection(BASCLOUD_TEST_TENANT_UUID);
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tFound properties: " << connectors.first.size() << std::endl;

    EXPECT_TRUE(connectors.first.size() >= 1);
}

TEST(BasicTests, SingleConnectorTest) {

    std::cout << "\tRequesting single connector with UUID..." << std::endl;

    Connector connector = BCAPI.getConnector(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_CONNECTOR_UUID);
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tConnector UUID: " << connector.getUUID() << std::endl;
    std::cout << "\tConnector Name: " << connector.getName() << std::endl;
    std::cout << "\tConnector created at: " << fmt::format("{:%F %T}", fmt::localtime(connector.getCreatedDate())) << std::endl;
    std::cout << "\tConnector updated at: " << fmt::format("{:%F %T}", fmt::localtime(connector.getLastUpdatedDate())) << std::endl;

    EXPECT_EQ(connector.getUUID(), BASCLOUD_TEST_CONNECTOR_UUID);
    EXPECT_EQ(connector.getName(), "Stefan01");
    EXPECT_TRUE(connector.getCreatedDate() > 0);
    EXPECT_TRUE(connector.getLastUpdatedDate() > 0);
}

// Deprecated
// TEST(BasicTests, AssociatedPropertyTest) {

//     std::cout << "\tRequesting associated property of the connector again..." << std::endl;

//     Property conn_prop = BCAPI.getAssociatedProperty(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_CONNECTOR_UUID);
//     std::cout << "\t\tOK." << std::endl;

//     std::cout << "\tConnector's property UUID: " << conn_prop.getUUID() << std::endl;

//     EXPECT_EQ(conn_prop.getUUID(), BASCLOUD_TEST_PROPERTY_UUID);
// }

TEST(BasicTests, AssociatedDevicesTest) {

    std::cout << "\tRequest connector's associated devices..." << std::endl;
    PagingOption paging = {};

    EntityCollection<Device> conn_devices = BCAPI.getAssociatedConnectorDevices(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_CONNECTOR_UUID);

    std::cout <<"\t\tFound connector with " << conn_devices.first.size() << " devices" << std::endl;

    EXPECT_TRUE(conn_devices.first.size() >= 1);
}

TEST(BasicTests, CreateUpdateAndDeleteConnectorTest) {

    std::cout << "\tCreating new connector..." << std::endl;

    Connector new_connector = BCAPI.createConnector(BASCLOUD_TEST_TENANT_UUID, "TestConnector");
    std::cout << "\t\tOK." << std::endl;

    std::string new_uuid = new_connector.getUUID();

    EXPECT_EQ(new_connector.getName(), "TestConnector");
    EXPECT_TRUE(new_connector.getCreatedDate() > 0);
    EXPECT_TRUE(new_connector.getLastUpdatedDate() > 0);

    std::cout << "\tUpdating newly created connector..." << std::endl;

    Connector up_re_connector = BCAPI.updateConnector(BASCLOUD_TEST_TENANT_UUID, new_uuid, "TestConnector2");
    std::cout << "\t\tOK." << std::endl;

    EXPECT_EQ(up_re_connector.getName(), "TestConnector2");


    std::cout << "\tDeleting newly created connector..." << std::endl;
    BCAPI.deleteConnector(BASCLOUD_TEST_TENANT_UUID, new_uuid);
    std::cout << "\t\tOK." << std::endl;

    try {
        Connector non_connector = BCAPI.getConnector(BASCLOUD_TEST_TENANT_UUID, new_uuid);
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