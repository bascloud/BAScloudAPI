#include <iostream>
#include <vector>

#include <fmt/core.h>
#include <fmt/chrono.h>

#include "EntityContext.h"
#include "entity/EntityCollection.h"
#include "entity/User.h"
#include "entity/Tenant.h"
#include "entity/Property.h"
#include "entity/Connector.h"
#include "entity/Device.h"
#include "entity/Reading.h"


/* Demo test application */

using namespace BAScloud;

int main (int argc, char *argv[]) {


    std::cout << "Demo of library methods for BAScloud API endpoints." << std::endl << std::endl;

    std::cout << "Initialising library..."<< std::endl;

    EntityContext BCAPI("http://localhost:3000");

    std::cout << "\tOK." << std::endl;


    std::cout << "1. - Authentication with user login" << std::endl;

        BCAPI.authenticateWithUserLogin("testuser@test.de", "test");
        std::cout << "\tOK." << std::endl;

        std::cout << "\tAuthenticated: " << BCAPI.isAuthenticated() << std::endl;
        std::cout << fmt::format("\tToken valid until: {:%F %T}", fmt::localtime(BCAPI.getTokenExpirationDate())) << std::endl;
        std::cout << "\tToken: " << BCAPI.getToken() << std::endl;


    std::cout << "2. - Access user information" << std::endl;

        std::cout << "\tRequesting all users..." << std::endl;

        EntityCollection<User> users = BCAPI.getUsersCollection({}, {}, [](std::exception& e, json& j) {
            std::cout << "\t\tException occured while receiving user entities: " << e.what() << j << std::endl;
        });

        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound users: " << users.first.size() << std::endl;

        std::cout << "\tRequesting single users with UUID..." << std::endl;

        User currentUser = BCAPI.getUser(users.first.at(1).getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tUser UUID: " << currentUser.getUUID() << std::endl;
        std::cout << "\tUser Email: " << currentUser.getEmail() << std::endl;
        std::cout << "\tUser created at: " << fmt::format("{:%F %T}", fmt::localtime(currentUser.getCreatedDate())) << std::endl;
        std::cout << "\tUser updated at: " << fmt::format("{:%F %T}", fmt::localtime(currentUser.getLastUpdatedDate())) << std::endl;

        std::cout << "\tRequesting associated tenant..." << std::endl;

        Tenant assoc_tenant = BCAPI.getAssociatedTenant(currentUser.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tTenant UUID: " << assoc_tenant.getUUID() << std::endl;


    // std::cout << "2.5. - Create and update user" << std::endl;

    //     std::cout << "\tCreating a new user..." << std::endl;

    //     User new_user = IncompleteUser("Uninitialized", &BCAPI);
    //     try {
    //         new_user = BCAPI.createNewUser("lode@accessec.com", "12346");
    //         std::cout << "\t\tOK." << std::endl;
    //     } catch(std::exception& e) {
    //         std::cout << "\t\tFAILED. " << e.what() << std::endl;
    //     }

    //     std::cout << "\tRequesting created user with UUID: " << new_user.getUUID() << std::endl;

    //     User new_re_user = BCAPI.getUser(new_user.getUUID());
    //     std::cout << "\t\tOK." << std::endl;

    //     std::cout << "\tUser UUID: " << new_re_user.getUUID() << std::endl;
    //     std::cout << "\tUser Email: " << new_re_user.getEmail() << std::endl;
    //     std::cout << "\tUser created at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_user.getCreatedDate())) << std::endl;
    //     std::cout << "\tUser updated at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_user.getLastUpdatedDate())) << std::endl;

    //     std::cout << "\tUpdate created user information... " << std::endl;

    //     User new_up_user = BCAPI.updateUser(new_user.getUUID(), "wrong@accessec.com");
    //     std::cout << "\t\tOK." << std::endl;

    //     std::cout << "\tUpdated User Email: " << new_up_user.getEmail() << std::endl;

    //     std::cout << "\tDeleting newly created user again... " << std::endl;

    //     BCAPI.deleteUser(new_user.getUUID());
    //     std::cout << "\t\tOK." << std::endl;


    std::cout << "3. - Access tenant information" << std::endl;

        std::cout << "\tRequesting all tenants..." << std::endl;

        EntityCollection<Tenant> tenants = BCAPI.getTenantsCollection();
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound tenants: " << tenants.first.size() << std::endl;

        std::cout << "\tRequesting single tenant with UUID..." << std::endl;

        Tenant tenant = BCAPI.getTenant(tenants.first.at(0).getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tTenant UUID: " << tenant.getUUID() << std::endl;
        std::cout << "\tTenant Name: " << tenant.getName() << std::endl;
        std::cout << "\tTenant URL Name: " << tenant.getUrlName() << std::endl;
        std::cout << "\tTenant created at: " << fmt::format("{:%F %T}", fmt::localtime(tenant.getCreatedDate())) << std::endl;
        std::cout << "\tTenant updated at: " << fmt::format("{:%F %T}", fmt::localtime(tenant.getLastUpdatedDate())) << std::endl;


        std::cout << "\tRequesting associated users of tenant..." << std::endl;

        EntityCollection<User> tenant_user = BCAPI.getAssociatedUsers(tenant.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound tenant users: " << tenant_user.first.size() << std::endl;


    // std::cout << "3.5 - Create tenant and update" << std::endl;

    //     std::cout << "\tCreating new tenant..." << std::endl;

    //     Tenant new_tenant = BCAPI.createTenant("MoritzTestTenant", currentUser.getUUID());
    //     std::cout << "\t\tOK." << std::endl;

    //     std::cout << "\tNew tenant UUID: " << new_tenant.getUUID() << std::endl;

    //     std::cout << "\tRequesting newly created tenant with UUID..." << std::endl;

    //     Tenant new_re_tenant = BCAPI.getTenant(new_tenant.getUUID());
    //     std::cout << "\t\tOK." << std::endl;

    //     std::cout << "\tTenant UUID: " << new_re_tenant.getUUID() << std::endl;
    //     std::cout << "\tTenant Name: " << new_re_tenant.getName() << std::endl;
    //     std::cout << "\tTenant URL Name: " << new_re_tenant.getUrlName() << std::endl;
    //     std::cout << "\tTenant created at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_tenant.getCreatedDate())) << std::endl;
    //     std::cout << "\tTenant updated at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_tenant.getLastUpdatedDate())) << std::endl;


    //     std::cout << "\tUpdating newly created tenant..." << std::endl;

    //     Tenant up_re_tenant = BCAPI.updateTenant(new_re_tenant.getUUID(), "MoritzTestTenant2");
    //     std::cout << "\t\tOK." << std::endl;

    //     std::cout << "\tTenant Name: " << up_re_tenant.getName() << std::endl;


    //     std::cout << "\tRequesting associated users of tenant..." << std::endl;

    //     EntityCollection<User> re_tenant_user = BCAPI.getAssociatedUsers(new_re_tenant.getUUID());
    //     std::cout << "\t\tOK." << std::endl;

    //     std::cout << "\tFound tenant users: " << re_tenant_user.first.size() << std::endl;


    //     std::cout << "\tDeleting newly created tenant..." << std::endl;

    //     BCAPI.deleteTenant(new_re_tenant.getUUID());
    //     std::cout << "\t\tOK." << std::endl;


    std::cout << "4. - Access property information" << std::endl;

        std::cout << "\tRequesting all properties..." << std::endl;

        EntityCollection<Property> props = BCAPI.getPropertiesCollection(tenant.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound properties: " << props.first.size() << std::endl;

        std::cout << "\tRequesting single property with UUID..." << std::endl;
        std::cout << "\tProperty UUID: " << props.first.at(0).getUUID() << std::endl;

        Property property = BCAPI.getProperty(tenant.getUUID(), props.first.at(0).getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tProperty UUID: " << property.getUUID() << std::endl;
        std::cout << "\tProperty Name: " << property.getName() << std::endl;
        std::cout << "\tProperty Address: " << fmt::format("{}, {} {}, {}", property.getStreet(), property.getPostalCode(), property.getCity(), property.getCountry()) << std::endl;
        std::cout << "\ttProperty created at: " << fmt::format("{:%F %T}", fmt::localtime(property.getCreatedDate())) << std::endl;
        std::cout << "\ttProperty updated at: " << fmt::format("{:%F %T}", fmt::localtime(property.getLastUpdatedDate())) << std::endl;


        std::cout << "\tRequesting associated connectors of the property..." << std::endl;

        EntityCollection<Connector> prop_connectors = BCAPI.getAssociatedConnectors(tenant.getUUID(), property.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound property connectors: " << prop_connectors.first.size() << std::endl;


    std::cout << "4.5 - Create property and update" << std::endl;

        std::cout << "\tCreating new property..." << std::endl;

        Property new_property = BCAPI.createProperty(tenant.getUUID(), "MoritzTestProperty", "Street", "12345", "City", "Country");
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tNew property UUID: " << new_property.getUUID() << std::endl;

        std::cout << "\tRequesting newly created property with UUID..." << std::endl;

        Property new_re_property = BCAPI.getProperty(tenant.getUUID(),new_property.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tProperty UUID: " << new_re_property.getUUID() << std::endl;
        std::cout << "\tProperty Name: " << new_re_property.getName() << std::endl;
        std::cout << "\tProperty Address: " << fmt::format("{}, {} {}, {}", new_re_property.getStreet(), new_re_property.getPostalCode(), new_re_property.getCity(), new_re_property.getCountry()) << std::endl;
        std::cout << "\tProperty created at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_property.getCreatedDate())) << std::endl;
        std::cout << "\tProperty updated at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_property.getLastUpdatedDate())) << std::endl;


        std::cout << "\tUpdating newly created property..." << std::endl;

        Property up_re_property = BCAPI.updateProperty(tenant.getUUID(), new_re_property.getUUID(), "MoritzTestProperty2");
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tProperty Name: " << up_re_property.getName() << std::endl;


        // Should be always be empty see next section create new connector for it
        // std::cout << "\tRequesting associated connectors of the property..." << std::endl;

        // EntityCollection<Connector> new_prop_connectors = BCAPI.getAssociatedConnectors(tenant.getUUID(), up_re_property.getUUID());
        // std::cout << "\t\tOK." << std::endl;

        // std::cout << "\tFound property connectors: " << new_prop_connectors.first.size() << std::endl;


        // std::cout << "\tDeleting newly created property..." << std::endl;

        // BCAPI.deleteProperty(tenant.getUUID(), up_re_property.getUUID());
        // std::cout << "\t\tOK." << std::endl;



    std::cout << "5. - Access connector information" << std::endl;

        std::cout << "\tRequesting all connectors..." << std::endl;

        EntityCollection<Connector> connectors = BCAPI.getConnectorsCollection(tenant.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound connectors: " << connectors.first.size() << std::endl;

        std::cout << "\tRequesting single connector with UUID..." << std::endl;
        std::cout << "\tConnector UUID: " << connectors.first.at(0).getUUID() << std::endl;

        Connector connector = BCAPI.getConnector(tenant.getUUID(), connectors.first.at(0).getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tConnector UUID: " << connector.getUUID() << std::endl;
        std::cout << "\tConnector Name: " << connector.getName() << std::endl;
        std::cout << "\tConnector created at: " << fmt::format("{:%F %T}", fmt::localtime(connector.getCreatedDate())) << std::endl;
        std::cout << "\tConnector updated at: " << fmt::format("{:%F %T}", fmt::localtime(connector.getLastUpdatedDate())) << std::endl;

        std::cout << "\tRequesting associated property of the connector again..." << std::endl;

        Property conn_prop = BCAPI.getAssociatedProperty(tenant.getUUID(), connector.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tConnector's property UUID: " << conn_prop.getUUID() << std::endl;


        std::cout << "\tRequest connector's associated devices..." << std::endl;

        int max_devices = 0;
        for(Connector conn: connectors.first) {
            EntityCollection<Device> conn_devices = BCAPI.getAssociatedDevices(tenant.getUUID(), conn.getUUID());
            if(conn_devices.first.size() > max_devices) {
                max_devices = conn_devices.first.size();
                connector = conn;
            }
            std::cout <<"\t\tFound connector with " << conn_devices.first.size() << " devices" << std::endl;
        }


    std::cout << "5.5 - Create connector and update" << std::endl;

        std::cout << "\tCreating new connector..." << std::endl;

        Connector new_connector = BCAPI.createConnector(tenant.getUUID(), new_property.getUUID(), "MoritzTestConnector");
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tNew connector UUID: " << new_connector.getUUID() << std::endl;

        std::cout << "\tRequesting new API key for created connector..." << std::endl;

        std::string connectorToken = BCAPI.getNewConnectorAuthToken(tenant.getUUID(), new_connector.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tConnector Auth. Token: " << connectorToken << std::endl;

        std::cout << "\tRequesting newly created connector with UUID..." << std::endl;

        Connector new_re_connector = BCAPI.getConnector(tenant.getUUID(), new_connector.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tConnector UUID: " << new_re_connector.getUUID() << std::endl;
        std::cout << "\tConnector Name: " << new_re_connector.getName() << std::endl;
        std::cout << "\tConnector created at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_connector.getCreatedDate())) << std::endl;
        std::cout << "\tConnector updated at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_connector.getLastUpdatedDate())) << std::endl;


        std::cout << "\tUpdating newly created connector..." << std::endl;

        Connector up_re_connector = BCAPI.updateConnector(tenant.getUUID(), new_re_connector.getUUID(), "MoritzTestConnector2");
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tConnector Name: " << up_re_connector.getName() << std::endl;



        std::cout << "\tRequesting associated connectors of the new property..." << std::endl;

        EntityCollection<Connector> new_prop_connectors = BCAPI.getAssociatedConnectors(tenant.getUUID(), new_property.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound property connectors: " << new_prop_connectors.first.size() << std::endl;


        // std::cout << "\tDeleting newly created connector..." << std::endl;

        // BCAPI.deleteConnector(tenant.getUUID(), new_connector.getUUID());
        // std::cout << "\t\tOK." << std::endl;

        // std::cout << "\tDeleting newly created property..." << std::endl;

        // BCAPI.deleteProperty(tenant.getUUID(), new_property.getUUID());
        // std::cout << "\t\tOK." << std::endl;



    std::cout << "6. - Access device information" << std::endl;

        std::cout << "\tRequesting all devices..." << std::endl;

        EntityCollection<Device> devices = BCAPI.getDevicesCollection(tenant.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound devices: " << devices.first.size() << std::endl;

        std::cout << "\tRequesting single device with UUID..." << std::endl;
        std::cout << "\tDevice UUID: " << devices.first.at(0).getUUID() << std::endl;

        Device device = BCAPI.getDevice(tenant.getUUID(), devices.first.at(0).getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tDevice UUID: " << device.getUUID() << std::endl;
        std::cout << "\tDevice AKS ID: " << device.getAksID() << std::endl;
        std::cout << "\tDevice Description: " << device.getDescription() << std::endl;
        std::cout << "\tDevice Unit: " << device.getUnit() << std::endl;
        std::cout << "\tDevice created at: " << fmt::format("{:%F %T}", fmt::localtime(device.getCreatedDate())) << std::endl;
        std::cout << "\tDevice updated at: " << fmt::format("{:%F %T}", fmt::localtime(device.getLastUpdatedDate())) << std::endl;

        std::cout << "\tRequesting associated connector of the device again..." << std::endl;

        Connector device_conn = BCAPI.getAssociatedConnector(tenant.getUUID(), device.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tDevice's Connector UUID: " << device_conn.getUUID() << std::endl;


        std::cout << "\tRequesting device associated readings..." << std::endl;

        int max_readings = 0;
        for(Device d: devices.first) {
            EntityCollection<Reading> device_readings = BCAPI.getAssociatedReadings(tenant.getUUID(), d.getUUID());
            if(device_readings.first.size() > max_readings) {
                max_readings = device_readings.first.size();
                device = d;
            }
            std::cout <<"\t\tFound device with " << device_readings.first.size() << " readings" << std::endl;
        }

        std::cout << "\tRequesting paginated device associated readings..." << std::endl;

        PagingOption paging(10, PagingOption::Direction::NONE);

        EntityCollection<Reading> device_readings = BCAPI.getAssociatedReadings(tenant.getUUID(), device.getUUID(), paging);
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound readings: " << device_readings.first.size() << std::endl;

        for(Reading r: device_readings.first) {

            std::cout << "\t\tReading: " << fmt::format("{:%F %T}", fmt::localtime(r.getTimestamp())) << " - " << r.getValue() << std::endl;
        }


        std::cout << "\tRequesting device associated setpoints..." << std::endl;

        int max_setpoints = 0;
        for(Device d: devices.first) {
            EntityCollection<SetPoint> device_setpoint = BCAPI.getAssociatedSetPoints(tenant.getUUID(), d.getUUID(), paging);
            if(device_setpoint.first.size() > max_setpoints) {
                max_setpoints = device_setpoint.first.size();
                device = d;
            }
            std::cout <<"\t\tFound device with " << device_setpoint.first.size() << " setpoints" << std::endl;
        }

        std::cout << "\tRequesting paginated device associated setpoints..." << std::endl;

        EntityCollection<SetPoint> device_setpoint = BCAPI.getAssociatedSetPoints(tenant.getUUID(), device.getUUID(), paging);
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound setpoints: " << device_setpoint.first.size() << std::endl;

        for(SetPoint sp: device_setpoint.first) {

            std::cout << "\t\tSetPoint: " << fmt::format("{:%F %T}", fmt::localtime(sp.getTimestamp())) << " - " << sp.getValue() << std::endl;
        }


    std::cout << "6.5 - Create device and update" << std::endl;

        std::cout << "\tCreating new device..." << std::endl;

        Device new_device = BCAPI.createDevice(tenant.getUUID(), new_connector.getUUID(), "MoritzTestAKS1000", "TestDevice", "m3");
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tNew device UUID: " << new_device.getUUID() << std::endl;

        std::cout << "\tRequesting newly created device with UUID..." << std::endl;

        Device new_re_device = BCAPI.getDevice(tenant.getUUID(), new_device.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tDevice UUID: " << new_re_device.getUUID() << std::endl;
        std::cout << "\tDevice AKS ID: " << new_re_device.getAksID() << std::endl;
        std::cout << "\tDevice Description: " << new_re_device.getDescription() << std::endl;
        std::cout << "\tDevice Unit: " << new_re_device.getUnit() << std::endl;
        std::cout << "\tDevice created at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_device.getCreatedDate())) << std::endl;
        std::cout << "\tDevice updated at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_device.getLastUpdatedDate())) << std::endl;


        std::cout << "\tUpdating newly created device..." << std::endl;

        Device up_re_device = BCAPI.updateDevice(tenant.getUUID(), new_re_device.getUUID(), "MoritzTestAKS1001");
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tDevice AKS ID: " << up_re_device.getAksID() << std::endl;



        std::cout << "\tRequesting associated connector of the new device..." << std::endl;

        Connector new_dev_connector = BCAPI.getAssociatedConnector(tenant.getUUID(), up_re_device.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound device connector: " << new_dev_connector.getUUID() << std::endl;


        // std::cout << "\tDeleting newly created device..." << std::endl;

        // BCAPI.deleteDevice(tenant.getUUID(), new_device.getUUID());
        // std::cout << "\t\tOK." << std::endl;


    std::cout << "7. - Access reading information" << std::endl;

        std::cout << "\tRequesting all readings..." << std::endl;

        EntityCollection<Reading> readings = BCAPI.getReadingsCollection(tenant.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound readings: " << readings.first.size() << std::endl;

        if(readings.first.size() > 0) {

            std::cout << "\tRequesting single reading with UUID..." << std::endl;
            std::cout << "\tReading UUID: " << readings.first.at(0).getUUID() << std::endl;

            Reading reading = BCAPI.getReading(tenant.getUUID(), readings.first.at(0).getUUID());
            std::cout << "\t\tOK." << std::endl;

            std::cout << "\tReading UUID: " << reading.getUUID() << std::endl;
            std::cout << "\tReading Value: " << reading.getValue() << std::endl;
            std::cout << "\tReading timestamp: " << fmt::format("{:%F %T}", fmt::localtime(reading.getTimestamp())) << std::endl;
            std::cout << "\tReading created at: " << fmt::format("{:%F %T}", fmt::localtime(reading.getCreatedDate())) << std::endl;
            std::cout << "\tReading updated at: " << fmt::format("{:%F %T}", fmt::localtime(reading.getLastUpdatedDate())) << std::endl;

            std::cout << "\tRequesting associated device of the reading again..." << std::endl;

            Device read_device = BCAPI.getAssociatedDevice(tenant.getUUID(), reading.getUUID());
            std::cout << "\t\tOK." << std::endl;

            std::cout << "\tReadings's Device UUID: " << read_device.getUUID() << std::endl;

        }


    std::cout << "8. - Create new reading" << std::endl;

        std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        Reading new_read = BCAPI.createReading(tenant.getUUID(), new_device.getUUID(), 1234.56, currentDateTime);
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tRequesting created reading information again... " << std::endl;

        Reading new_re_reading = BCAPI.getReading(tenant.getUUID(), new_read.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tReading UUID: " << new_re_reading.getUUID() << std::endl;
        std::cout << "\tReading Value: " << new_re_reading.getValue() << std::endl;
        std::cout << "\tReading timestamp: " << fmt::format("{:%F %T}", fmt::localtime(new_re_reading.getTimestamp())) << std::endl;
        std::cout << "\tReading created at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_reading.getCreatedDate())) << std::endl;
        std::cout << "\tReading updated at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_reading.getLastUpdatedDate())) << std::endl;


        std::cout << "\tRequesting paginated device associated readings..." << std::endl;

        EntityCollection<Reading> new_device_readings = BCAPI.getAssociatedReadings(tenant.getUUID(), new_device.getUUID(), paging);
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound readings: " << new_device_readings.first.size() << std::endl;

        for(Reading sp: new_device_readings.first) {

            std::cout << "\t\tReading: " << fmt::format("{:%F %T}", fmt::localtime(sp.getTimestamp())) << " - " << sp.getValue() << std::endl;
        }


        std::cout << "\tRequesting deletion of created reading again... " << std::endl;

        BCAPI.deleteReading(tenant.getUUID(), new_re_reading.getUUID());
        std::cout << "\t\tOK." << std::endl;


    std::cout << "9. - Access setpoint information" << std::endl;

        std::cout << "\tRequesting all setpoints..." << std::endl;

        EntityCollection<SetPoint> setpoints = BCAPI.getSetPointsCollection(tenant.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound setpoints: " << setpoints.first.size() << std::endl;

        if(setpoints.first.size() > 0) {

            std::cout << "\tRequesting single setpoint with UUID..." << std::endl;
            std::cout << "\tSetPoint UUID: " << setpoints.first.at(0).getUUID() << std::endl;

            SetPoint setpoint = BCAPI.getSetPoint(tenant.getUUID(), setpoints.first.at(0).getUUID());
            std::cout << "\t\tOK." << std::endl;

            std::cout << "\tSetPoint UUID: " << setpoint.getUUID() << std::endl;
            std::cout << "\tSetPoint Value: " << setpoint.getValue() << std::endl;
            std::cout << "\tSetPoint timestamp: " << fmt::format("{:%F %T}", fmt::localtime(setpoint.getTimestamp())) << std::endl;
            std::cout << "\tSetPoint created at: " << fmt::format("{:%F %T}", fmt::localtime(setpoint.getCreatedDate())) << std::endl;
            std::cout << "\tSetPoint updated at: " << fmt::format("{:%F %T}", fmt::localtime(setpoint.getLastUpdatedDate())) << std::endl;

        }


    std::cout << "10. - Create new setpoint" << std::endl;

        currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() + 3*60*60*60;

        SetPoint new_setPoint = BCAPI.createSetPoint(tenant.getUUID(), new_device.getUUID(), 2345.67, currentDateTime);
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tRequesting created setpoint information again... " << std::endl;

        SetPoint new_re_setpoint = BCAPI.getSetPoint(tenant.getUUID(), new_setPoint.getUUID());
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tSetPoint UUID: " << new_re_setpoint.getUUID() << std::endl;
        std::cout << "\tSetPoint Value: " << new_re_setpoint.getValue() << std::endl;
        std::cout << "\tSetPoint timestamp: " << fmt::format("{:%F %T}", fmt::localtime(new_re_setpoint.getTimestamp())) << std::endl;
        std::cout << "\tSetPoint created at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_setpoint.getCreatedDate())) << std::endl;
        std::cout << "\tSetPoint updated at: " << fmt::format("{:%F %T}", fmt::localtime(new_re_setpoint.getLastUpdatedDate())) << std::endl;

        std::cout << "\tRequesting paginated device associated setpoints..." << std::endl;

        EntityCollection<SetPoint> new_device_setpoint = BCAPI.getAssociatedSetPoints(tenant.getUUID(), new_device.getUUID(), paging);
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tFound setpoints: " << new_device_setpoint.first.size() << std::endl;

        for(SetPoint sp: new_device_setpoint.first) {

            std::cout << "\t\tSetPoint: " << fmt::format("{:%F %T}", fmt::localtime(sp.getTimestamp())) << " - " << sp.getValue() << std::endl;
        }


    std::cout << "12. - Deleting created entities" << std::endl;

        // std::cout << "\tDeleting created device..." << std::endl;

        // BCAPI.deleteDevice(tenant.getUUID(), new_device.getUUID());
        // std::cout << "\t\tOK." << std::endl;


        std::cout << "\tDeleting created connector..." << std::endl;

        BCAPI.deleteConnector(tenant.getUUID(), new_connector.getUUID());
        std::cout << "\t\tOK." << std::endl;


        std::cout << "\tDeleting created property..." << std::endl;

        BCAPI.deleteProperty(tenant.getUUID(), new_property.getUUID());
        std::cout << "\t\tOK." << std::endl;
        


}
