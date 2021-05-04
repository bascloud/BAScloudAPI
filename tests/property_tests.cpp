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

TEST(BasicTests, PropertyCollectionTest) {

    std::cout << "\tRequesting all properties..." << std::endl;

    EntityCollection<Property> props = BCAPI.getPropertiesCollection(BASCLOUD_TEST_TENANT_UUID, {}, {}, {}, {}, {}, {}, [](std::exception& e, json& j) {
            throw e;
        });
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tFound properties: " << props.first.size() << std::endl;

    EXPECT_TRUE(props.first.size() >= 1);
}

TEST(BasicTests, SinglePropertyTest) {

    std::cout << "\tRequesting single property with UUID..." << std::endl;

    Property property = BCAPI.getProperty(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_PROPERTY_UUID);
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tProperty UUID: " << property.getUUID() << std::endl;
    std::cout << "\tProperty Name: " << property.getName() << std::endl;
    std::cout << "\tProperty Address: " << fmt::format("{}, {} {}, {}", property.getStreet(), property.getPostalCode(), property.getCity(), property.getCountry()) << std::endl;
    std::cout << "\ttProperty created at: " << fmt::format("{:%F %T}", fmt::localtime(property.getCreatedDate())) << std::endl;
    std::cout << "\ttProperty updated at: " << fmt::format("{:%F %T}", fmt::localtime(property.getLastUpdatedDate())) << std::endl;

    EXPECT_EQ(property.getUUID(), BASCLOUD_TEST_PROPERTY_UUID);
    EXPECT_EQ(property.getName(), "Nils Test-Property");
    EXPECT_EQ(property.getStreet(), "Dieselstraße 6");
    EXPECT_EQ(property.getPostalCode(), "63739");
    EXPECT_EQ(property.getCity(), "Aschaffenburg");
    EXPECT_EQ(property.getCountry(), "Deutschland");
    EXPECT_TRUE(property.getCreatedDate() > 0);
    EXPECT_TRUE(property.getLastUpdatedDate() > 0);
}

TEST(BasicTests, AssociatedConnectorTest) {

    std::cout << "\tRequesting associated connectors of the property..." << std::endl;

    EntityCollection<Connector> prop_connectors = BCAPI.getAssociatedConnectors(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_PROPERTY_UUID, {}, [](std::exception& e, json& j) {
            throw e;
        });
    std::cout << "\t\tOK." << std::endl;

    std::cout << "\tFound property connectors: " << prop_connectors.first.size() << std::endl;

    EXPECT_TRUE(prop_connectors.first.size() >= 1);
}

TEST(BasicTests, CreateUpdateAndDeletePropertyTest) {

    std::cout << "\tCreating new property..." << std::endl;

    Property new_property = BCAPI.createProperty(BASCLOUD_TEST_TENANT_UUID, "TestProperty", "Street", "12345", "City", "Deutschland");
    std::cout << "\t\tOK." << std::endl;

    std::string new_uuid = new_property.getUUID();

    EXPECT_EQ(new_property.getName(), "TestProperty");
    EXPECT_EQ(new_property.getStreet(), "Street");
    EXPECT_EQ(new_property.getPostalCode(), "12345");
    EXPECT_EQ(new_property.getCity(), "City");
    EXPECT_EQ(new_property.getCountry(), "Deutschland");
    EXPECT_TRUE(new_property.getCreatedDate() > 0);
    EXPECT_TRUE(new_property.getLastUpdatedDate() > 0);

    std::cout << "\tUpdating newly created property..." << std::endl;

    Property up_re_property = BCAPI.updateProperty(BASCLOUD_TEST_TENANT_UUID, new_uuid, "TestProperty 2");
    std::cout << "\t\tOK." << std::endl;

    EXPECT_EQ(up_re_property.getName(), "TestProperty 2");

    std::cout << "\tDeleting newly created property..." << std::endl;
    BCAPI.deleteProperty(BASCLOUD_TEST_TENANT_UUID, new_uuid);
    std::cout << "\t\tOK." << std::endl;

    try {
        Property non_property = BCAPI.getProperty(BASCLOUD_TEST_TENANT_UUID, new_uuid);
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