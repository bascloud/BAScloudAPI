#pragma once

#include <string>


namespace BASCloud {

class EntityContext;

/** 
 * Base class modeling a BASCloud entity.
 * 
 * Each entity retrieved from the BASCloud API inherits from this class and has a universally unique identifier (uuid).
 */
class Entity {

 private:

   /**
    * Universally unique identifier of the represented BASCloud entity.
    */
    std::string uuid;

 protected:
 
    /**
    * Pointer to an EntityContext proving an abstracted context for accessing the API functions.
    */
    EntityContext *context;

   /**
    * Entity constructor
    *
    * Protected as Entity class should not be instantiated directly.
    *
    * @param API_UUID Universally unique identifier of the represented BASCloud entity.
    * @param context EntityContext proving an abstracted context for accessing the API functions.
    */
    Entity(std::string API_UUID, EntityContext *context) : uuid(API_UUID), context(context) {
        
    }

 public:

   /**
    * Get the entity's UUID
    * 
    * @return Universally unique identifier of the represented BASCloud entity.
    */
    std::string getUUID() {
        return uuid;
    }

};

}