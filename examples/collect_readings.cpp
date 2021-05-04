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


    std::cout << "2. - Collecting all devices" << std::endl;

    // For accessing a device, its associated tenant UUID is needed
    std::string tenantUUID = "";

    /**
     * For requesting a collection of BAScloud readings, filters on their attributes can be applied
     * Readings in a corresponding time-span can be searched for, for a specific timestamp, a specific value, 
     * and a corresponding device UUID.
     * 
     * Filter parameter are optional and can be ommitted, or left empty (default values)
     */
    std::time_t from_filter = -1;
    std::time_t until_filter = -1;
    std::time_t timestamp_filter = -1;
    double value_filter = std::numeric_limits<double>::quiet_NaN();
    std::string deviceUUID_filter = {};

    /**
     * For requesting a collection of BAScloud entities, pagination can be used for large collections.
     * 
     * PagingOption consists of:
     * page_size: number of entries per page.
     * direction: for navigating between pages, next, prev and none.
     * page_pointer: for navigating between pages, points to the next/prev page.
     * 
     * Paging are optional and can be ommitted, or left empty ({})
     */
    PagingOption paging = {};


    try {
        /** 
         * Request a collection of devices from the BAScloud 
         * Parameters are a tenant UUID under which the devices are grouped, 
         * and optional paging and filters.
         * 
         * Each library method returning a collection further supports an error handler function. This function 
         * is called for each entity in the request response for which an error occured e.g. failed to parse due to missing or invalid data.
         * 
         * The function is given the exception which was thrown and the json data which caused the error.
         */
        EntityCollection<Reading> readings = BCAPI.getReadingsCollection(tenantUUID, paging, from_filter, until_filter, timestamp_filter, value_filter, deviceUUID_filter, [](std::exception& e, json& j) {
                throw e;
            });
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tNumber of received readings: " << readings.first.size() << std::endl;

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


    std::cout << "3. - Collecting a single device per UUID" << std::endl;


    // For requesting a reading its UUID is needed.
    std::string readingUUID = "";

    try {
        /** 
         * Requests a single reading from the BAScloud 
         * Parameters are a tenant UUID under which the reading is grouped and the UUID of the reading itself
         */
        std::cout << "\tRequesting single reading with UUID..." << std::endl;

        Reading reading = BCAPI.getReading(tenantUUID, readingUUID);
        std::cout << "\t\tOK." << std::endl;

        std::cout << "\tReading UUID: " << reading.getUUID() << std::endl;
        std::cout << "\tReading Value: " << reading.getValue() << std::endl;
        std::cout << "\tReading timestamp: " << fmt::format("{:%F %T}", fmt::localtime(reading.getTimestamp())) << std::endl;
        std::cout << "\tReading created at: " << fmt::format("{:%F %T}", fmt::localtime(reading.getCreatedDate())) << std::endl;
        std::cout << "\tReading updated at: " << fmt::format("{:%F %T}", fmt::localtime(reading.getLastUpdatedDate())) << std::endl;


    } catch(std::exception& e) {
        std::cout << "\tAn error occured for the request." << std::endl << e.what() << std::endl;
        return 1;
    }

}