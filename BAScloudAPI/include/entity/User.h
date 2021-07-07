#pragma once

#include <string>
#include <vector>

#include "Entity.h"
#include "EntityDateMixin.h"
#include "EntityCollection.h"

#include "Util.h"


namespace BAScloud {

class Tenant;

/** 
 * A User entity represents a API user of the BAScloud.
 * 
 * A User can authenticate itself against the BAScloud API using an email and password combination.
 * 
 * New User can be created using the static User method User::createUser().
 * 
 */
class User : public Entity, public EntityDateMixin {

 private:

  /**
   * Email of the User. Functions as the username for authentication.
   */
  std::string email;

 public:

/**
  * User constructor
  *
  * Creates a User object representing a BAScloud API entity.
  *
  * Note: Creating an entity object over its constructor does not automatically create the entity in the BAScloud. 
  * For creation of a BAScloud entity use the static method of the corresponding object class User::createUser().
  *
  * @param API_UUID Universally unique identifier of the represented BAScloud User.
  * @param email Email address of the User.
  * @param createdAt Datetime describing the creation of the User entity in the BAScloud.
  * @param updatedAt Datetime describing the last update of the User information in the BAScloud.
  * @param context EntityContext proving an abstracted context for accessing the API functions.
  */
  User(std::string API_UUID, std::string email, std::time_t createdAt, std::time_t updatedAt, EntityContext* context);


  /**
   * Get the User email.
   * 
   * @return Email of the Tenant.
   */
  std::string getEmail();

   /**
    * Request a single User entity.
    * 
    * A User is uniquely identified by a BAScloud Tenant UUID.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param API_UUID UUID of the BAScloud User.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return A User object representing the BAScloud User with the specified UUID.
    */
  static User getUser(std::string API_UUID, EntityContext* context);

  /**
    * Request a collection of User entities.
    * 
    * Returns all User the currently authenticated user is authorized to view. This may be only itself.
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
    * @param email Optional value filter for the user email.
    * @param createdFrom Optional filter for the creation date of the User. All points from this timestamp.
    * @param createdUntil Optional filter for the creation date of the User. All points until this timestamp.
    * 
    * @return EntityCollection containing a list of User entities.
    */
  static EntityCollection<User> getUsers(EntityContext* context, PagingOption paging={}, std::string email={}, std::time_t createdFrom=-1, std::time_t createdUntil=-1);

   /**
    * Get the associated Tenant entity of the User.
    * 
    * You may be only authorized to view the associated Tenant of the currently authenticated User.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @return A Tenant object representing the BAScloud Tenant associated with the User.
    */
  Tenant getAssociatedTenant();

   /**
    * Get the tenant permissions and role of the User.
    * 
    * Returns the role and list of resource actions the user is allowed to access.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @return A PermissionData object representing the permissions the user has and resources the user can access.
    */
  PermissionData getPermissions();

   /**
    * Request a password reset for the User.
    * 
    * If the User exists in the BAScloud, an email is sent upon the request containing a reset token.
    * The received token can then be used in User::updatePassword() to change the User password to a new value.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    */
  void resetPassword();

    /**
    * Changes the password of a User.
    * 
    * Prior to calling this function, a reset token should be requested using User::resetPassword().
    * Given the valid token, a new password can be set for the User.
    * 
    * @throws ServerError
    * @throws ConnectionError
    * @throws BadRequest
    * @throws UnauthorizedRequest
    * @throws NotFoundRequest
    * @throws ConflictRequest
    * @throws InvalidResponse
    * 
    * @param reset_token Valid reset token received from the BAScloud upon User::resetPassword() request.
    * @param new_password New password value for the User.
    */
  void updatePassword(std::string reset_token, std::string new_password);
  
  /**
    * Create a new User entity in the BAScloud.
    * 
    * A new User is created using the given email and password combination.
    * 
    * After creation, the new User is not assigned to any Tenant. User Tenant::assignUser() to assign the User.
    * 
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
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * 
    * @return User entity object representing the newly created BAScloud User.
    */
  static User createUser(std::string email, std::string password, EntityContext* context);

  /**
    * Deletes an existing User in the BAScloud. [Admin] 
    * 
    * The request deletes a User entity in the BAScloud based on the given User UUID. 
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
    * @param API_UUID UUID of the BAScloud User.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
  static void deleteUser(std::string API_UUID, EntityContext* context);

  /**
    * Updates the information of a User entity in the BAScloud. [Admin] 
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
    * @param API_UUID User entity UUID that is supposed to be updated.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    * @param email Optional new value for the User email.
    * @param API_tenant_UUID Tenant UUID of the tenant the user should be associated to (updates relationship)
    * @param role If Tenant relationship is updated, a role needs to be specified
    * 
    * @return User entity object representing the updated BAScloud User.
    */
  static User updateUser(std::string API_UUID, EntityContext* context, std::string email={}, std::string API_tenant_UUID={}, std::string role={});

};

}