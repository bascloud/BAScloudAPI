#include "entity/Property.h"
#include "EntityContext.h"


namespace BAScloud {

Property::Property(std::string API_UUID, std::string API_tenant_UUID, std::string name, std::string street, std::string postalCode, std::string city, std::string country, std::time_t createdAt, std::time_t updatedAt, EntityContext* context) : 
    Entity(API_UUID, context), EntityDateMixin(createdAt, updatedAt), EntityTenantMixin(API_tenant_UUID), name(name), street(street), postal_code(postalCode), city(city), country(country) {

}

std::string Property::getName() {
    return name;
}

std::string Property::getStreet() {
    return street;
}

std::string Property::getPostalCode() {
    return postal_code;
}

std::string Property::getCity() {
    return city;
}

std::string Property::getCountry() {
    return country;
}

Property Property::getProperty(std::string API_tenant_UUID, std::string API_property_UUID, EntityContext* context) {
    return context->getProperty(API_tenant_UUID, API_property_UUID);
}

EntityCollection<Property> Property::getProperties(std::string API_tenant_UUID, EntityContext* context, PagingOption paging/*={}*/, std::string name/*={}*/, std::string street/*={}*/, std::string postalCode/*={}*/, std::string city/*={}*/, std::string country/*={}*/) {
    return context->getPropertiesCollection(API_tenant_UUID, paging, name, street, postalCode, city, country);
}

EntityCollection<Connector> Property::getAssociatedConnectors(PagingOption paging/*={}*/) {
    return context->getAssociatedConnectors(getTenantUUID(), getUUID(), paging);
}

Property createProperty(std::string API_tenant_UUID, std::string name, std::string street, std::string postalCode, std::string city, std::string country, EntityContext* context) {
    return context->createProperty(API_tenant_UUID, name, street, postalCode, city, country);
}

void deleteProperty(std::string API_tenant_UUID, std::string API_property_UUID, EntityContext* context) {

}
Property updateProperty(std::string API_tenant_UUID, std::string API_property_UUID, EntityContext* context, std::string name/*={}*/, std::string street/*={}*/, std::string postalCode/*={}*/, std::string city/*={}*/, std::string country/*={}*/) {
    return context->updateProperty(API_tenant_UUID, API_property_UUID, name, street, postalCode, city, country);
}


}