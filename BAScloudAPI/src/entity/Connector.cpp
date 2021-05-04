#include "entity/Connector.h"
#include "EntityContext.h"


namespace BAScloud {

Connector::Connector(std::string API_UUID, std::string API_tenant_UUID, std::string name, std::string apiKey, std::time_t createdAt, std::time_t updatedAt, EntityContext* context) : 
    Entity(API_UUID, context), EntityDateMixin(createdAt, updatedAt), EntityTenantMixin(API_tenant_UUID), name(name), api_key(apiKey) {

}

std::string Connector::getName() {
    return  name;
}

void Connector::setAPIKey(std::string newApiKey) {
    api_key = newApiKey;
}

std::string Connector::getAPIKey() {
    return api_key;
}

Connector Connector::getConnector(std::string API_tenant_UUID, std::string API_connector_UUID, EntityContext* context) {
    return context->getConnector(API_tenant_UUID, API_connector_UUID);
}

EntityCollection<Connector> Connector::getConnectors(std::string API_tenant_UUID, EntityContext* context, PagingOption paging/*={}*/) {
    return context->getConnectorsCollection(API_tenant_UUID, paging);  
}

void Connector::refreshAuthToken() {
    api_key = context->getNewConnectorAuthToken(getTenantUUID(), getUUID());
}

Property Connector::getAssociatedProperty() {
    return context->getAssociatedProperty(getTenantUUID(), getUUID());
}

EntityCollection<Device> Connector::getAssociatedDevices(PagingOption paging/*={}*/) {
    return context->getAssociatedDevices(getTenantUUID(), getUUID(), paging);
}

Connector createConnector(std::string API_tenant_UUID, std::string API_property_UUID, std::string name, EntityContext* context) {
    return context->createConnector(API_tenant_UUID, API_property_UUID, name);
}

Connector updateConnector(std::string API_tenant_UUID, std::string API_connector_UUID, EntityContext* context, std::string name={}) {
    return context->updateConnector(API_tenant_UUID, API_connector_UUID, name);
}

void deleteConnector(std::string API_tenant_UUID, std::string API_connector_UUID, EntityContext* context) {
    context->deleteConnector(API_tenant_UUID, API_connector_UUID);
}


}
