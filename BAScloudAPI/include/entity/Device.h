
#pragma once

#include <string>
#include <vector>

#include "Entity.h"
#include "EntityTenantMixin.h"
#include "EntityDateMixin.h"
#include "Paging.h"
#include "Reading.h"
#include "SetPoint.h"
#include "EntityCollection.h"


namespace BAScloud {

class Connector;

/** 
 * A device entity represents a sensor or actor in a property/building.
 * 
 * Each Device has an associated Tenant parent and a mandatory relation to one and only one connector.
 * 
 */
class Device : public Entity, public EntityTenantMixin, public EntityDateMixin {

 private:

   /**
    * Anlagenkennzeichnungsschluessel (AKS) physical identifier for the device in the building.
    */
   std::string aks_ID;

   /**
    * Textual description of the device.
    */
   std::string description;
   /**
    * The measuring unit for the stored readings that are linked to the device.
    */
   std::string unit;

 public:

   /**
    * Device constructor
    *
    * Creates a Device object representing a BAScloud API entity.
    *
    * Note: Creating an entity object over its constructor does not automatically create the entity in the BAScloud. 
    * For creation of a BAScloud entity use the static method of the corresponding object class Device::createDevice().
    * 
    * @param API_UUID Universally unique identifier of the represented BAScloud Device.
    * @param API_tenant_UUID Universally unique identifier of the represented BAScloud Device.
    * @param aksID Anlagenkennzeichnungsschluessel (AKS) physical identifier for the Device in the building.
    * @param description Textual description of the Device.
    * @param unit The measuring unit for the stored readings that are linked to the Device.
    * @param createdAt Datetime describing the creation of the Device entity in the BAScloud.
    * @param updatedAt Datetime describing the last update of the Device information in the BAScloud.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
   Device(std::string API_UUID, std::string API_tenant_UUID, std::string aksID, std::string description, std::string unit, std::time_t createdAt, std::time_t updatedAt, EntityContext* context);

   /**
    * Get the Device AKS identifier.
    * 
    * @return Anlagenkennzeichnungsschluessel (AKS) physical identifier for the Device in the building.
    */
   std::string getAksID();

   /**
    * Get the Device description.
    * 
    * @return Textual description of the Device.
    */
   std::string getDescription();

   /**
    * Get the Device unit.
    * 
    * @return The measuring unit for the stored readings that are linked to the Device.
    */
   std::string getUnit();

   /**
    * Get the associated Connector entity of the Device.
    * 
    * Each Device has a mandatory relation to one and only one Connector.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @return Connector entity associated with this Device.
    */
	 Connector getAssociatedConnector();

   /**
    * Get a collection of associated Reading entities of the Device.
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
    * @return EntityCollection containing list of Reading entities and paging information.
    */
	 EntityCollection<Reading> getAssociatedReadings(PagingOption paging={});

  /**
    * Get a collection of associated Reading entities of the Device.
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
    * @return EntityCollection containing list of SetPoint entities and paging information.
    */
	 EntityCollection<SetPoint> getAssociatedSetPoints(PagingOption paging={});

   /**
    * Request a single Device entity.
    * 
    * A Device is uniquely identified by the associated Tenant and Device UUID.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return A Device object representing the BAScloud Device with the specified UUID.
    */
   static Device getDevice(std::string API_tenant_UUID, std::string API_device_UUID, EntityContext* context);

   /**
    * Request a collection of Device entities grouped under the given Tenant.
    * 
    * The request filters the BAScloud Devices based on the given parameters and returns a collection 
    * of Devices matching these values.
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
    * @param aksID Optional filter for the AKS ID of the Device.
    * @param description Optional filter for the Description of the Device.
    * @param unit Optional filter for the measuring unit of the Device.
    * 
    * @return EntityCollection containing list of Device entities matching the provided filters and paging information.
    */
   static EntityCollection<Device> getDevices(std::string API_tenant_UUID, EntityContext* context, PagingOption paging={}, std::string aksID={}, std::string description={}, std::string unit={});

   /**
    * Create a new Device entity in the BAScloud.
    * 
    * Given the associated Tenant and Connector a new Device is created using the given Device parameter.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return Device entity object representing the newly created BAScloud Device.
    */
	 static Device createDevice(std::string API_tenant_UUID, std::string API_connector_UUID, std::string aksID, std::string description, std::string unit, EntityContext* context);
   
   /**
    * Update an existing Device in the BAScloud.
    * 
    * The request updates attributes of an existing BAScloud Device based on the given Device UUID and returns 
    * a new Device object representing the updated entity.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * @param aksID Optional new value for the AKS ID of the Device.
    * @param description Optional new value for the Description of the Device.
    * @param unit Optional new value for the measuring unit of the Device.
    * 
    * @return Device entity object representing the updated BAScloud Device.
    */
	 static Device updateDevice(std::string API_tenant_UUID, std::string API_device_UUID, EntityContext* context, std::string aksID={}, std::string description={}, std::string unit={});

   /**
    * Deletes an existing Device in the BAScloud.
    * 
    * The request deletes a Device entity in the BAScloud based on the given Device UUID.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    */
	 static void deleteDevice(std::string API_tenant_UUID, std::string API_device_UUID, EntityContext* context);

};

}