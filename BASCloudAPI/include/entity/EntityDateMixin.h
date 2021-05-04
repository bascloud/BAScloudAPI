#pragma once

#include <chrono>


namespace BAScloud {

/** 
 * Mixin class that adds datetime functionality and attributes.
 * 
 * Each entity retrieved from the BAScloud API includes datetime information about the creation and last update.
 */
class EntityDateMixin {

 private:

   /**
    * Time of the creation of the entity represented as a UNIX timestamp.
    */
    std::time_t created_at;

   /**
    * Time of the last update of the entity represented as a UNIX timestamp.
    */
    std::time_t updated_at; 

 protected:

   /**
    * EntityDateMixin constructor
    *
    * Protected as EntityDateMixin class should not be instantiated directly.
    *
    * @param createdAt Time of the creation of the entity represented as a UNIX timestamp.
    * @param updatedAt Time of the last update of the entity represented as a UNIX timestamp.
    */
    EntityDateMixin(time_t createdAt, time_t updatedAt) : created_at(createdAt), updated_at(updatedAt) {
        
    }

 public:

   /**
    * Get the time of creation of the entity.
    * 
    * @return Time of creation as a UNIX timestamp.
    */
    std::time_t getCreatedDate() {
        return created_at;
    }

   /**
    * Get the time of the last update of the entity.
    * 
    * @return Time of the last update as a UNIX timestamp.
    */
    std::time_t getLastUpdatedDate() {
        return updated_at;
    }

};

}