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

EntityCollection<User> Tenant::getAssociatedUsers(PagingOption paging/*={}*/, std::string email/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/) {
    return context->getAssociatedUsers(getUUID(), paging, email, createdFrom, createdUntil);
}

void Tenant::assignUser(User user, std::string role) {
    assignUsers(std::vector<std::pair<User, std::string>> {std::make_pair(user, role)});
}

void Tenant::assignUsers(std::vector<std::pair<User, std::string>> users) {
    std::vector<std::string> userUUIDs;
    std::vector<std::string> userRoles;
    for(std::pair<User, std::string> pu: users) {
        userUUIDs.push_back(pu.first.getUUID());
        userUUIDs.push_back(pu.second);
    }
    context->assignTenantUsers(getUUID(), userUUIDs, userRoles);
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

EntityCollection<Tenant> getTenants(EntityContext* context, PagingOption paging/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/) {
    return context->getTenantsCollection(paging, createdFrom, createdUntil);
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
