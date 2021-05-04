#include "entity/SetPoint.h"
#include "EntityContext.h"


namespace BAScloud {

SetPoint::SetPoint(std::string API_UUID, std::string API_tenant_UUID, double value, std::time_t timestamp, std::time_t createdAt, std::time_t updatedAt, EntityContext* context) :
    Entity(API_UUID, context), EntityDateMixin(createdAt, updatedAt), EntityTenantMixin(API_tenant_UUID), value(value), timestamp(timestamp) {

}

double SetPoint::getValue() {
    return value;
}
std::time_t SetPoint::getTimestamp() {
    return timestamp;
}

SetPoint getSetPoint(std::string API_tenant_UUID, std::string API_setpoint_UUID, EntityContext* context) {
    return context->getSetPoint(API_tenant_UUID, API_setpoint_UUID);
}

EntityCollection<SetPoint> getSetPoints(std::string API_tenant_UUID, EntityContext* context, PagingOption paging/*={}*/, std::time_t from/*=-1*/, std::time_t until/*=-1*/, std::time_t timestamp/*=-1*/, std::time_t currentTime/*=-1*/, std::string API_device_UUID/*=""*/) {
    return context->getSetPointsCollection(API_tenant_UUID, paging, from, until, timestamp, currentTime, API_device_UUID);
}

SetPoint createSetPoint(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp, EntityContext* context) {
    return context->createSetPoint(API_tenant_UUID, API_device_UUID, value, timestamp);
}


}
