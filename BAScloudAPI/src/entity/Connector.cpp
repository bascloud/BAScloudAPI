#include "entity/Connector.h"
#include "EntityContext.h"

#include "Util.h"

namespace BAScloud {

Connector::Connector(std::string API_UUID, std::string API_tenant_UUID, std::string name, std::string token, std::time_t createdAt, std::time_t updatedAt, EntityContext* context) : 
    Entity(API_UUID, context), EntityDateMixin(createdAt, updatedAt), EntityTenantMixin(API_tenant_UUID), name(name), token(token) {

}

std::string Connector::getName() {
    return  name;
}

void Connector::setToken(std::string newToken) {
    token = newToken;
}

std::string Connector::getToken() {
    return token;
}

Connector Connector::getConnector(std::string API_tenant_UUID, std::string API_connector_UUID, EntityContext* context) {
    return context->getConnector(API_tenant_UUID, API_connector_UUID);
}

EntityCollection<Connector> Connector::getConnectors(std::string API_tenant_UUID, EntityContext* context, PagingOption paging/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/) {
    return context->getConnectorsCollection(API_tenant_UUID, paging, createdFrom, createdUntil);  
}

void Connector::refreshAuthToken() {
    token = context->getNewConnectorAuthToken(getTenantUUID(), getUUID());
}

Property Connector::getAssociatedProperty() {
    return context->getAssociatedProperty(getTenantUUID(), getUUID());
}

EntityCollection<Device> Connector::getAssociatedDevices(PagingOption paging/*={}*/, std::string aksID/*={}*/, std::string localAksID/*={}*/, std::string description/*={}*/, std::string unit/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/, std::time_t deletedUntil/*=-1*/) {
    return context->getAssociatedConnectorDevices(getTenantUUID(), getUUID(), paging, aksID, localAksID, description, unit, createdFrom, createdUntil, deletedUntil);
}

PermissionData Connector::getPermissions() {
    return context->getConnectorPermissions(getTenantUUID(), getUUID());
}

Connector createConnector(std::string API_tenant_UUID, std::string name, EntityContext* context) {
    return context->createConnector(API_tenant_UUID, name);
}

Connector updateConnector(std::string API_tenant_UUID, std::string API_connector_UUID, EntityContext* context, std::string name={}) {
    return context->updateConnector(API_tenant_UUID, API_connector_UUID, name);
}

void deleteConnector(std::string API_tenant_UUID, std::string API_connector_UUID, EntityContext* context) {
    context->deleteConnector(API_tenant_UUID, API_connector_UUID);
}


}
