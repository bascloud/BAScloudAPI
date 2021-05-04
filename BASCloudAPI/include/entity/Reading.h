
#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "Entity.h"
#include "EntityTenantMixin.h"
#include "EntityDateMixin.h"
#include "Paging.h"
#include "EntityCollection.h"


namespace BAScloud {

class Device;

/** 
 * A Reading entity represents a reading from a sensor or actor (Device) in a building (Property).
 * 
 * Each Reading has an associated Device, a value, and a timestamp.
 * 
 */
class Reading : public Entity, public EntityTenantMixin, public EntityDateMixin {

 private:
 
  /**
   * Time of the reading of the entity value.
   */
   std::time_t timestamp;

  /**
   * Value of the reading.
   */
   double value;

 public:

   /**
    * Reading constructor
    *
    * Creates a Reading object representing a BAScloud API entity.
    *
    * Note: Creating an entity object over its constructor does not automatically create the entity in the BAScloud. 
    * For creation of a BAScloud entity use the static method of the corresponding object class Reading::createReading().
    * 
    * @param API_UUID Universally unique identifier of the represented BAScloud Reading.
    * @param API_tenant_UUID Universally unique identifier of the represented BAScloud Reading.
    * @param value Value of the Reading.
    * @param timestamp Timestamp of the Reading.
    * @param createdAt Datetime describing the creation of the Reading entity in the BAScloud.
    * @param updatedAt Datetime describing the last update of the Reading information in the BAScloud.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
   Reading(std::string API_UUID, std::string API_tenant_UUID, double value, std::time_t timestamp, std::time_t createdAt, std::time_t updatedAt, EntityContext* context);

  /**
   * Get the Reading value.
   * 
   * @return Double precision floating point value of the reading.
   */
   double getValue();

  /**
   * Get the time of the reading of the entity value.
   * 
   * @return UNIX timestamp value.
   */
   std::time_t getTimestamp();

   /**
    * Get the associated Device entity of the Reading.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @return Device entity object of the associated BAScloud Device.
    */
	 Device getAssociatedDevice();
   
   /**
    * Request a single Reading entity.
    * 
    * A Reading is uniquely identified by the associated Tenant and Reading UUID.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return A Reading object representing the BAScloud Reading with the specified UUID.
    */
   static Reading getReading(std::string API_tenant_UUID, std::string API_reading_UUID, EntityContext* context);

   /**
    * Request a collection of Reading entities grouped under the given Tenant.
    * 
    * The request filters the BAScloud Reading based on the given parameters and returns a collection 
    * of Reading matching these values.
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
    * @param from Optional filter defining the start time of a time-range the requested Readings should lie in.
    * @param until Optional filter defining the end time of a time-range the requested Readings should lie in.
    * @param timestamp Optional filter for the timestamp of the Reading.
    * @param value Optional filter for the value of the Reading.
    * @param API_device_UUID Optional filter for the associated Device UUID of the Reading.
    * 
    * @return EntityCollection containing list of Property entities matching the provided filters and paging information.
    */
   static EntityCollection<Reading> getReadings(std::string API_tenant_UUID, EntityContext* context, PagingOption paging={}, std::time_t from=-1, std::time_t until=-1, std::time_t timestamp=-1, double value=std::numeric_limits<double>::quiet_NaN(), std::string API_device_UUID={});

   /**
    * Create a new Reading entity in the BAScloud.
    * 
    * Given the associated Tenant and Device entity, a new Reading is created using the given Reading parameter.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return Reading entity object representing the newly created BAScloud Reading.
    */
	 static Reading createReading(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp, EntityContext* context);

   /**
    * Deletes an existing Reading in the BAScloud. [Admin] 
    * 
    * The request deletes a Reading entity in the BAScloud based on the given Reading UUID.
    * This operation needs administration authority. 
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
	 static void deleteReading(std::string API_tenant_UUID, std::string API_reading_UUID, EntityContext* context);

};

}