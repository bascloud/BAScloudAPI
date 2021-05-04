#pragma once

#include <string>


namespace BAScloud {

/** 
 * Mixin class that adds functionality and attributes to associate a Tenant to a Entity object.
 * 
 * Most BAScloud entities have an associated Tenant object which is needed to request the BAScloud API accordingly.
 */
class EntityTenantMixin {

 private:

   /**
    * The UUID of the associated Tenant entity in the BAScloud.
    */
    std::string tenant_UUID;

 protected:

   /**
    * EntityTenantMixin constructor
    *
    * Protected as EntityTenantMixin class should not be instantiated directly.
    *
    * @param API_tenant_UUID The UUID of the associated Tenant entity in the BAScloud.
    */
    EntityTenantMixin(std::string API_tenant_UUID) : tenant_UUID(API_tenant_UUID) {
        
    }

 public:

   /**
    * Get the UUID of the associated Tenent entity.
    * 
    * @return The UUID of the associated Tenant entity in the BAScloud.
    */
    std::string getTenantUUID() {
        return tenant_UUID;
    }

};

}