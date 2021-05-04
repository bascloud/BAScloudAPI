#pragma once

#include <string>
#include <chrono>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <cpr/cpr.h>


namespace BAScloud {

/** 
 * BAScloud API context is a 1:1 implementation of the unabstracted API endpoints.
 * 
 * Methods in this class directly request the corresponding BAScloud API endpoint and 
 * return its unmodified response e.g. including error responses and raw JSON formatted response data.
 * 
 * It is recommended to use the abstracted class EntityContext which already handles error responses and JSON data parsing. 
 * For working with raw response data one may use this class directly.
 * 
 */
class APIContext {

 private:

   /**
	 * URL of the BAScloud API server. 
	 * URL should include protocol defintions e.g. http:// or https://.
	 */
 	std::string API_server_URL;

   /**
	 * API authentication token. Prior to authentication this may be empty.
	 * After authentication this variable holds the returned bearer token. 
	 * 
	 * Note: The token may expire after some time, this is not handled in this class. See EntityContext class for expiration handling.
	 */
	std::string API_token;

   /**
	 * API Endpoint URL paths.
	 * 
	 * Relative paths to the API_server_URL for the different resource endpoints of the BAScloud API.
	 * 
	 * Note: Adminstration resources are included which may not be possible to access without special authority.
	 */

	const std::string API_AUTHENTICATION_PATH = "/login";

	const std::string API_USER_SIGNUP_PATH = "/signup";
	const std::string API_USER_RESET_PASSWORD_PATH = "/resetPassword";
	const std::string API_USER_CHANGE_PASSWORD_PATH = "/changePassword";

	const std::string API_USER_SINGLE_PATH = "/users/{}";
	const std::string API_USER_COLLECTION_PATH = "/users";
	const std::string API_USER_TENANT_RELATIONSHIP_PATH = "/users/{}/relationships/tenant";
	const std::string API_USER_ASSOCIATED_TENANT_PATH = "/users/{}/tenant";
	const std::string API_USER_DELETE_PATH = "/users/{}";

	const std::string API_TENANT_SINGLE_PATH = "/tenants/{}";
	const std::string API_TENANT_COLLECTION_PATH = "/tenants";
	const std::string API_TENANT_USERS_RELATIONSHIP_PATH = "/tenants/{}/relationships/users";
	const std::string API_TENANT_ASSOCIATED_USERS_PATH = "/tenants/{}/users";
	const std::string API_TENANT_CREATE_PATH = "/tenants";
	const std::string API_TENANT_UPDATE_DELETE_PATH = "/tenants/{}";
	const std::string API_TENANT_ASSIGN_REMOVE_USERS_PATH = "/tenants/{}/relationships/users";

	const std::string API_PROPERTY_SINGLE_PATH = "/tenants/{}/properties/{}";
	const std::string API_PROPERTY_COLLECTION_PATH = "/tenants/{}/properties";
	const std::string API_PROPERTY_CONNECTORS_RELATIONSHIP_PATH = "/tenants/{}/properties/{}/relationships/connectors";
	const std::string API_PROPERTY_ASSOCIATED_CONNECTORS_PATH = "/tenants/{}/properties/{}/connectors";
	const std::string API_PROPERTY_CREATE_PATH = "/tenants/{}/properties";
	const std::string API_PROPERTY_UPDATE_DELETE_PATH = "/tenants/{}/properties/{}";

	const std::string API_CONNECTOR_SINGLE_PATH = "/tenants/{}/connectors/{}";
	const std::string API_CONNECTOR_COLLECTION_PATH = "/tenants/{}/connectors";
	const std::string API_CONNECTOR_PROPERTY_RELATIONSHIP_PATH = "/tenants/{}/connectors/{}/relationships/property";
	const std::string API_CONNECTOR_ASSOCIATED_PROPERTY_PATH = "/tenants/{}/connectors/{}/property";
	const std::string API_CONNECTOR_DEVICES_RELATIONSHIP_PATH = "/tenants/{}/connectors/{}/relationships/devices";
	const std::string API_CONNECTOR_ASSOCIATED_DEVICES_PATH = "/tenants/{}/connectors/{}/devices";
	const std::string API_CONNECTOR_CREATE_PATH = "/tenants/{}/connectors";
	const std::string API_CONNECTOR_UPDATE_DELETE_PATH = "/tenants/{}/connectors/{}";
	const std::string API_CONNECTOR_GENERATE_TOKEN_PATH = "/tenants/{}/connectors/{}/generateToken";

	const std::string API_DEVICE_SINGLE_PATH = "/tenants/{}/devices/{}";
	const std::string API_DEVICE_COLLECTION_PATH = "/tenants/{}/devices";
	const std::string API_DEVICE_CONNECTOR_RELATIONSHIP_PATH = "/tenants/{}/devices/{}/relationships/connector";
	const std::string API_DEVICE_ASSOCIATED_CONNECTOR_PATH = "/tenants/{}/devices/{}/connector";
	const std::string API_DEVICE_READINGS_RELATIONSHIP_PATH = "/tenants/{}/devices/{}/relationships/readings";
	const std::string API_DEVICE_ASSOCIATED_READINGS_PATH = "/tenants/{}/devices/{}/readings";
	const std::string API_DEVICE_SETPOINTS_RELATIONSHIP_PATH = "/tenants/{}/devices/{}/relationships/setpoints";
	const std::string API_DEVICE_ASSOCIATED_SETPOINTS_PATH = "/tenants/{}/devices/{}/setpoints";
	const std::string API_DEVICE_CREATE_PATH = "/tenants/{}/devices";
	const std::string API_DEVICE_UPDATE_DELETE_PATH = "/tenants/{}/devices/{}";

	const std::string API_READING_SINGLE_PATH = "/tenants/{}/readings/{}";
	const std::string API_READING_COLLECTION_PATH = "/tenants/{}/readings";
	const std::string API_READING_DEVICE_RELATIONSHIP_PATH = "/tenants/{}/readings/{}/relationships/device";
	const std::string API_READING_ASSOCIATED_DEVICE_PATH = "/tenants/{}/readings/{}/device";
	const std::string API_READING_CREATE_PATH = "/tenants/{}/readings";
	const std::string API_READING_DELETE_PATH = "/tenants/{}/readings/{}";

	const std::string API_SETPOINT_SINGLE_PATH = "/tenants/{}/setpoints/{}";
	const std::string API_SETPOINT_COLLECTION_PATH = "/tenants/{}/setpoints";
	const std::string API_SETPOINT_CREATE_PATH = "/tenants/{}/setpoints";

 public:

	/**
	 * APIContext constructor
	 *
	 * Creates an APIContext object providing methods for accessing BAScloud API endpoints and their raw response data.
	 *
	 * @param API_server_URL HTTP/S URL of the BAScloud API instance.
	 */
	APIContext(std::string API_server_URL);

	/**
	 * Set the URL of the BAScloud API instance.
	 *
	 * URL should include protocol defintions e.g. http:// or https://.
	 *
	 * @param API_server_URL HTTP/S URL of the BAScloud API instance.
	 */
	void setAPIURL(std::string API_server_URL);

	/**
	 * Get the currently used URL of the BAScloud API instance.
	 *
	 * @return HTTP/S URL of the BAScloud API instance.
	 */
	std::string getAPIURL();

	/**
	 * Set the authentication token.
	 *
	 * @param token Authentication token.
	 */
	void setToken(std::string token);

   /**
	 * Get the currently used authentication token for the API requests.
	 * 
	 * Note: The token may expire after some time, this is not handled in this class. See EntityContext class for expiration handling.
	 *
	 * @return Authentication token, empty string when not authenticated.
	 */
	std::string getToken();

	// Authentication API endpoints

   /**
	 * Request the authentication of a user per login credentials.
	 * 
	 * A successfull response contains the authentication bearer token and its expiration date.
	 *
	 * @param API_email Email address of an registered user.
	 * @param API_password Corresponding password for the given user email.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestAuthenticationLogin(std::string API_email, std::string API_password);

	// Users API endpoints

   /**
	 * Request a single user entity by UUID.
	 * 
	 * A successfull response contains entity data for the API user.
	 *
	 * @param API_user_UUID API UUID of the user entity.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestUser(std::string API_user_UUID);

   /**
	 * Request a user collection with optional filter.
	 * 
	 * Email filter is optional, request returns all user entities the current user is authorized to access 
	 * (Most likely only the user itself).
	 *
	 * @param email Optional email filter.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestUserCollection(std::string email={});

   /**
	 * Request the user tenant relationship of a given user.
	 * 
	 * @param API_user_UUID API UUID of the user entity.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestUserTenantRelationship(std::string API_user_UUID);

	/**
	 * Request the associated tenant of a given user.
	 * 
	 * @param API_user_UUID API UUID of the user entity.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestUserAssociatedTenant(std::string API_user_UUID);


	// Admin User endpoints

   /**
	 * Request to signup a new user. 
	 * 
	 * A registration email will be send to the specified email address.
	 * 
	 * Note: This endpoint does not need any authentication.
	 * 
	 * @param email Email address for the new user.
	 * @param password Corresponding password for the new user.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestUserSignup(std::string email, std::string password);

   /**
	 * Request to password reset for an existing user. 
	 * 
	 * Password reset is a two-step process. First this method requests a reset after which 
	 * a reset email will be send to the user. The email contains a reset token which is used 
	 * in the requestUserPasswordChange() function to change the password to a new value.
	 * 
	 * Note: This endpoint does not need any authentication.
	 * 
	 * @param email Email address for an existing user.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestUserPasswordReset(std::string email);

   /**
	 * Request to change the password for an existing user. 
	 * 
	 * Password reset is a two-step process. requestUserPasswordReset() requests a reset after which 
	 * a reset email will be send to the user. The email contains a reset token which is used 
	 * in this function to change the password to a new value.
	 * 
	 * Note: This endpoint does not need any authentication.
	 * 
	 * @param API_user_UUID API UUID of an existing user.
	 * @param API_reset_token Password reset token received per email upon the requestUserPasswordReset() call.
	 * @param new_password Value for the new password of the user.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestUserPasswordChange(std::string API_user_UUID, std::string API_reset_token, std::string new_password);

   /**
	 * Request to update information of an existing user. [Admin] 
	 * 
	 * Current authenticated user needs authorization for changing user data.
	 * 
	 * @param API_user_UUID API UUID of an existing user which's data is to be changed.
	 * @param email Updated value for the email address of an existing user.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestUpdateUser(std::string API_user_UUID, std::string email);

   /**
	 * Request the deletion of an existing user. [Admin] 
	 * 
	 * Current authenticated user needs authorization for deleting user.
	 * 
	 * @param API_user_UUID API UUID of an existing user which is to be deleted.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeleteUser(std::string API_user_UUID);

	// Tenants API endpoints

   /**
	 * Request a single tenant entity by UUID.
	 * 
	 * A successfull response contains entity data for the API tenant.
	 *
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestTenant(std::string API_tenant_UUID);

   /**
	 * Request a tenant collection.
	 * 
	 * Request returns all tenant entities the current user is authorized to access 
	 * (Most likely only its own tenant).
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestTenantCollection();

   /**
	 * Request the tenant users relationships of a given tenant.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestTenantUsersRelationship(std::string API_tenant_UUID);

   /**
	 * Request the associated users of a given tenant with optional paging.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param page_size Optional page size for the request e.g. maximal number of entries per page.
	 * @param page_before Optional page identifier pointing to the previous page.
	 * @param page_after Optional page identifier pointing to the next page.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestTenantAssociatedUsers(std::string API_tenant_UUID, int page_size=-1, std::string page_before={}, std::string page_after={});


	// Admin Tenants API endpoints

   /**
	 * Request the deletion of a tenant entity. [Admin] 
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeleteTenant(std::string API_tenant_UUID);

   /**
	 * Request the creation of a tenant entity. [Admin] 
	 * 
	 * Set the user administrator of the new tenant.
	 * 
	 * @param name Name of the new Tenant.
	 * @param API_user_UUID API UUID of the User entity that will be administrator of the new tenant.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestCreateTenant(std::string name, std::string API_user_UUID);

   /**
	 * Request the information update of a tenant entity. [Admin] 
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param name Name of the tenant.
	 * 
     * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestUpdateTenant(std::string API_tenant_UUID, std::string name);

  /**
    * Request to assign a collection of User entities to a tenant. [Admin] 
    * 
    * @param API_tenant_UUID tenant entity UUID to assign the users to.
    * @param API_user_UUIDs Collection of User UUIDs that are to be assigned to the tenant.
	* 
    * @return cpr::Response object representing the raw request response.
    */
	cpr::Response requestAssignTenantUsers(std::string API_tenant_UUID, std::vector<std::string> API_user_UUIDs);

  /**
    * Request to remove a collection of User entities from a tenant. [Admin] 
    * 
    * @param API_tenant_UUID tenant entity UUID to assign the users to.
    * @param API_user_UUIDs Collection of User UUIDs that are to be removed from the tenant.
	* 
    * @return cpr::Response object representing the raw request response.
    */
	cpr::Response requestRemoveTenantUsers(std::string API_tenant_UUID, std::vector<std::string> API_user_UUIDs);

	// Properties API endpoints

   /**
	 * Request a single property entity by UUID.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_property_UUID API UUID of the property entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestProperty(std::string API_tenant_UUID, std::string API_property_UUID);

   /**
	 * Request a property collection with optional filter.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param name Optional name filter.
	 * @param street Optional street filter.
	 * @param postalCode Optional postalCode filter.
	 * @param city Optional city filter.
	 * @param country Optional country filter.
	 * @param page_size Optional page size for the request e.g. maximal number of entries per page.
	 * @param page_before Optional page identifier pointing to the previous page.
	 * @param page_after Optional page identifier pointing to the next page.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestPropertyCollection(std::string API_tenant_UUID, std::string name={}, std::string street={}, std::string postalCode={}, std::string city={}, std::string country={}, int page_size=-1, std::string page_before={}, std::string page_after={});
	
   /**
	 * Request the connectors relationships of a given property.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_property_UUID API UUID of the property entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestPropertyConnectorsRelationship(std::string API_tenant_UUID, std::string API_property_UUID);
	
	/**
	 * Request the associated connectors of a given property.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_property_UUID API UUID of the property entity.
	 * @param page_size Optional page size for the request e.g. maximal number of entries per page.
	 * @param page_before Optional page identifier pointing to the previous page.
	 * @param page_after Optional page identifier pointing to the next page.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestPropertyAssociatedConnectors(std::string API_tenant_UUID, std::string API_property_UUID, int page_size=-1, std::string page_before={}, std::string page_after={});
	
	
   /**
    * Requests the creation a new property.
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param name Name of the property.
    * @param street Street of the property address.
    * @param postalCode Postal code of the property address.
    * @param city City of the property address.
    * @param country Country of the property address.
    * 
    * @return cpr::Response object representing the raw request response.
    */
	cpr::Response requestCreateProperty(std::string API_tenant_UUID, std::string name, std::string street, std::string postalCode, std::string city, std::string country);
	
   /**
    * Requests the update an existing property.
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant of the property.
    * @param API_property_UUID UUID of the existing BAScloud property that is supposed to be updated.
    * @param name Optional new value for the name of the Device.
    * @param street Optional new value for the street of the property address.
    * @param postalCode Optional new value for the postal code of the property address.
    * @param city Optional new value for the city of the property address.
    * @param country Optional new value for the country of the property address.
    * 
    * @return cpr::Response object representing the raw request response.
    */
	cpr::Response requestUpdateProperty(std::string API_tenant_UUID, std::string API_property_UUID, std::string name={}, std::string street={}, std::string postalCode={}, std::string city={}, std::string country={});
	
   /**
    * Requests the deletion of an existing property.
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud tenant of the property.
    * @param API_property_UUID UUID of the existing BAScloud property that is supposed to be deleted.
	* 
    */
	cpr::Response requestDeleteProperty(std::string API_tenant_UUID, std::string API_property_UUID);

	// Connectors API endpoints

   /**
	 * Request a single connector entity by UUID.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_connector_UUID API UUID of the connector entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestConnector(std::string API_tenant_UUID, std::string API_connector_UUID);

   /**
	 * Request a connector collection with optional pagination.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param page_size Optional page size for the request e.g. maximal number of entries per page.
	 * @param page_before Optional page identifier pointing to the previous page.
	 * @param page_after Optional page identifier pointing to the next page.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestConnectorCollection(std::string API_tenant_UUID, int page_size=-1, std::string page_before={}, std::string page_after={});
	
   /**
	 * Request the property relationships of a given connector.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_connector_UUID API UUID of the connector entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestConnectorPropertyRelationship(std::string API_tenant_UUID, std::string API_connector_UUID);
	
	/**
	 * Request the associated property of a given connector.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_connector_UUID API UUID of the connector entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestConnectorAssociatedProperty(std::string API_tenant_UUID, std::string API_connector_UUID);
	
   /**
	 * Request the devices relationships of a given connector.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_connector_UUID API UUID of the connector entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestConnectorDevicesRelationship(std::string API_tenant_UUID, std::string API_connector_UUID);
	
	/**
	 * Request the associated devices of a given connector.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_connector_UUID API UUID of the connector entity.
	 * @param page_size Optional page size for the request e.g. maximal number of entries per page.
	 * @param page_before Optional page identifier pointing to the previous page.
	 * @param page_after Optional page identifier pointing to the next page.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestConnectorAssociatedDevices(std::string API_tenant_UUID, std::string API_connector_UUID, int page_size=-1, std::string page_before={}, std::string page_after={});
	
   /**
    * Requests the creation a new connector.
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param API_property_UUID UUID of the associated BAScloud property.
    * @param name Name of the connector.
    * 
    * @return cpr::Response object representing the raw request response.
    */
	cpr::Response requestCreateConnector(std::string API_tenant_UUID, std::string API_property_UUID, std::string name);
	
   /**
    * Requests the update of an existing connector.
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param API_connector_UUID UUID of the BAScloud connector.
    * @param name Optional update of the name of the connector.
    * 
    * @return cpr::Response object representing the raw request response.
    */
	cpr::Response requestUpdateConnector(std::string API_tenant_UUID, std::string API_connector_UUID, std::string name={});
	
   /**
    * Requests the deletion of an existing connector.
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud tenant of the property.
    * @param API_connector_UUID UUID of the existing BAScloud connector that is supposed to be deleted.
	* 
    * @return cpr::Response object representing the raw request response.
    */
	cpr::Response requestDeleteConnector(std::string API_tenant_UUID, std::string API_connector_UUID);
	
   /**
    * Requests a new API key for a connector entity.
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud tenant.
    * @param API_connector_UUID UUID of a BAScloud connector for which the API key is requested.
	* 
    * @return cpr::Response object representing the raw request response.
    */
	cpr::Response requestConnectorToken(std::string API_tenant_UUID, std::string API_connector_UUID);

	// Devices API endpoints

   /**
	 * Request a single device entity by UUID.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_device_UUID API UUID of the device entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDevice(std::string API_tenant_UUID, std::string API_device_UUID);
	
   /**
	 * Request a device collection with optional filters.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
     * @param aksID Optional filter for the AKS ID of the device.
     * @param description Optional filter for the Description of the device.
     * @param unit Optional filter for the measuring unit of the device.
	 * @param page_size Optional page size for the request e.g. maximal number of entries per page.
	 * @param page_before Optional page identifier pointing to the previous page.
	 * @param page_after Optional page identifier pointing to the next page.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeviceCollection(std::string API_tenant_UUID, std::string aksID={}, std::string description={}, std::string unit={}, int page_size=-1, std::string page_before={}, std::string page_after={});
	
   /**
	 * Request the connector relationship of a given device.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_device_UUID API UUID of the device entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeviceConnectorRelationship(std::string API_tenant_UUID, std::string API_device_UUID);
	
   /**
	 * Request the associated connector of a given device.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_device_UUID API UUID of the device entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeviceAssociatedConnector(std::string API_tenant_UUID, std::string API_device_UUID);
	
   /**
	 * Request the readings releationship of a given device.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_device_UUID API UUID of the device entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeviceReadingsRelationship(std::string API_tenant_UUID, std::string API_device_UUID);
	
   /**
	 * Request the associated readings of a given device.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_device_UUID API UUID of the device entity.
	 * @param page_size Optional page size for the request e.g. maximal number of entries per page.
	 * @param page_before Optional page identifier pointing to the previous page.
	 * @param page_after Optional page identifier pointing to the next page.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeviceAssociatedReadings(std::string API_tenant_UUID, std::string API_device_UUID, int page_size=-1, std::string page_before={}, std::string page_after={});
	
   /**
	 * Request the setpoints releationship of a given device.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_device_UUID API UUID of the device entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeviceSetPointsRelationship(std::string API_tenant_UUID, std::string API_device_UUID);
	
   /**
	 * Request the associated setpoints of a given device.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_device_UUID API UUID of the device entity.
	 * @param page_size Optional page size for the request e.g. maximal number of entries per page.
	 * @param page_before Optional page identifier pointing to the previous page.
	 * @param page_after Optional page identifier pointing to the next page.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeviceAssociatedSetPoints(std::string API_tenant_UUID, std::string API_device_UUID, int page_size=-1, std::string page_before={}, std::string page_after={}); 
	
   /**
	 * Request the creation of a new device entity given an associated connector.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_connector_UUID API UUID of the associated connector entity.
     * @param aksID The AKS ID of the new device.
     * @param description The Description of the new device.
     * @param unit The measuring unit of the new device.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestCreateDevice(std::string API_tenant_UUID, std::string API_connector_UUID, std::string aksID, std::string description, std::string unit);
	
   /**
	 * Request an update of an existing device.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_device_UUID API UUID of the existing device entity.
     * @param aksID Optional new value for the AKS ID of the device.
     * @param description Optional new value for the Description of the device.
     * @param unit Optional new value for the measuring unit of the device.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestUpdateDevice(std::string API_tenant_UUID, std::string API_device_UUID, std::string aksID={}, std::string description={}, std::string unit={});
	
	/**
	 * Request the deletion of an existing device.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_device_UUID API UUID of the existing device entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeleteDevice(std::string API_tenant_UUID, std::string API_device_UUID);

	// Readings API endpoints
	
   /**
	 * Request a single reading entity by UUID.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_reading_UUID API UUID of the reading entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestReading(std::string API_tenant_UUID, std::string API_reading_UUID);
	
   /**
	 * Request a reading collection with optional filters.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
     * @param from Optional filter defining the start time of a time-range the requested reading should lie in.
     * @param until Optional filter defining the end time of a time-range the requested reading should lie in.
     * @param timestamp Optional filter for the timestamp of the reading.
     * @param value Optional filter for the value of the reading.
     * @param API_device_UUID Optional filter for the associated device UUID of the reading.
	 * @param page_size Optional page size for the request e.g. maximal number of entries per page.
	 * @param page_before Optional page identifier pointing to the previous page.
	 * @param page_after Optional page identifier pointing to the next page.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestReadingCollection(std::string API_tenant_UUID, std::time_t from=-1, std::time_t until=-1, std::time_t timestamp=-1, double value=std::numeric_limits<double>::quiet_NaN(), std::string API_device_UUID={}, int page_size=-1, std::string page_before={}, std::string page_after={});
	
   /**
	 * Request the device releationship of a given reading.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_reading_UUID API UUID of the reading entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestReadingDeviceRelationship(std::string API_tenant_UUID, std::string API_reading_UUID);
	
   /**
	 * Request the associated device of a given reading.
	 * 
	 * @param API_tenant_UUID API UUID of the tenant entity.
	 * @param API_reading_UUID API UUID of the reading entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestReadingAssociatedDevice(std::string API_tenant_UUID, std::string API_reading_UUID);
	
   /**
	 * Request the creation of a new reading entity given an associated device.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_device_UUID API UUID of the associated device entity.
     * @param value Value of the reading.
     * @param timestamp The time of the reading of the entity value.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestCreateReading(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp);
	
	/**
	 * Request the deletion of an existing reading entity.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_reading_UUID API UUID of the reading entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestDeleteReading(std::string API_tenant_UUID, std::string API_reading_UUID);

	// SetPoints API endpoints
	
   /**
	 * Request a single setpoint entity by UUID.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_setpoint_UUID API UUID of the setpoint entity.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestSetPoint(std::string API_tenant_UUID, std::string API_setpoint_UUID);
	
   /**
	 * Request a setpoint collection with optional filters.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
     * @param from Optional filter defining the start time of a time-range the requested setpoint should lie in.
     * @param until Optional filter defining the end time of a time-range the requested setpoint should lie in.
     * @param timestamp Optional filter for the timestamp of the setpoint.
     * @param currentTime Optional filter for the currentTime of the setpoint.
     * @param API_device_UUID Optional filter for the associated device UUID of the setpoint.
	 * @param page_size Optional page size for the request e.g. maximal number of entries per page.
	 * @param page_before Optional page identifier pointing to the previous page.
	 * @param page_after Optional page identifier pointing to the next page.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestSetPointCollection(std::string API_tenant_UUID, std::time_t from=-1, std::time_t until=-1, std::time_t timestamp=-1, std::time_t currentTime=-1, std::string API_device_UUID={}, int page_size=-1, std::string page_before={}, std::string page_after={});
	
   /**
	 * Request the creation of a new setpoint entity given an associated device.
	 * 
	 * @param API_tenant_UUID API UUID of the associated tenant entity.
	 * @param API_device_UUID API UUID of the associated device entity.
     * @param value Value of the SetPoint
     * @param timestamp The time of the SetPoint value.
	 * 
	 * @return cpr::Response object representing the raw request response.
	 */
	cpr::Response requestCreateSetPoint(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp);


};


}




