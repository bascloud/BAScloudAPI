
#pragma once

#include <string>
#include <vector>

#include "Entity.h"
#include "EntityTenantMixin.h"
#include "EntityDateMixin.h"
#include "Paging.h"
#include "EntityCollection.h"
#include "Device.h"

namespace BAScloud {

class Connector;

/** 
 * A Property entity represents a building or location in the BAScloud.
 * 
 * Each Property has an associated Tenant parent and relations to multiple associated Connectors.
 * 
 */
class Property : public Entity, public EntityTenantMixin, public EntityDateMixin {

 private:

  /**
   * Name of the Property.
   */
   std::string name; 

   /**
    * Property part of the Anlagenkennzeichnungsschluessel (AKS) physical identifier for the building.
    */
   std::string aks_ID;

   /**
    * Administration identifier of the property (i.e. Liegenschaftsnummer)
    */
   std::string identifier;

  /**
   * Street of the Property address.
   */
   std::string street;

  /**
   * Postal code of the Property address.
   */
   std::string postal_code;

  /**
   * City of the Property address.
   */
   std::string city;
  
  /**
   * Country of the Property address.
   */
   std::string country;

 public:

   /**
    * Property constructor
    *
    * Creates a Property object representing a BAScloud API entity.
    *
    * Note: Creating an entity object over its constructor does not automatically create the entity in the BAScloud. 
    * For creation of a BAScloud entity use the static method of the corresponding object class Property::createProperty().
    * 
    * @param API_UUID Universally unique identifier of the represented BAScloud Property.
    * @param API_tenant_UUID Universally unique identifier of the represented BAScloud Property.
    * @param name Name of the Property.
    * @param aksID AKS ID part for the property.
    * @param identifier Administrative identifer of the Property.
    * @param street Street of the Property address.
    * @param postalCode Postal code of the Property address.
    * @param city City of the Property address.
    * @param country Country of the Property address.
    * @param createdAt Datetime describing the creation of the Property entity in the BAScloud.
    * @param updatedAt Datetime describing the last update of the Property information in the BAScloud.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
   Property(std::string API_UUID, std::string API_tenant_UUID, std::string name, std::string aksID, std::string identifier, std::string street, std::string postalCode, std::string city, std::string country, std::time_t createdAt, std::time_t updatedAt, EntityContext* context);

  /**
   * Get the Property name.
   * 
   * @return Textual name of the Property.
   */
   std::string getName();

  /**
   * Get the Property AKS ID.
   * 
   * @return AKS ID part of the Property.
   */
   std::string getAksID();

  /**
   * Get the Property identifier.
   * 
   * @return Administrative identifier of the Property.
   */
   std::string getIdentifier();

  /**
   * Get the Property street.
   * 
   * @return Street address of the Property.
   */
   std::string getStreet();

  /**
   * Get the Property postal code.
   * 
   * @return Postal code of the Property.
   */
   std::string getPostalCode();

  /**
   * Get the Property city.
   * 
   * @return City name of the Property.
   */
   std::string getCity();
   
  /**
   * Get the Property country.
   * 
   * @return Country of the Property.
   */
   std::string getCountry();

   /**
    * [DEPRECATED] in API v2.1
    * 
    * Get a collection of associated Connector entities of the Property.
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
    * @return EntityCollection containing list of Connector entities and paging information.
    */
	 EntityCollection<Connector> getAssociatedConnectors(PagingOption paging={});

   /**
    * Get a collection of associated Device entities of the Property.
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
    * @param localAksID Optional filter for the local AKS ID of the Device.
    * @param API_connector_UUID UUID of the associated Connector.
    * @param API_property_UUID UUID of the associated Property.
    * @param description Optional filter for the Description of the Device.
    * @param unit Optional filter for the measuring unit of the Device.
    * @param createdFrom Optional filter for the creation date of the Device. All points from this timestamp.
    * @param createdUntil Optional filter for the creation date of the Device. All points until this timestamp.
    * @param deletedUntil Optional filter for the soft-deletion date of the Device. All points until this timestamp.
    * 
    * @return EntityCollection containing list of Device entities and paging information.
    */
	 EntityCollection<Device> getAssociatedDevices(PagingOption paging={}, std::string aksID={}, std::string localAksID={}, std::string API_connector_UUID={}, std::string API_property_UUID={}, std::string description={}, std::string unit={}, std::time_t createdFrom=-1, std::time_t createdUntil=-1, std::time_t deletedUntil=-1);

   /**
    * Request a single Property entity.
    * 
    * A Property is uniquely identified by the associated Tenant and Property UUID.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return A Property object representing the BAScloud Property with the specified UUID.
    */
   static Property getProperty(std::string API_tenant_UUID, std::string API_property_UUID, EntityContext* context);

   /**
    * Request a collection of Property entities grouped under the given Tenant.
    * 
    * The request filters the BAScloud Property based on the given parameters and returns a collection 
    * of Property matching these values.
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
    * @param name Optional filter for the name of the Property.
    * @param aksID Optional AKS ID part for the property.
    * @param identifier Optional Administrative identifier of the Property.
    * @param street Optional filter for the street of the Property address.
    * @param postalCode Optional filter for the postal code of the Property address.
    * @param city Optional filter for the city of the Property address.
    * @param country Optional filter for the country of the Property address.
    * @param createdFrom Optional filter for the creation date of the Property. All points from this timestamp.
    * @param createdUntil Optional filter for the creation date of the Property. All points until this timestamp.
    * 
    * @return EntityCollection containing list of Property entities matching the provided filters and paging information.
    */
   static EntityCollection<Property> getProperties(std::string API_tenant_UUID, EntityContext* context, PagingOption paging={}, std::string name={}, std::string aksID={}, std::string identifier={}, std::string street={}, std::string postalCode={}, std::string city={}, std::string country={}, std::time_t createdFrom=-1, std::time_t createdUntil=-1);

   /**
    * Create a new Property entity in the BAScloud.
    * 
    * Given the associated Tenant a new Property is created using the given Property parameter.
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
    * @param name Name of the Property.
    * @param street Street of the Property address.
    * @param postalCode Postal code of the Property address.
    * @param city City of the Property address.
    * @param country Country of the Property address.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return Property entity object representing the newly created BAScloud Property.
    */
	 static Property createProperty(std::string API_tenant_UUID, std::string name, EntityContext* context, std::string aksID={}, std::string identifier={}, std::string street={}, std::string postalCode={}, std::string city={}, std::string country={});
      
   /**
    * Update an existing Property in the BAScloud.
    * 
    * The request updates attributes of an existing BAScloud Property based on the given Property UUID and returns 
    * a new Property object representing the updated entity.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * @param name Optional new value for the name of the Device.
    * @param street Optional new value for the street of the Property address.
    * @param postalCode Optional new value for the postal code of the Property address.
    * @param city Optional new value for the city of the Property address.
    * @param country Optional new value for the country of the Property address.
    * 
    * @return Property entity object representing the updated BAScloud Property.
    */
	 static Property updateProperty(std::string API_tenant_UUID, std::string API_property_UUID, EntityContext* context, std::string name={}, std::string aksID={}, std::string identifier={}, std::string street={}, std::string postalCode={}, std::string city={}, std::string country={});

   /**
    * Deletes an existing Property in the BAScloud.
    * 
    * The request deletes a Property entity in the BAScloud based on the given Property UUID.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
   static void deleteProperty(std::string API_tenant_UUID, std::string API_property_UUID, EntityContext* context);

};

}