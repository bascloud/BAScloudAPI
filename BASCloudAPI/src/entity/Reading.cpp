#include "entity/Reading.h"
#include "EntityContext.h"


namespace BAScloud {

Reading::Reading(std::string API_UUID, std::string API_tenant_UUID, double value, std::time_t timestamp, std::time_t createdAt, std::time_t updatedAt, EntityContext* context) :
    Entity(API_UUID, context), EntityDateMixin(createdAt, updatedAt), EntityTenantMixin(API_tenant_UUID), value(value), timestamp(timestamp) {

}

double Reading::getValue() {
    return value;
}
std::time_t Reading::getTimestamp() {
    return timestamp;
}

Reading getReading(std::string API_tenant_UUID, std::string API_reading_UUID, EntityContext* context) {
    return context->getReading(API_tenant_UUID, API_reading_UUID);
}

EntityCollection<Reading> getReadings(std::string API_tenant_UUID, EntityContext* context, PagingOption paging/*={}*/, std::time_t from/*=-1*/, std::time_t until/*=-1*/, std::time_t timestamp/*=-1*/, double value/*=std::numeric_limits<double>::quiet_NaN()*/, std::string API_device_UUID/*=""*/) {
    return context->getReadingsCollection(API_tenant_UUID, paging, from, until, timestamp, value, API_device_UUID);
}

Device Reading::getAssociatedDevice() {
    return context->getAssociatedDevice(getTenantUUID(), getUUID());
}

Reading createReading(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp, EntityContext* context) {
    return context->createReading(API_tenant_UUID, API_device_UUID, value, timestamp);
}

void deleteReading(std::string API_tenant_UUID, std::string API_reading_UUID, EntityContext* context) {
    context->deleteReading(API_tenant_UUID, API_reading_UUID);
}


}
