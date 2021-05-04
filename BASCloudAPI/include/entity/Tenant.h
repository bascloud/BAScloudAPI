#pragma once

#include <string>
#include <vector>

#include "Entity.h"
#include "EntityDateMixin.h"
#include "Paging.h"
#include "EntityCollection.h"


namespace BAScloud {

class User;

/** 
 * A Tenant entity represents a customer/tenant in the BAScloud.
 * 
 * A Tenant can have one or multiple associated User entities.
 * 
 */
class Tenant : public Entity, public EntityDateMixin {

 private:

  /**
   * Name of the Tenant.
   */
   std::string name;

  /**
   * The URL name of the Tenant.
   */
   std::string url_name;

 public:

   /**
    * Tenant constructor
    *
    * Creates a Tenant object representing a BAScloud API entity.
    * 
    * Note: Creating an entity object over its constructor does not automatically create the entity in the BAScloud. 
    * For creation of a BAScloud entity use the static method of the corresponding object class Tenant::createTenant().
    *
    * @param API_UUID Universally unique identifier of the represented BAScloud Tenant.
    * @param name Name of the Tenant.
    * @param urlName URL Name of the Tenant.
    * @param createdAt Datetime describing the creation of the Tenant entity in the BAScloud.
    * @param updatedAt Datetime describing the last update of the Tenant information in the BAScloud.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
   Tenant(std::string API_UUID, std::string name, std::string urlName, std::time_t createdAt, std::time_t updatedAt, EntityContext* context);

  /**
   * Get the Tenant name.
   * 
   * @return Textual name of the Tenant.
   */
   std::string getName();

  /**
   * Get the Tenant URL name.
   * 
   * @return Textual URL name of the Tenant.
   */
   std::string getUrlName();

   /**
    * Request a single Tenant entity.
    * 
    * A Tenant is uniquely identified by a BAScloud Tenant UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID UUID of the BAScloud Tenant.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return A Tenant object representing the BAScloud Tenant with the specified UUID.
    */
   static Tenant getTenant(std::string API_tenant_UUID, EntityContext* context);

  /**
    * Request a collection of Tenant entities.
    * 
    * Returns all Tenants the currently authenticated user is authorized to view. This is probably 
    * only its own tenant.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * @param paging Optional PagingOption that is used for requesting paged API results.
    * 
    * @return EntityCollection containing a list of Tenant entities.
    */
   static EntityCollection<Tenant> getTenants(EntityContext* context, PagingOption paging={});

   /**
    * Get a collection of associated User entities of the Tenant.
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
    * @return EntityCollection containing list of User entities and paging information.
    */
   EntityCollection<User> getAssociatedUsers(PagingOption paging={});

  /**
    * Assigns a User entity to the Tenant. [Admin] 
    * 
    * This operation is only permitted to Users with administrator rights for the Tenant.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param user User entity object that is to be assigned to the Tenant.
    */
   void assignUser(User user);

  /**
    * Assigns a collection of User entities to the Tenant. [Admin] 
    * 
    * This operation is only permitted to Users with administrator rights for the Tenant.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param users Collection of User entity objects that are to be assigned to the Tenant.
    */
   void assignUsers(std::vector<User> users);

  /**
    * Removes a User entity from the Tenant. [Admin] 
    * 
    * This operation is only permitted to Users with administrator rights for the Tenant.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param user User entity object that is to be removed from the Tenant.
    */
   void removeUser(User user); // TODO ML: what happens when user is removed? tenant set to null? null causes bug in tenant visibility on backend

  /**
    * Removes a collection of User entities from the Tenant. [Admin] 
    * 
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
    * @param users Collection of User entity objects that are to be removed from the Tenant.
    */
   void removeUsers(std::vector<User> users);

   /**
    * Create a new Tenant entity in the BAScloud. [Admin] 
    * 
    * A new Tenant is created using the given Tenant parameter. This operation needs 
    * administration authority. 
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return Tenant entity object representing the newly created BAScloud Tenant.
    */
   static Tenant createTenant(std::string name, std::string API_user_UUID, EntityContext* context);

  /**
    * Deletes an existing Tenant in the BAScloud. [Admin] 
    * 
    * The request deletes a Tenant entity in the BAScloud based on the given Tenant UUID. 
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
    * @param API_tenant_UUID UUID of the BAScloud Tenant.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
   static void deleteTenant(std::string API_tenant_UUID, EntityContext* context);

  /**
    * Updates the information of a Tenant entity in the BAScloud. [Admin] 
    * 
    * All parameters are optional. This operation needs administration authority. 
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_tenant_UUID Tenant entity UUID that is supposed to be updated.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * @param name Optional new value for the Tenant name.
    * 
    * @return Tenant entity object representing the updated BAScloud Tenant.
    */
	 static Tenant updateTenant(std::string API_tenant_UUID, EntityContext* context, std::string name={});

};

}
