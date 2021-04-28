#include <iostream>

#include <fmt/core.h>
#include <fmt/chrono.h>

#include "EntityContext.h"


using namespace BASCloud;

int main (int argc, char *argv[]) {

    std::cout << "Initialising library..."<< std::endl;

    // Before accessing the BASCloud API, the correct API endpoint URL needs to be defined (Note: no trailing symbols, must end with TLD identifier)
    EntityContext BCAPI("server_URL");
    std::cout << "\tOK." << std::endl;


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

    // Check if entitycontext has valid authentication, checks expiration
    std::cout << "\tAuthenticated: " << BCAPI.isAuthenticated() << std::endl;

    // Authentication token has an expiration date
    std::cout << fmt::format("\tToken valid until: {:%F %T}", fmt::localtime(BCAPI.getTokenExpirationDate())) << std::endl;

    // Access the authentication token directly
    std::cout << "\tToken: " << BCAPI.getToken() << std::endl;

}