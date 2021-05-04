#include "entity/User.h"
#include "entity/Tenant.h"
#include "EntityContext.h"


namespace BAScloud {

User::User(std::string API_UUID, std::string email, std::time_t createdAt, std::time_t updatedAt, EntityContext* context) : 
    Entity(API_UUID, context), EntityDateMixin(createdAt, updatedAt), email(email) {

}

// User::User(bool incomplete, std::string status/*={}*/, std::exception exception/*={}*/) : 
//     Entity("", nullptr), EntityDateMixin(-1, -1), EntityIncompleteMixin(true, status, exception), email("") {
    
// }

std::string User::getEmail() {
    return email;
}

void User::resetPassword() {
    context->requestUserPasswordReset(getUUID());
}

void User::updatePassword(std::string reset_token, std::string new_password) {
    context->updateUserPassword(getUUID(), reset_token, new_password);
}

Tenant User::getAssociatedTenant() {
    return context->getAssociatedTenant(getUUID());
}


User User::createUser(std::string email, std::string password, EntityContext* context) {
    return context->createNewUser(email, password);
}

User getUser(std::string API_UUID, EntityContext* context) {
    return context->getUser(API_UUID);
}

EntityCollection<User> getUsers(EntityContext* context, PagingOption paging/*={}*/, std::string email/*={}*/) {
    return context->getUsersCollection(email);
}

void deleteUser(std::string API_UUID, EntityContext* context) {
    context->deleteUser(API_UUID);
}

User updateUser(std::string API_UUID, EntityContext* context, std::string email/*={}*/) {
    return context->updateUser(API_UUID, email);
}


}
