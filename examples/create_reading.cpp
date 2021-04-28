#include <iostream>

#include <fmt/core.h>
#include <fmt/chrono.h>

#include "EntityContext.h"
#include "error/Exceptions.h"


using namespace BASCloud;

int main (int argc, char *argv[]) {

    std::cout << "Initialising library..."<< std::endl;

    // For accessing the BASCloud API, a API endpoint URL needs to be defined (Note: no trailing symbols, must end with TLD identifier)
    EntityContext BCAPI("server_URL");
    std::cout << "\tOK." << std::endl;


    // Before accessing any API methods, a user must be authenticated
    std::cout << "1. - Authentication with user login" << std::endl;

    /**
     * All methods in the BASCloud API library potential throw different exceptions to signal a failure in the API request.
     * 
     * The following exceptions may be thrown:
     * 
     * ServerError: If the server returns a (unexpected) server error 500.
     * ConnectionError: If the connection to the server could not be establised.
     * BadRequest: If parameters given through the request were invalid e.g. invalid UUID.
     * UnauthorizedRequest: If the currently authenticated user has no authority to access the API endpoint.
     * NotFoundRequest: If the requested resource was not found e.g. no entity with the given UUID exists.
     * InvalidResponse: If the response returned by the server was invalid e.g. invalid JSON was received or missing data.
     * 
     * Wherver to catch all exceptions together or individually is left up to the library user and may be use-case dependend.
     * 
     */
    try {
        BCAPI.authenticateWithUserLogin("user_email", "user_password");
        std::cout << "\tOK." << std::endl;
    } catch(std::exception& e) {
        std::cout << "\tRequest failed." << std::endl << e.what() << std::endl;
        return 1;
    }

    // Check if current authentication is valid, checks expiration date and valid token
    std::cout << "\tAuthenticated: " << BCAPI.isAuthenticated() << std::endl;


    std::cout << "2. - Create a new reading" << std::endl;

    // A reading contains a timestamp of the time the reading was read (as a UNIX timestamp in seconds)
    std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // For creating a reading, its associated tenant UUID is needed
    std::string tenantUUID = "";

    // For creating a reading, its associated device UUID is needed describing the device for which a new reading is saved.
    std::string deviceUUID = "";

    // The value reading of the device at the specified timestamp.
    double value = 1234.56;

    
    /** 
     * If no tenant and device UUIDs are known, the needed information can be requested from the API
     * 
     * To retrieve the current tenant the user UUID is needed, if the UUID is not known, the current user can be requested through a collection request
     * Usually this returns only your own user, only for administators multiple users are returned
     */
    // EntityCollection<User> users = BCAPI.getUsersCollection({}, {}, [](std::exception& e, json& j) {
    //         std::cout << "\t\tException occured while receiving user entities: " << e.what() << j << std::endl;
    //     });
    // std::cout << "\t\tOK." << std::endl;
    // User currentUser = users.first.at(0);

    // // Request the associated tenant of the user based on the UUID
    // Tenant associated_tenant = BCAPI.getAssociatedTenant(currentUser.getUUID());
    // std::cout << "\t\tOK." << std::endl;

    // std::cout << "\tTenant UUID: " << associated_tenant.getUUID() << std::endl;
    // tenantUUID = associated_tenant.getUUID();

    // // Now using the known tenant UUID, a device can be requested through i.e. a device collection request
    // // To select a specific device, use the request filters
    // std::string aksID_filter = {};
    // std::string description_filter = {};
    // std::string unit_filter = {};

    // EntityCollection<Device> devices = BCAPI.getDevicesCollection(tenantUUID, {}, aksID_filter, description_filter, unit_filter, [](std::exception& e, json& j) {
    //     std::cout << "\tAn error occured while parsing a device entity from the follwing data: " << j.dump() << std::endl << e.what() << std::endl;
    // });    
    // std::cout << "\t\tOK." << std::endl;

    // deviceUUID = devices.first.at(0).getUUID();


    try {
        /** 
         * Creates a new reading in the BASCloud 
         * Parameters are a tenant UUID under which the device and reading is grouped, 
         * a device UUID for which the reading was read, the readings value and current timestamp.
         */
        Reading new_reading = BCAPI.createReading(tenantUUID, deviceUUID, value, currentDateTime);
        std::cout << "\t\tOK." << std::endl;

        std::cout << "Created Reading:" << std::endl;
        std::cout << "\tReading UUID: " << new_reading.getUUID() << std::endl;
        std::cout << "\tReading Value: " << new_reading.getValue() << std::endl;
        std::cout << "\tReading timestamp: " << fmt::format("{:%F %T}", fmt::localtime(new_reading.getTimestamp())) << std::endl;
        std::cout << "\tReading created at: " << fmt::format("{:%F %T}", fmt::localtime(new_reading.getCreatedDate())) << std::endl;
        std::cout << "\tReading updated at: " << fmt::format("{:%F %T}", fmt::localtime(new_reading.getLastUpdatedDate())) << std::endl;

    } catch(ServerError& e) {
        std::cout << "\tServer Error occured." << std::endl << e.what() << std::endl;
        return 1;
    } catch(ConnectionError& e) {
        std::cout << "\tConnection Error occured." << std::endl << e.what() << std::endl;
        return 1;
    } catch(BadRequest& e) {
        std::cout << "\tBad Request. Request contained invalid parameters." << std::endl << e.what() << std::endl;
        // Maybe try again...
        return 1;
    } catch(UnauthorizedRequest& e) {
        std::cout << "\tCurrent user has no authority to create a reading." << std::endl << e.what() << std::endl;
        // Possible the user has no authority to create a reading for this tenant or device.
        return 1;
    } catch(NotFoundRequest& e) {
        std::cout << "\tGiven UUID was not found." << std::endl << e.what() << std::endl;
        // Either the tenant UUID or device UUID was not found.
        return 1;
    } catch(InvalidResponse& e) {
        std::cout << "\tServer responds with invalid." << std::endl << e.what() << std::endl;
        // Maybe try again...
        return 1;
    }

}