#pragma once

#include <limits>
#include <string>
#include <vector>

#include "APIContext.h"
#include "Paging.h"

#include "entity/User.h"
#include "entity/Tenant.h"
#include "entity/Property.h"
#include "entity/Connector.h"
#include "entity/Device.h"


namespace BAScloud {

// Class that abstracts the APIContext and its api endpoint functions

/** 
 * The EntityContext class provides a high-level abstraction to access the BAScloud API endpoint functions.
 * 
 * BAScloud entites are modelled through object-classes and endpoint operations are abstracted as functions. For each function requesting an endpoint, response parsing 
 * and error logic is handled by EntityContext. EntityContext also checks for valid authentication
 * token at each request and, if expired, renews the token automatically.
 * 
 */
class EntityContext {

 private:

	/**
	 * APIContext object providing a low-level access to the BAScloud API.
	 */
	APIContext api_context;

	/**
	 * Authentication token received from the BAScloud after a successfull user login.
	 * This may be empty if not authenticated yet.
	 */
	std::string API_token;

	/**
	 * HTTP/S URL for the BAScloud server. The URL should not end with any symbols e.g. /, ?.
	 */
	std::string API_server_URL;

	/**
	 * Expiration date of the authentication token.
	 */
	std::time_t API_token_valid_until;

    /**
	 * User email for authentication. The variable is empty until the first call to authenticateWithUserLogin().
	 */
	std::string API_login_email;

    /**
	 * User password for the provided email for authentication. The variable is empty until the first call to authenticateWithUserLogin().
	 */
	std::string API_login_password;
	
   /**
    * Parse the cpr::Response object returned by the APIContext functions.
    *
    * Handles potential errors in the request and while parsing the JSON reponse data.
    *
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
	* 
    * @param response Request response returned by the APIContext functions.
	* 
    * @return json object representing the parsed JSON response data.
    */
	json parseResponse(cpr::Response response);

   /**
    * Parse the paging information contained in the JSON response data.
    *
    * Handles potential errors while parsing the JSON reponse data.
    *
    * @throws InvalidResponse
	* 
    * @param response json response returned by the parseResponse() functions.
	* 
    * @return PagingResult object representing the paging information of the API response.
    */
	PagingResult parsePaging(json response);

   /**
    * Checks the provided UUID string for validity.
    *
    * A UUID must be a 36 character long string e.g. "a2e411ab-9543-41cf-bb06-7465e1cdd0dc"
    * 
    * If the given string does not hold this format and length an exception is thrown.
    *
    * @throws std::invalid_argument
	* 
    * @param UUID BAScloud API entity UUID string.
	* 
    */
    void validateUUID(std::string UUID);

   /**
    * Checks the current authentication token and renews it if necessary.
    * 
    * The function will the token format and expiration and if invalid will try to renew the authentication
    * using the previously used login data (Note: login data is hold in memory for as long as the EntityContext object exists)
    * 
    * This function internal calls authenticateWithUserLogin() which may throw different exceptions on failure.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
	* 
    */
    void checkAndRenewAuthentication();


 public:

   /**
    * EntityContext constructor
    *
    * Creates a EntityContext given the server URL for the BAScloud API. 
    *
    * Note: For accessing methods annotated with [Admin], a backend server URL is 
	* needed instead of gateway URL (Discuss this with the BAScloud provider).
	* 
    * @param API_server_URL BAScloud server URL, must end with the TLD identifier e.g. .com, .de etc. 
	* (No end symbols allowed e.g. /, ?)
    */
	EntityContext(std::string API_server_URL);

   /**
    * Authenticate a user against the BAScloud API.
    *
    * Given a correct email password combination the API returns an bearer authentication token which is used
	* for the upcoming requests. The token is accessible through getToken(). The token expires 
	* after some time, the expiration date is accessed through getTokenExpirationDate(). 
	* Invalid email password combination throws 
    *
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
	* 
    * @param API_email Email address of an registered user.
    * @param API_password Matching user password to the provided email address.
	* 
    */
	void authenticateWithUserLogin(std::string API_email, std::string API_password);

  /**
    * Authenticate a connector using a valid connector token.
    *
    * Given a valid connector token previusly requested from the BAScloud through a connector creation or 
    * getNewConnectorAuthToken(). The connector token has no expiration date, getTokenExpirationDate() will return 
    * a date long in the future (year > 10000).
    * 
    * Note: Previously date for no expiration date was set to long::max, however, this breaks localtime functions this a "real" date is used.
    * 
    * When a new connector token is requested for authenticating a connector in the future, authenticateWithConnectorToken 
    * must be called again with the new token.
    *
    * @param API_connector_token Valid connector api token.
	* 
    */
	void authenticateWithConnectorToken(std::string API_connector_token);


	/**
	 * Get the authentication token. 
	 * 
	 * @return Authentication token string, empty if not authenticated.
	 */
	std::string getToken();

	/**
	 * Get the expiration date of the authentication token.
     * 
     * When a connector token is used to authentication, the expiration date is never modelled by long::MAX.
	 * 
	 * @return UNIX timestamp of the expiration date or long::MAX for connector tokens (expires never).
	 */
	std::time_t getTokenExpirationDate();

	/**
	 * Returns wherever the current EntityContext is authenticated.
	 * 
	 * For the context to be authenticated a valid token is needed (not expired).
	 * 
	 * @return True if the context is currently authenticated, false otherwise.
	 */
	bool isAuthenticated();

	// User API endpoints

   /**
    * Request a single User entity.
    * 
    * A User is uniquely identified by a BAScloud Tenant UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_user_UUID UUID of the BAScloud User.
    * 
    * @return A User object representing the BAScloud User with the specified UUID.
    */
	User getUser(std::string API_user_UUID);

  /**
    * Request a collection of User entities.
    * 
    * Returns all User the currently authenticated user is authorized to view. This may be only itself.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param email Optional value filter for the user email.
    * @param paging Optional PagingOption that is used for requesting paged API results. 
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * Note: Paging for UsersCollection endpoint is currently not implemented. All results will be returned in a single page.
    * 
    * @return EntityCollection containing list of User entities and empty paging information (See note).
    */
	EntityCollection<User> getUsersCollection(std::string email={}, PagingOption paging={}, std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});

   /**
    * Get a the associated Tenant entity of the User.
    * 
    * You may be only authorized to view the associated Tenant of the currently authenticated User.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
	* 
    * @param API_user_UUID UUID of the BAScloud User.
    * 
    * @return A Tenant object representing the BAScloud Tenant associated with the User.
    */
	Tenant getAssociatedTenant(std::string API_user_UUID);

   /**
    * Request a password reset for the User.
    * 
    * If the User exists in the BAScloud, an email is sent upon the request containing a reset token.
    * The received token can then be used in updateUserPassword() to change the User password to a new value.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
	* 
    * @param email Email address of the new User.
	* 
    */
	void requestUserPasswordReset(std::string email);

   /**
    * Changes the password of a User.
    * 
    * Prior to calling this function, a reset token should be requested using requestUserPasswordReset().
    * Given the valid token, a new password can be set for the User.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_user_UUID UUID of the BAScloud User.
    * @param reset_token Valid reset token received from the BAScloud upon requestUserPasswordReset() request.
    * @param new_password New password value for the User.
	* 
    */
	void updateUserPassword(std::string API_user_UUID, std::string reset_token, std::string new_password);

  /**
    * Create a new User entity in the BAScloud.
    * 
    * A new User is created using the given email and password combination.
    * 
    * After creation, the new User is not assigned to any Tenant. User Tenant::assignUser() to assign the User.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param email Email address of the new User.
    * @param password Password of the new User.
    * 
    * @return User entity object representing the newly created BAScloud User.
    */
	User createNewUser(std::string email, std::string password);

  /**
    * Updates the information of a User entity in the BAScloud. [Admin] 
    * 
    * All parameters are optional. This operation needs administration authority.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_user_UUID User entity UUID that is supposed to be updated.
    * @param email Optional new value for the User email.
    * 
    * @return User entity object representing the updated BAScloud User.
    */
	User updateUser(std::string API_user_UUID, std::string email);

  /**
    * Deletes an existing User in the BAScloud. [Admin] 
    * 
    * The request deletes a User entity in the BAScloud based on the given User UUID. 
    * This operation needs administration authority.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_user_UUID UUID of the BAScloud User.
	* 
    */
	void deleteUser(std::string API_user_UUID);

	// Tenant API endpoints

   /**
    * Request a single Tenant entity.
    * 
    * A Tenant is uniquely identified by a BAScloud Tenant UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the BAScloud Tenant.
    * 
    * @return A Tenant object representing the BAScloud Tenant with the specified UUID.
    */
	Tenant getTenant(std::string API_tenant_UUID);

   /**
    * Request a collection of Tenant entities.
    * 
    * Returns all Tenants the currently authenticated user is authorized to view. This is 
	* probably only its own tenant.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param paging Optional PagingOption that is used for requesting paged API results. 
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * Note: Paging for TenantsCollection endpoint is currently not implemented. All results will be returned in a single page.
    * 
    * @return EntityCollection containing list of Tenant entities and empty paging information (See note).
    */
	EntityCollection<Tenant> getTenantsCollection(PagingOption paging={}, std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});

  /**
    * Get a collection of associated User entities of the Tenant.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the BAScloud Tenant.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * @return EntityCollection containing list of User entities and paging information.
    */
	EntityCollection<User> getAssociatedUsers(std::string API_tenant_UUID, PagingOption paging={}, std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});

   /**
    * Creates a new Tenant entity in the BAScloud. [Admin] 
    * 
    * A new Tenant is created using the given Tenant parameter. This operation needs 
    * administration authority. 
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param name Name of the new Tenant
    * @param API_user_UUID User entity UUID that is supposed to be associated with the new Tenant.
    * 
    * @return Tenant entity object representing the newly created BAScloud Tenant.
    */
	Tenant createTenant(std::string name, std::string API_user_UUID);

  /**
    * Deletes an existing Tenant in the BAScloud. [Admin] 
    * 
    * The request deletes a Tenant entity in the BAScloud based on the given Tenant UUID. 
    * This operation needs administration authority. 
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the BAScloud Tenant.
	* 
    */
	void deleteTenant(std::string API_tenant_UUID);

  /**
    * Updates the information of a Tenant entity in the BAScloud. [Admin] 
    * 
    * All parameters are optional. This operation needs administration authority. 
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID Tenant entity UUID that is supposed to be updated.
    * @param name Optional new value for the Tenant name.
    * 
    * @return Tenant entity object representing the updated BAScloud Tenant.
    */
	Tenant updateTenant(std::string API_tenant_UUID, std::string name={});

  /**
    * Assigns a collection of User entities to a Tenant. [Admin] 
    * 
    * This operation is only permitted to Users with administrator rights for the Tenant.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID Tenant entity UUID to assign the users to.
    * @param API_user_UUIDs Collection of User entity objects that are to be assigned to the Tenant.
    */
	void assignTenantUsers(std::string API_tenant_UUID, std::vector<std::string> API_user_UUIDs);

  /**
    * Removes a collection of User entities from a Tenant. [Admin] 
    * 
    * This operation needs administration authority.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID Tenant entity UUID from where to remove the users.
    * @param API_user_UUIDs Collection of User entity objects that are to be removed from the Tenant.
    */
	void removeTenantUsers(std::string API_tenant_UUID, std::vector<std::string> API_user_UUIDs);

	// Property API endpoints

   /**
    * Request a single Property entity.
    * 
    * A Property is uniquely identified by the associated Tenant and Property UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Property.
    * @param API_property_UUID UUID of the represented BAScloud Property.
    * 
    * @return A Property object representing the BAScloud Property with the specified UUID.
    */
	Property getProperty(std::string API_tenant_UUID, std::string API_property_UUID);

   /**
    * Request a collection of Property entities grouped under the given Tenant.
    * 
    * The request filters the BAScloud Property based on the given parameters and returns a collection 
    * of Property matching these values.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param name Optional filter for the name of the Device.
    * @param street Optional filter for the street of the Property address.
    * @param postalCode Optional filter for the postal code of the Property address.
    * @param city Optional filter for the city of the Property address.
    * @param country Optional filter for the country of the Property address.
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * @return EntityCollection containing list of Property entities matching the provided filters and paging information.
    */
   	EntityCollection<Property> getPropertiesCollection(std::string API_tenant_UUID, PagingOption paging={}, std::string name={}, 
		std::string street={}, std::string postalCode={}, std::string city={}, std::string country={}, std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});
	
   /**
    * Get a collection of associated Connector entities of a Property.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Property.
    * @param API_property_UUID UUID of the BAScloud Property.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * @return EntityCollection containing a list of Connector entities associated with the Property and paging information.
    */
	EntityCollection<Connector> getAssociatedConnectors(std::string API_tenant_UUID, std::string API_property_UUID, PagingOption paging={}, 
        std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});
	
   /**
    * Create a new Property entity in the BAScloud.
    * 
    * Given the associated Tenant a new Property is created using the given Property parameter.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param name Name of the Property.
    * @param street Street of the Property address.
    * @param postalCode Postal code of the Property address.
    * @param city City of the Property address.
    * @param country Country of the Property address.
    * 
    * @return Property entity object representing the newly created BAScloud Property.
    */
	Property createProperty(std::string API_tenant_UUID, std::string name, std::string street, std::string postalCode, std::string city, std::string country);
	
   /**
    * Deletes an existing Property in the BAScloud.
    * 
    * The request deletes a Property entity in the BAScloud based on the given Property UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Property.
    * @param API_property_UUID UUID of the existing BAScloud Property that is supposed to be deleted.
	* 
    */
	void deleteProperty(std::string API_tenant_UUID, std::string API_property_UUID);
	
   /**
    * Update an existing Property in the BAScloud.
    * 
    * The request updates attributes of an existing BAScloud Property based on the given Property UUID and returns 
    * a new Property object representing the updated entity.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Property.
    * @param API_property_UUID UUID of the existing BAScloud Property that is supposed to be updated.
    * @param name Optional new value for the name of the Device.
    * @param street Optional new value for the street of the Property address.
    * @param postalCode Optional new value for the postal code of the Property address.
    * @param city Optional new value for the city of the Property address.
    * @param country Optional new value for the country of the Property address.
    * 
    * @return Property entity object representing the updated BAScloud Property.
    */
	Property updateProperty(std::string API_tenant_UUID, std::string API_property_UUID, std::string name={}, std::string street={}, 
		std::string postalCode={}, std::string city={}, std::string country={});

	// Connector API endpoints

   /**
    * Request a single Connector entity.
    * 
    * A Connector is uniquely identified by the associated Tenant and Connector UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Connector.
    * @param API_connector_UUID UUID of the represented BAScloud Connector.
    * 
    * @return A Connector object representing the BAScloud Connector with the specified UUID.
    */
	Connector getConnector(std::string API_tenant_UUID, std::string API_connector_UUID);
   	
   /**
    * Request a collection of Connector entities grouped under the given Tenant.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * @return EntityCollection containing list of Connector entities matching the provided filters and paging information.
    */
	EntityCollection<Connector> getConnectorsCollection(std::string API_tenant_UUID, PagingOption paging={}, 
        std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});
	
   /**
    * Get the associated Property entity of the Connector.
    * 
    * Each Connector can have a relation to one Property.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
	* 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Connector.
    * @param API_connector_UUID UUID of the represented BAScloud Connector.
    * 
    * @return Associated Property entity object
    */
	Property getAssociatedProperty(std::string API_tenant_UUID, std::string API_connector_UUID);
	
    /**
    * Get the associated Device entities of the Connector.
    * 
    * Each Connector can have multiple associated Devices.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
	* 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Connector.
    * @param API_connector_UUID UUID of the represented BAScloud Connector.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * @return EntityCollection containing list of Device entities associated with the Connector.
    */
	EntityCollection<Device> getAssociatedDevices(std::string API_tenant_UUID, std::string API_connector_UUID, PagingOption paging={}, 
        std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});
	
   /**
    * Create a new Connector entity in the BAScloud.
    * 
    * Given the associated Tenant and Property a new Connector is created using the given Connector parameter.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Connector.
    * @param API_property_UUID UUID of the associated BAScloud Property of the Connector.
    * @param name The name of the new Connector.
    * 
    * @return Connector entity object representing the newly created BAScloud Connector.
    */
	Connector createConnector(std::string API_tenant_UUID, std::string API_property_UUID, std::string name);

   /**
    * Deletes an existing Connector in the BAScloud.
    * 
    * The request deletes a Connector entity in the BAScloud based on the given Connector UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Connector.
    * @param API_connector_UUID UUID of the existing BAScloud Connector that is supposed to be deleted.
	* 
    */
	void deleteConnector(std::string API_tenant_UUID, std::string API_connector_UUID);
	
   /**
    * Update an existing Connector in the BAScloud.
    * 
    * The request updates attributes of an existing BAScloud Connector based on the given Connector UUID and returns 
    * a new Connector object representing the updated entity.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Connector.
    * @param API_connector_UUID UUID of the existing BAScloud Connector that is supposed to be updated.
    * @param name Optional new name of the new Connector.
    * 
    * @return Connector entity object representing the updated BAScloud Connector.
    */	
	Connector updateConnector(std::string API_tenant_UUID, std::string API_connector_UUID, std::string name={});
	
   /**
    * Requests a new API key for a Connector entity.
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Connector.
    * @param API_connector_UUID UUID of a BAScloud Connector for which the API key is requested.
    * 
    * @return API key token for the Connector entity. A Connector API key does not expire.
    */
	std::string getNewConnectorAuthToken(std::string API_tenant_UUID, std::string API_connector_UUID);

	// Device API endpoints

   /**
    * Request a single Device entity.
    * 
    * A Device is uniquely identified by the associated Tenant and Device UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Device.
    * @param API_device_UUID UUID of the represented BAScloud Device.
    * 
    * @return A Device object representing the BAScloud Device with the specified UUID.
    */
	Device getDevice(std::string API_tenant_UUID, std::string API_device_UUID);
	
   /**
    * Request a collection of Device entities grouped under the given Tenant.
    * 
    * The request filters the BAScloud Devices based on the given parameters and returns a collection 
    * of Devices matching these values.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param aksID Optional filter for the AKS ID of the Device.
    * @param description Optional filter for the Description of the Device.
    * @param unit Optional filter for the measuring unit of the Device.
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * @return EntityCollection containing list of Device entities matching the provided filters and paging information.
    */
	EntityCollection<Device> getDevicesCollection(std::string API_tenant_UUID, PagingOption paging={}, std::string aksID={}, 
		std::string description={}, std::string unit={}, std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});
	
   /**
    * Get the associated Connector entity of the Device.
    * 
    * Each Device has a mandatory relation to one and only one Connector.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
	* 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param API_device_UUID UUID of the BAScloud Device.
    * 
    * @return Connector entity associated with the specified Device.
    */
	Connector getAssociatedConnector(std::string API_tenant_UUID, std::string API_device_UUID);
	
   /**
    * Get a collection of associated Reading entities of the Device.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param API_device_UUID UUID of the BAScloud Device.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * @return EntityCollection containing list of Reading entities and paging information.
    */
	EntityCollection<Reading> getAssociatedReadings(std::string API_tenant_UUID, std::string API_device_UUID, PagingOption paging={}, 
        std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});
	
   /**
    * Get a collection of associated Setpoints entities of the Device.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param API_device_UUID UUID of the BAScloud Device.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * @return EntityCollection containing list of SetPoint entities and paging information.
    */
	EntityCollection<SetPoint> getAssociatedSetPoints(std::string API_tenant_UUID, std::string API_device_UUID, PagingOption paging={}, 
        std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});
	
   /**
    * Create a new Device entity in the BAScloud.
    * 
    * Given the associated Tenant and Connector a new Device is created using the given Device parameter.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Device.
    * @param API_connector_UUID UUID of the associated BAScloud Tenant of the Device.
    * @param aksID The AKS ID of the new Device.
    * @param description The Description of the new Device.
    * @param unit The measuring unit of the new Device.
    * 
    * @return Device entity object representing the newly created BAScloud Device.
    */
	Device createDevice(std::string API_tenant_UUID, std::string API_connector_UUID, std::string aksID, std::string description, std::string unit);
	
   /**
    * Update an existing Device in the BAScloud.
    * 
    * The request updates attributes of an existing BAScloud Device based on the given Device UUID and returns 
    * a new Device object representing the updated entity.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Device.
    * @param API_device_UUID UUID of the existing BAScloud Device that is supposed to be updated.
    * @param aksID Optional new value for the AKS ID of the Device.
    * @param description Optional new value for the Description of the Device.
    * @param unit Optional new value for the measuring unit of the Device.
    * 
    * @return Device entity object representing the updated BAScloud Device.
    */
	Device updateDevice(std::string API_tenant_UUID, std::string API_device_UUID, std::string aksID={}, std::string description={}, std::string unit={});
	
   /**
    * Deletes an existing Device in the BAScloud.
    * 
    * The request deletes a Device entity in the BAScloud based on the given Device UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Device.
    * @param API_device_UUID UUID of the existing BAScloud Device that is supposed to be deleted.
    * 
    */
	void deleteDevice(std::string API_tenant_UUID, std::string API_device_UUID);

	// Reading API endpoints

   /**
    * Request a single Reading entity.
    * 
    * A Reading is uniquely identified by the associated Tenant and Reading UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Reading.
    * @param API_reading_UUID UUID of the represented BAScloud Reading.
    * 
    * @return A Reading object representing the BAScloud Reading with the specified UUID.
    */
	Reading getReading(std::string API_tenant_UUID, std::string API_reading_UUID);
	
   /**
    * Request a collection of Reading entities grouped under the given Tenant.
    * 
    * The request filters the BAScloud Reading based on the given parameters and returns a collection 
    * of Reading matching these values.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param from Optional filter defining the start time of a time-range the requested Readings should lie in.
    * @param until Optional filter defining the end time of a time-range the requested Readings should lie in.
    * @param timestamp Optional filter for the timestamp of the Reading.
    * @param value Optional filter for the value of the Reading.
    * @param API_device_UUID Optional filter for the associated Device UUID of the Reading.
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * @return EntityCollection containing list of Property entities matching the provided filters and paging information.
    */
	EntityCollection<Reading> getReadingsCollection(std::string API_tenant_UUID, PagingOption paging={}, std::time_t from=-1, std::time_t until=-1, 
		std::time_t timestamp=-1, double value=std::numeric_limits<double>::quiet_NaN(), std::string API_device_UUID={}, 
        std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});
	
   /**
    * Get the associated Device entity of the Reading.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
	* @param API_tenant_UUID UUID of the associated BAScloud Tenant.
	* @param API_reading_UUID UUID of the Reading entity.
	* 
    * @return Device entity object of the associated BAScloud Device.
    */
	Device getAssociatedDevice(std::string API_tenant_UUID, std::string API_reading_UUID);
	
   /**
    * Create a new Reading entity in the BAScloud.
    * 
    * Given the associated Tenant and Device entity, a new Reading is created using the given Reading parameter.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Reading.
    * @param API_device_UUID UUID of the associated BAScloud Device of the Reading.
    * @param value Value of the reading
    * @param timestamp The time of the reading of the entity value.
    * 
    * @return Reading entity object representing the newly created BAScloud Reading.
    */
	Reading createReading(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp);
	
   /**
    * Deletes an existing Reading in the BAScloud. [Admin] 
    * 
    * The request deletes a Reading entity in the BAScloud based on the given Reading UUID.
    * This operation needs administration authority. 
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the Reading.
    * @param API_reading_UUID UUID of the existing BAScloud Reading that is supposed to be deleted.
    */
	void deleteReading(std::string API_tenant_UUID, std::string API_reading_UUID);

	// SetPoint API endpoints

   /**
    * Request a single SetPoint entity.
    * 
    * A SetPoint is uniquely identified by the associated Tenant and SetPoint UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the SetPoint.
    * @param API_setpoint_UUID UUID of the represented BAScloud SetPoint.
    * 
    * @return A SetPoint object representing the BAScloud SetPoint with the specified UUID.
    */
	SetPoint getSetPoint(std::string API_tenant_UUID, std::string API_setpoint_UUID);
	
  /**
    * Request a collection of SetPoint entities grouped under the given Tenant.
    * 
    * The request filters the BAScloud SetPoint based on the given parameters and returns a collection 
    * of SetPoint matching these values.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param from Optional filter defining the start time of a time-range the requested SetPoint should lie in.
    * @param until Optional filter defining the end time of a time-range the requested SetPoint should lie in.
    * @param timestamp Optional filter for the timestamp of the SetPoint.
    * @param currentTime Optional filter for the current time.
    * @param API_device_UUID Optional filter for the associated Device UUID of the SetPoint.
    * @param errorHandler Optional callback function for handling errors in the request. Function gets occured exception and invalid data.
    * 
    * @return EntityCollection containing list of SetPoint entities matching the provided filters and paging information.
    */
	EntityCollection<SetPoint> getSetPointsCollection(std::string API_tenant_UUID, PagingOption paging={}, std::time_t from=-1, std::time_t until=-1, 
		std::time_t timestamp=-1, std::time_t currentTime=-1, std::string API_device_UUID={}, 
        std::function<void (std::exception&, json&)> errorHandler=[](std::exception& e, json& j){});
	
   /**
    * Create a new SetPoint entity in the BAScloud.
    * 
    * Given the associated Tenant and Device entity, a new SetPoint is created using the given SetPoint parameter.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the SetPoint.
    * @param API_device_UUID UUID of the associated BAScloud Device of the SetPoint.
    * @param value Value of the SetPoint
    * @param timestamp The time of the SetPoint value.
    * 
    * @return SetPoint entity object representing the newly created BAScloud SetPoint.
    */
	SetPoint createSetPoint(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp);

};

}