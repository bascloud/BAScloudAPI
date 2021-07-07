
#pragma once

#include <string>
#include <vector>

#include "Entity.h"
#include "EntityTenantMixin.h"
#include "EntityDateMixin.h"
#include "Paging.h"
#include "EntityCollection.h"

#include "Util.h"


namespace BAScloud {

class Property;
class Device;

/** 
 * A Connector entity represents a BAScloud Connector in a property/building. 
 * 
 * Each Connector is responsible for a particular set of Devices and its related entities (Readings, Setpoints).
 * 
 */
class Connector : public Entity, public EntityTenantMixin, public EntityDateMixin {

 private:

   /**
    * Name of the Connector.
    */
   std::string name;

   /**
    * API token of the Connector for accessing the BAScloud API. A Connector's API token never expires.
    * 
    * The API key may not be available (empty), a new API token can be requested through refreshAuthToken().
    * This request invalidates the previous API token.
    */
   std::string token;

 public:

   /**
    * Connector constructor
    *
    * Creates a Connector object representing a BAScloud API entity.
    *
    * Note: Creating an entity object over its constructor does not automatically create the entity in the BAScloud. 
    * For creation of a BAScloud entity use the static method of the object class Connector::createConnector().
    * 
    * @param API_UUID Universally unique identifier of the represented BAScloud Connector.
    * @param API_tenant_UUID Universally unique identifier of the represented BAScloud Device.
    * @param name Name for the Connector in the building.
    * @param token API token of the Connector for accessing the BAScloud API.
    * @param createdAt Datetime describing the creation of the device entity in the BAScloud.
    * @param updatedAt Datetime describing the last update of the device information in the BAScloud.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
   Connector(std::string API_UUID, std::string API_tenant_UUID, std::string name, std::string token, std::time_t createdAt, std::time_t updatedAt, EntityContext* context);

   /**
    * Get the Connector name.
    * 
    * @return Name of the Connector.
    */
   std::string getName();

   /**
    * Get the Connector API token.
    * 
    * This field may be empty if no current API token is available. User refreshAuthToken() to update the API token attribute.
    * The refreshAuthToken() call invalidates all previous API tokens. 
    * 
    * @return API key of the Connector.
    */
   std::string getToken();

   /**
    * Set the Connector API token.
    */
   void setToken(std::string newToken);

   /**
    * Refresh the Connector API token from the BAScloud.
    * 
    * Requests a new API token for this Connector entity and updates its token attribute.
    * A call to this function invalidates the previous API token.
    * 
    */
   void refreshAuthToken();

   /**
    * [DEPRECATED] in API v2.1
    * 
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
    * @return Associated Property entity object
    */
   Property getAssociatedProperty();

   /**
    * Get the tenant permissions and role of the Connector.
    * 
    * Returns the role and list of resource actions the Connector is allowed to access.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @return A PermissionData object representing the permissions the Connector has and resources the Connector can access.
    */
  PermissionData getPermissions();

    /**
    * Get the associated Device entities of the Connector.
    * 
    * Each Connector can have multiple associated Devices.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param aksID Optional filter for the AKS ID of the Device.
    * @param description Optional filter for the Description of the Device.
    * @param unit Optional filter for the measuring unit of the Device.
    * @param createdFrom Optional filter for the creation date of the Device. All points from this timestamp.
    * @param createdUntil Optional filter for the creation date of the Device. All points until this timestamp.
    * 
    * @return EntityCollection containing list of Device entities associated with the Connector.
    */
   EntityCollection<Device> getAssociatedDevices(PagingOption paging={}, std::string aksID={}, std::string localAksID={}, std::string description={}, std::string unit={}, std::time_t createdFrom=-1, std::time_t createdUntil=-1, std::time_t deletedUntil=-1);

   /**
    * Request a single Connector entity.
    * 
    * A Connector is uniquely identified by the associated Tenant and Connector UUID.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return A Connector object representing the BAScloud Connector with the specified UUID.
    */
   static Connector getConnector(std::string API_tenant_UUID, std::string API_connector_UUID, EntityContext* context);

   /**
    * Request a collection of Connector entities grouped under the given Tenant.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the associated BAScloud Tenant.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * @param createdFrom Optional filter for the creation date of the Connector. All points from this timestamp.
    * @param createdUntil Optional filter for the creation date of the Connector. All points until this timestamp.
    * 
    * @return EntityCollection containing list of Connector entities matching the provided filters and paging information.
    */
   static EntityCollection<Connector> getConnectors(std::string API_tenant_UUID, EntityContext* context, PagingOption paging={}, std::time_t createdFrom=-1, std::time_t createdUntil=-1);

   /**
    * Create a new Connector entity in the BAScloud.
    * 
    * Given the associated Tenant and Property a new Connector is created using the given Connector parameter.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return Connector entity object representing the newly created BAScloud Connector.
    */
   static Connector createConnector(std::string API_tenant_UUID, std::string name, EntityContext* context);

   /**
    * Update an existing Connector in the BAScloud.
    * 
    * The request updates attributes of an existing BAScloud Connector based on the given Connector UUID and returns 
    * a new Connector object representing the updated entity.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * @param name Optional new name of the new Connector.
    * 
    * @return Connector entity object representing the updated BAScloud Connector.
    */
   static Connector updateConnector(std::string API_tenant_UUID, std::string API_connector_UUID, EntityContext* context, std::string name={});

   /**
    * Deletes an existing Connector in the BAScloud.
    * 
    * The request deletes a Connector entity in the BAScloud based on the given Connector UUID.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
   static void deleteConnector(std::string API_tenant_UUID, std::string API_connector_UUID, EntityContext* context);

};

}