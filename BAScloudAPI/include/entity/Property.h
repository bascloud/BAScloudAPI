
#pragma once

#include <string>
#include <vector>

#include "Entity.h"
#include "EntityTenantMixin.h"
#include "EntityDateMixin.h"
#include "Paging.h"
#include "EntityCollection.h"


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
    * @param street Street of the Property address.
    * @param postalCode Postal code of the Property address.
    * @param city City of the Property address.
    * @param country Country of the Property address.
    * @param createdAt Datetime describing the creation of the Property entity in the BAScloud.
    * @param updatedAt Datetime describing the last update of the Property information in the BAScloud.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
   Property(std::string API_UUID, std::string API_tenant_UUID, std::string name, std::string street, std::string postalCode, std::string city, std::string country, std::time_t createdAt, std::time_t updatedAt, EntityContext* context);

  /**
   * Get the Property name.
   * 
   * @return Textual name of the Property.
   */
   std::string getName();

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
    * @param name Optional filter for the name of the Device.
    * @param street Optional filter for the street of the Property address.
    * @param postalCode Optional filter for the postal code of the Property address.
    * @param city Optional filter for the city of the Property address.
    * @param country Optional filter for the country of the Property address.
    * 
    * @return EntityCollection containing list of Property entities matching the provided filters and paging information.
    */
   static EntityCollection<Property> getProperties(std::string API_tenant_UUID, EntityContext* context, PagingOption paging={}, std::string name={}, std::string street={}, std::string postalCode={}, std::string city={}, std::string country={});

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
	 static Property createProperty(std::string API_tenant_UUID, std::string name, std::string street, std::string postalCode, std::string city, std::string country, EntityContext* context);
      
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
	 static Property updateProperty(std::string API_tenant_UUID, std::string API_property_UUID, EntityContext* context, std::string name={}, std::string street={}, std::string postalCode={}, std::string city={}, std::string country={});

	 
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