#include "entity/Tenant.h"
#include "entity/User.h"
#include "EntityContext.h"


namespace BAScloud {

Tenant::Tenant(std::string API_UUID, std::string name, std::string urlName, std::time_t createdAt, std::time_t updatedAt, EntityContext* context) : 
    Entity(API_UUID, context), EntityDateMixin(createdAt, updatedAt), name(name), url_name(urlName) {

}

std::string Tenant::getName() {
    return name;
}

std::string Tenant::getUrlName() {
    return url_name;
}

EntityCollection<User> Tenant::getAssociatedUsers(PagingOption paging/*={}*/) {
    return context->getAssociatedUsers(getUUID(), paging);
}

void Tenant::assignUser(User user) {
    assignUsers(std::vector<User>{user});
}

void Tenant::assignUsers(std::vector<User> users) {
    std::vector<std::string> userUUIDs;
    for(User u: users) {
        userUUIDs.push_back(u.getUUID());
    }
    context->assignTenantUsers(getUUID(), userUUIDs);
}

void Tenant::removeUser(User user) {
    removeUsers(std::vector<User>{user});
}

void Tenant::removeUsers(std::vector<User> users) {
    std::vector<std::string> userUUIDs;
    for(User u: users) {
        userUUIDs.push_back(u.getUUID());
    }
    context->removeTenantUsers(getUUID(), userUUIDs);
}

Tenant getTenant(std::string API_tenant_UUID, EntityContext* context) {
    return context->getTenant(API_tenant_UUID);
}

EntityCollection<Tenant> getTenants(EntityContext* context, PagingOption paging/*={}*/) {
    return context->getTenantsCollection();
}

Tenant createTenant(std::string name, std::string API_user_UUID, EntityContext* context) {
    return context->createTenant(name, API_user_UUID);
}

void deleteTenant(std::string API_tenant_UUID, EntityContext* context) {
    context->deleteTenant(API_tenant_UUID);
}

Tenant updateTenant(std::string API_tenant_UUID, EntityContext* context, std::string name/*={}*/) {
    return context->updateTenant(API_tenant_UUID, name);
}


}
