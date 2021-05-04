#include <iostream>

#include <fmt/core.h>
#include <fmt/chrono.h>

#include "EntityContext.h"
#include "error/Exceptions.h"


using namespace BAScloud;

int main (int argc, char *argv[]) {

    std::cout << "Initialising library..."<< std::endl;

    // For accessing the BAScloud API, a API endpoint URL needs to be defined (Note: no trailing symbols, must end with TLD identifier)
    EntityContext BCAPI("server_URL");
    std::cout << "\tOK." << std::endl;


    // Before accessing any API methods, a user must be authenticated
    std::cout << "1. - Authentication with user login" << std::endl;

    /**
     * All methods in the BAScloud API library potential throw different exceptions to signal a failure in the API request.
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

    // A setpoint contains a (future) timestamp of the time the setpoint value should be set (as a UNIX timestamp in seconds)
    std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() + 3*60*60*60;

    // For creating a setpoint, its associated tenant UUID is needed
    std::string tenantUUID = "";

    // For creating a setpoint, its associated device UUID is needed describing the device for which a new setpoint is set.
    std::string deviceUUID = "";

    // The setpoint value of the device that is set at the specified timestamp.
    double value = 1234.56;

    try {
        /** 
         * Creates a new setpoint in the BAScloud 
         * Parameters are a tenant UUID under which the device and setpoint is grouped, 
         * a device UUID for which the setpoint is set, the setpoint value and future timestamp.
         */
        SetPoint new_setPoint = BCAPI.createSetPoint(tenantUUID, deviceUUID, 2345.67, currentDateTime);
        std::cout << "\t\tOK." << std::endl;

        std::cout << "Created SetPoint:" << std::endl;
        std::cout << "\tSetPoint UUID: " << new_setPoint.getUUID() << std::endl;
        std::cout << "\tSetPoint Value: " << new_setPoint.getValue() << std::endl;
        std::cout << "\tSetPoint timestamp: " << fmt::format("{:%F %T}", fmt::localtime(new_setPoint.getTimestamp())) << std::endl;
        std::cout << "\tSetPoint created at: " << fmt::format("{:%F %T}", fmt::localtime(new_setPoint.getCreatedDate())) << std::endl;
        std::cout << "\tSetPoint updated at: " << fmt::format("{:%F %T}", fmt::localtime(new_setPoint.getLastUpdatedDate())) << std::endl;

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
        // Possible the user has no authority to create a setpoint for this tenant or device.
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