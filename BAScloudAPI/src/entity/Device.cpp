#include "entity/Device.h"
#include "EntityContext.h"


namespace BAScloud {

Device::Device(std::string API_UUID, std::string API_tenant_UUID, std::string aksID, std::string localAksID, std::string description, std::string unit, std::time_t createdAt, std::time_t updatedAt, EntityContext* context) : 
    Entity(API_UUID, context), EntityDateMixin(createdAt, updatedAt), EntityTenantMixin(API_tenant_UUID), aks_ID(aksID), local_aks_ID(localAksID), description(description), unit(unit) {

}

std::string Device::getAksID() {
    return aks_ID;
}

std::string Device::getLocalAksID() {
    return local_aks_ID;
}

std::string Device::getDescription() {
    return description;
}

std::string Device::getUnit() {
    return unit;
}

Device Device::getDevice(std::string API_tenant_UUID, std::string API_device_UUID, EntityContext* context) {
    return context->getDevice(API_tenant_UUID, API_device_UUID);
}

EntityCollection<Device> Device::getDevices(std::string API_tenant_UUID, EntityContext* context, PagingOption paging/*={}*/, std::string aksID/*={}*/, std::string localAksID/*={}*/, std::string API_connector_UUID/*={}*/, std::string API_property_UUID/*={}*/, std::string description/*={}*/, std::string unit/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/, std::time_t deletedUntil/*=-1*/) {
    return context->getDevicesCollection(API_tenant_UUID, paging, aksID, localAksID, API_connector_UUID, API_property_UUID, description, unit, createdFrom, createdUntil, deletedUntil);
}

Connector Device::getAssociatedConnector() {
    return context->getAssociatedConnector(getTenantUUID(), getUUID());
}

EntityCollection<Reading> Device::getAssociatedReadings(PagingOption paging/*={}*/, std::time_t from/*=-1*/, std::time_t until/*=-1*/, std::time_t timestamp/*=-1*/, double value/*=std::numeric_limits<double>::quiet_NaN()*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/) {
    return context->getAssociatedReadings(getTenantUUID(), getUUID(), paging, from, until, timestamp, value, createdFrom, createdUntil);
}

EntityCollection<SetPoint> Device::getAssociatedSetPoints(PagingOption paging/*={}*/, std::time_t from/*=-1*/, std::time_t until/*=-1*/, std::time_t timestamp/*=-1*/, std::time_t currentTime/*=-1*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/) {
    return context->getAssociatedSetPoints(getTenantUUID(), getUUID(), paging, from, until, timestamp, currentTime, createdFrom, createdUntil);
}

Device createDevice(std::string API_tenant_UUID, std::string API_connector_UUID, std::string API_property_UUID, std::string aksID, std::string description, std::string unit, EntityContext* context) {
    return context->createDevice(API_tenant_UUID, API_connector_UUID, API_property_UUID, aksID, description, unit);
}

Device updateDevice(std::string API_tenant_UUID, std::string API_device_UUID, EntityContext* context, std::string aksID={}, std::string description={}, std::string unit={}) {
    return context->updateDevice(API_tenant_UUID, API_device_UUID, aksID, description, unit);
}

void deleteDevice(std::string API_tenant_UUID, std::string API_device_UUID, EntityContext* context) {
    context->deleteDevice(API_tenant_UUID, API_device_UUID);
}


}
