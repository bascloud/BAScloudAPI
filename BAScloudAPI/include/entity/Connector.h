
#pragma once

#include <string>
#include <vector>

#include "Entity.h"
#include "EntityTenantMixin.h"
#include "EntityDateMixin.h"
#include "Paging.h"
#include "EntityCollection.h"


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
    * API key of the Connector for accessing the BAScloud API. A Connector's API key never expires.
    * 
    * The API key may not be available (empty), a new API key can be requested through refreshAuthToken().
    * This request invalidates the previous API key.
    */
   std::string api_key;

 public:

   /**
    * Connector constructor
    *
    * Creates a Connector object representing a BAScloud API entity.
    *
    * Note: Creating an entity object over its constructor does not automatically create the entity in the BAScloud. 
    * For creation of a BAScloud entity use the static method of the corresponding object class Connector::createConnector().
    * 
    * @param API_UUID Universally unique identifier of the represented BAScloud Connector.
    * @param API_tenant_UUID Universally unique identifier of the represented BAScloud Device.
    * @param name Name for the Connector in the building.
    * @param apiKey API key of the Connector for accessing the BAScloud API.
    * @param createdAt Datetime describing the creation of the device entity in the BAScloud.
    * @param updatedAt Datetime describing the last update of the device information in the BAScloud.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
   Connector(std::string API_UUID, std::string API_tenant_UUID, std::string name, std::string apiKey, std::time_t createdAt, std::time_t updatedAt, EntityContext* context);

   /**
    * Get the Connector name.
    * 
    * @return Name of the Connector.
    */
   std::string getName();

   /**
    * Get the Connector API key.
    * 
    * This field may be empty if no current API key is available. User refreshAuthToken() to update the API key attribute.
    * The refreshAuthToken() call invalidates all previous API keys. 
    * 
    * @return API key of the Connector.
    */
   std::string getAPIKey();

   /**
    * Set the Connector API key.
    */
   void setAPIKey(std::string newApiKey);

   /**
    * Refresh the Connector API key from the BAScloud.
    * 
    * Requests a new API key for this Connector entity and updates its api_key attribute.
    * A call to this function invalidates the previous API key.
    * 
    */
   void refreshAuthToken();

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
    * @return Associated Property entity object
    */
   Property getAssociatedProperty();

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
    * 
    * @return EntityCollection containing list of Device entities associated with the Connector.
    */
   EntityCollection<Device> getAssociatedDevices(PagingOption paging={});

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
    * 
    * @return EntityCollection containing list of Connector entities matching the provided filters and paging information.
    */
   static EntityCollection<Connector> getConnectors(std::string API_tenant_UUID, EntityContext* context, PagingOption paging={});

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
   static Connector createConnector(std::string API_tenant_UUID, std::string API_property_UUID, std::string name, EntityContext* context);

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