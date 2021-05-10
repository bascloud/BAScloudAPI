#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>

#include "Paging.h"
#include "Util.h"

#include "entity/Entity.h"
#include "entity/EntityDateMixin.h"
#include "entity/EntityTenantMixin.h"
#include "error/Exceptions.h"

#include "EntityContext.h"



#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;

using namespace BAScloud;


PYBIND11_MODULE(pyBAScloudAPI, m) {
    m.doc() = R"pbdoc(
        BAScloud API bindings
        -----------------------
        .. currentmodule:: pyBAScloudAPI
        .. autosummary::
            :toctree: _generate
            
            EntityDateMixin
            EntityTenantMixin
            Entity
            Connector
            Device
            Property
            Reading
            SetPoint
            Tenant
            User
            PagingResult
            PagingOption
            EntityContext
            APIContext
            Util
    )pbdoc";

    py::class_<EntityDateMixin>(m, "EntityDateMixin")
        // Constructor is protected
        .def_property_readonly("createdAt", &EntityDateMixin::getCreatedDate)
        .def_property_readonly("updatedAt", &EntityDateMixin::getLastUpdatedDate);

    py::class_<EntityTenantMixin>(m, "EntityTenantMixin")
        // Constructor is protected
        .def_property_readonly("tenantUUID", &EntityTenantMixin::getTenantUUID);

    py::class_<Entity>(m, "Entity")
        // Constructor is protected
        .def_property_readonly("uuid", &Entity::getUUID);

    py::class_<Connector, Entity, EntityTenantMixin, EntityDateMixin>(m, "Connector")
        .def(py::init<const std::string&, const std::string&, const std::string&, const std::string&, const std::time_t&, const std::time_t&, EntityContext*>(), R"pbdoc(
            Connector entity class.
        )pbdoc", py::arg("UUID"), py::arg("tenantUUID"), py::arg("name"), py::arg("apiKey"), py::arg("createdAt"), py::arg("updateAt"), py::arg("context"))
        .def_property_readonly("name", &Connector::getName)
        .def_property_readonly("apiKey", &Connector::getAPIKey);

    py::class_<Device, Entity, EntityTenantMixin, EntityDateMixin>(m, "Device")
        .def(py::init<const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::time_t&, const std::time_t&, EntityContext*>(), R"pbdoc(
            Device entity class.
        )pbdoc", py::arg("UUID"), py::arg("tenantUUID"), py::arg("aksID"), py::arg("description"), py::arg("unit"), py::arg("createdAt"), py::arg("updateAt"), py::arg("context"))
        .def_property_readonly("aksID", &Device::getAksID)
        .def_property_readonly("description", &Device::getDescription)
        .def_property_readonly("unit", &Device::getUnit);

    py::class_<Property, Entity, EntityTenantMixin, EntityDateMixin>(m, "Property")
        .def(py::init<const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, 
            const std::time_t&, const std::time_t&, EntityContext*>(), R"pbdoc(
            Property entity class.
        )pbdoc", py::arg("UUID"), py::arg("tenantUUID"), py::arg("name"), py::arg("street"), py::arg("postalCode"), py::arg("city"), py::arg("country"), 
        py::arg("createdAt"), py::arg("updateAt"), py::arg("context"))
        .def_property_readonly("name", &Property::getName)
        .def_property_readonly("street", &Property::getStreet)
        .def_property_readonly("postalCode", &Property::getPostalCode)
        .def_property_readonly("city", &Property::getCity)
        .def_property_readonly("country", &Property::getCountry);

    py::class_<Reading, Entity, EntityTenantMixin, EntityDateMixin>(m, "Reading")
        .def(py::init<const std::string&, const std::string&, const double&, const std::time_t&, const std::time_t&, const std::time_t&, EntityContext*>(), R"pbdoc(
            Reading entity class.
        )pbdoc", py::arg("UUID"), py::arg("tenantUUID"), py::arg("value"), py::arg("timestamp"), py::arg("createdAt"), py::arg("updateAt"), py::arg("context"))
        .def_property_readonly("value", &Reading::getValue)
        .def_property_readonly("timestamp", &Reading::getTimestamp);

    py::class_<SetPoint, Entity, EntityTenantMixin, EntityDateMixin>(m, "SetPoint")
        .def(py::init<const std::string&, const std::string&, const double&, const std::time_t&, const std::time_t&, const std::time_t&, EntityContext*>(), R"pbdoc(
            SetPoint entity class.
        )pbdoc", py::arg("UUID"), py::arg("tenantUUID"), py::arg("value"), py::arg("timestamp"), py::arg("createdAt"), py::arg("updateAt"), py::arg("context"))
        .def_property_readonly("value", &SetPoint::getValue)
        .def_property_readonly("timestamp", &SetPoint::getTimestamp);

    py::class_<Tenant, Entity, EntityDateMixin>(m, "Tenant")
        .def(py::init<const std::string&, const std::string&, const std::string&, const std::time_t&, const std::time_t&, EntityContext*>(), R"pbdoc(
            Tenant entity class.
        )pbdoc", py::arg("UUID"), py::arg("name"), py::arg("urlName"), py::arg("createdAt"), py::arg("updateAt"), py::arg("context"))
        .def_property_readonly("name", &Tenant::getName)
        .def_property_readonly("urlName", &Tenant::getUrlName);

    py::class_<User, Entity, EntityDateMixin>(m, "User")
        .def(py::init<const std::string&, const std::string&, const std::time_t&, const std::time_t&, EntityContext*>(), R"pbdoc(
            User entity class.
        )pbdoc", py::arg("UUID"), py::arg("email"), py::arg("createdAt"), py::arg("updateAt"), py::arg("context"))
        .def_property_readonly("email", &User::getEmail);


    py::class_<PagingResult>(m, "PagingResult")
        .def_readwrite("nextPagePointer", &PagingResult::nextPagePointer)
        .def_readwrite("previousPagePointer", &PagingResult::previousPagePointer)
        .def_readwrite("pageSize", &PagingResult::pageSize)
        .def_readwrite("currentPage", &PagingResult::currentPage)
        .def_readwrite("totalPages", &PagingResult::totalPages)
        .def_readwrite("count", &PagingResult::count);


    py::class_<PagingOption> pagingOption(m, "PagingOption");

    py::enum_<PagingOption::Direction>(pagingOption, "Direction")
        .value("NEXT", PagingOption::Direction::NEXT)
        .value("PREVIOUS", PagingOption::Direction::PREVIOUS)
        .value("NONE", PagingOption::Direction::NONE)
        .export_values();

    pagingOption.def(py::init<int&, PagingOption::Direction&, std::string&>(), R"pbdoc(
            Paging Option.
        )pbdoc", py::arg("pageSize")=1000, py::arg("direction")=PagingOption::Direction::NONE, py::arg("pagePointer")="")
        .def_readwrite("pageSize", &PagingOption::page_size)
        .def_readwrite("direction", &PagingOption::direction)
        .def_readwrite("pagePointer", &PagingOption::page_pointer);



    // py::cpp_function default_error_handler_func() {
    //     return py::cpp_function(py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")),
    //     py::arg("e"), py::arg("json"));
    // }

    py::class_<EntityContext>(m, "EntityContext")
        .def(py::init<const std::string&>(), R"pbdoc(
            EntityContext.
        )pbdoc", py::arg("apiServerURL"))
        .def("authenticateWithUserLogin", &EntityContext::authenticateWithUserLogin, R"pbdoc(
            Authenticate a user against the BAScloud API.
        )pbdoc", py::arg("email"), py::arg("password"))
        .def("authenticateWithConnectorToken", &EntityContext::authenticateWithConnectorToken, R"pbdoc(
            Authenticate a connector using a valid connector token.
        )pbdoc", py::arg("connectorToken"))

        .def("getToken", &EntityContext::getToken, R"pbdoc(
            Get the authentication token.
        )pbdoc")
        .def("getTokenExpirationDate", &EntityContext::getTokenExpirationDate, R"pbdoc(
            Get the expiration date of the authentication token.
        )pbdoc")
        .def("isAuthenticated", &EntityContext::isAuthenticated, R"pbdoc(
            Returns wherever the current EntityContext is authenticated.
        )pbdoc")

        .def("getUser", &EntityContext::getUser, R"pbdoc(
            Request a single User entity.
        )pbdoc", py::arg("userUUID"))
        .def("getUsersCollection", &EntityContext::getUsersCollection, R"pbdoc(
            Request a collection of User entities.
        )pbdoc", py::arg("email")="", py::arg("paging")=PagingOption(), py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){}, py::arg("e"), py::arg("json")))

        .def("getAssociatedTenant", &EntityContext::getAssociatedTenant, R"pbdoc(
            Get a the associated Tenant entity of the User.
        )pbdoc", py::arg("userUUID"))

        .def("requestUserPasswordReset", &EntityContext::requestUserPasswordReset, R"pbdoc(
            Request a password reset for the User.
        )pbdoc", py::arg("email"))
        .def("updateUserPassword", &EntityContext::updateUserPassword, R"pbdoc(
            Changes the password of a User.
        )pbdoc", py::arg("userUUID"), py::arg("resetToken"), py::arg("newPassword"))
        .def("createNewUser", &EntityContext::createNewUser, R"pbdoc(
            Authenticate with user login.
        )pbdoc", py::arg("email"), py::arg("password"))
        .def("updateUser", &EntityContext::updateUser, R"pbdoc(
            Authenticate with user login.
        )pbdoc", py::arg("userUUID"), py::arg("email"))
        .def("deleteUser", &EntityContext::deleteUser, R"pbdoc(
            Deletes an existing User in the BAScloud. [Admin] 
        )pbdoc", py::arg("userUUID"))

        .def("getTenant", &EntityContext::getTenant, R"pbdoc(
            Request a single Tenant entity.
        )pbdoc", py::arg("tenantUUID"))
        .def("getTenantsCollection", &EntityContext::getTenantsCollection, R"pbdoc(
            Request a collection of Tenant entities.
        )pbdoc", py::arg("paging")=PagingOption(), py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("getAssociatedUsers", &EntityContext::getAssociatedUsers, R"pbdoc(
            Get a collection of associated User entities of the Tenant.
        )pbdoc", py::arg("tenantUUID"), py::arg("paging")=PagingOption(), py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("createTenant", &EntityContext::createTenant, R"pbdoc(
            Creates a new Tenant entity in the BAScloud. [Admin] 
        )pbdoc", py::arg("name"), py::arg("userUUID"))
        .def("deleteTenant", &EntityContext::deleteTenant, R"pbdoc(
            Deletes an existing Tenant in the BAScloud. [Admin] 
        )pbdoc", py::arg("tenantUUID"))
        .def("updateTenant", &EntityContext::updateTenant, R"pbdoc(
            Updates the information of a Tenant entity in the BAScloud. [Admin] 
        )pbdoc", py::arg("tenantUUID"), py::arg("name")="")
        .def("assignTenantUsers", &EntityContext::assignTenantUsers, R"pbdoc(
            Assigns a collection of User entities to a Tenant. [Admin] 
        )pbdoc", py::arg("tenantUUID"), py::arg("userUUIDs"))
        .def("removeTenantUsers", &EntityContext::removeTenantUsers, R"pbdoc(
            Removes a collection of User entities from a Tenant. [Admin] 
        )pbdoc", py::arg("tenantUUID"), py::arg("userUUIDs"))

        .def("getProperty", &EntityContext::getProperty, R"pbdoc(
            Request a single Property entity.
        )pbdoc", py::arg("tenantUUID"), py::arg("propertyUUID"))
        .def("getPropertiesCollection", &EntityContext::getPropertiesCollection, R"pbdoc(
            Request a collection of Property entities grouped under the given Tenant.
        )pbdoc", py::arg("tenantUUID"), py::arg("paging")=PagingOption(), py::arg("name")="", py::arg("street")="", 
            py::arg("postalCode")="", py::arg("city")="", py::arg("country")="", py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("getAssociatedConnectors", &EntityContext::getAssociatedConnectors, R"pbdoc(
            Get a collection of associated Connector entities of a Property.
        )pbdoc", py::arg("tenantUUID"), py::arg("propertyUUID"), py::arg("paging")=PagingOption(), py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("createProperty", &EntityContext::createProperty, R"pbdoc(
            Create a new Property entity in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("name"), py::arg("street"), 
            py::arg("postalCode"), py::arg("city"), py::arg("country"))
        .def("deleteProperty", &EntityContext::deleteProperty, R"pbdoc(
            Deletes an existing Property in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("propertyUUID"))
        .def("updateProperty", &EntityContext::updateProperty, R"pbdoc(
            Update an existing Property in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("propertyUUID"), py::arg("name")="", py::arg("street")="", 
            py::arg("postalCode")="", py::arg("city")="", py::arg("country")="")

        .def("getConnector", &EntityContext::getConnector, R"pbdoc(
            Request a single Connector entity.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"))
        .def("getConnectorsCollection", &EntityContext::getConnectorsCollection, R"pbdoc(
            Request a collection of Connector entities grouped under the given Tenant.
        )pbdoc", py::arg("tenantUUID"), py::arg("paging")=PagingOption(), py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("getAssociatedProperty", &EntityContext::getAssociatedProperty, R"pbdoc(
            Get the associated Property entity of the Connector.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"))
        .def("getAssociatedDevices", &EntityContext::getAssociatedDevices, R"pbdoc(
            Get the associated Device entities of the Connector.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"), py::arg("paging")=PagingOption(), py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("createConnector", &EntityContext::createConnector, R"pbdoc(
            Create a new Connector entity in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"), py::arg("name"))
        .def("deleteConnector", &EntityContext::deleteConnector, R"pbdoc(
            Deletes an existing Connector in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"))
        .def("updateConnector", &EntityContext::updateConnector, R"pbdoc(
            Update an existing Connector in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"), py::arg("name")="")
        .def("getNewConnectorAuthToken", &EntityContext::getNewConnectorAuthToken, R"pbdoc(
            Requests a new API key for a Connector entity.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"))

        .def("getDevice", &EntityContext::getDevice, R"pbdoc(
            Request a single Device entity.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"))
        .def("getDevicesCollection", &EntityContext::getDevicesCollection, R"pbdoc(
            Request a collection of Device entities grouped under the given Tenant.
        )pbdoc", py::arg("tenantUUID"), py::arg("paging")=PagingOption(), py::arg("aksID")="", py::arg("description")="", 
            py::arg("unit")="", py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("getAssociatedConnector", &EntityContext::getAssociatedConnector, R"pbdoc(
            Get the associated Connector entity of the Device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"))
        .def("getAssociatedReadings", &EntityContext::getAssociatedReadings, R"pbdoc(
            Get a collection of associated Reading entities of the Device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"), py::arg("paging")=PagingOption(), py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("getAssociatedSetPoints", &EntityContext::getAssociatedSetPoints, R"pbdoc(
            Get a collection of associated Reading entities of the Device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"), py::arg("paging")=PagingOption(), py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("createDevice", &EntityContext::createDevice, R"pbdoc(
            Create a new Device entity in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"), py::arg("aksID"), py::arg("description"), 
            py::arg("unit"))
        .def("deleteDevice", &EntityContext::deleteDevice, R"pbdoc(
            Deletes an existing Device in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"))
        .def("updateDevice", &EntityContext::updateDevice, R"pbdoc(
            Update an existing Device in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"), py::arg("aksID")="", py::arg("description")="", 
            py::arg("unit")="")
            
        .def("getReading", &EntityContext::getReading, R"pbdoc(
            Request a single Reading entity.
        )pbdoc", py::arg("tenantUUID"), py::arg("readingUUID"))
        .def("getReadingsCollection", &EntityContext::getReadingsCollection, R"pbdoc(
            Request a collection of Reading entities grouped under the given Tenant.
        )pbdoc", py::arg("tenantUUID"), py::arg("paging")=PagingOption(), py::arg("from_")=-1, py::arg("until")=-1, 
            py::arg("timestamp")=-1, py::arg("value")=std::numeric_limits<double>::quiet_NaN(), py::arg("deviceUUID")="", py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("getAssociatedDevice", &EntityContext::getAssociatedDevice, R"pbdoc(
            Get the associated Device entity of the Reading.
        )pbdoc", py::arg("tenantUUID"), py::arg("readingUUID"))
        .def("createReading", &EntityContext::createReading, R"pbdoc(
            Create a new Reading entity in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"), py::arg("value"), py::arg("timestamp"))
        .def("deleteReading", &EntityContext::deleteReading, R"pbdoc(
            Deletes an existing Reading in the BAScloud. [Admin] 
        )pbdoc", py::arg("tenantUUID"), py::arg("readingUUID"))
            
        .def("getSetPoint", &EntityContext::getSetPoint, R"pbdoc(
            Request a single SetPoint entity.
        )pbdoc", py::arg("tenantUUID"), py::arg("setpointUUID"))
        .def("getSetPointsCollection", &EntityContext::getSetPointsCollection, R"pbdoc(
            Request a collection of SetPoint entities grouped under the given Tenant.
        )pbdoc", py::arg("tenantUUID"), py::arg("paging")=PagingOption(), py::arg("from_")=-1, py::arg("until")=-1, 
            py::arg("timestamp")=-1, py::arg("currentTime")=-1, py::arg("deviceUUID")="", py::arg("errorHandler")=py::cpp_function([](std::exception& e, json& j){},py::arg("e"), py::arg("json")))
        .def("createSetPoint", &EntityContext::createSetPoint, R"pbdoc(
            Create a new SetPoint entity in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"), py::arg("value"), py::arg("timestamp"));


    py::class_<APIContext>(m, "APIContext")
        .def(py::init<const std::string&>(), R"pbdoc(
            APIContext constructor.
        )pbdoc", py::arg("apiServerURL"))
        .def("setAPIURL", &APIContext::setAPIURL, R"pbdoc(
            Set the URL of the BAScloud API instance.
        )pbdoc", py::arg("apiServerURL"))

        .def("getAPIURL", &APIContext::getAPIURL, R"pbdoc(
            Get the currently used URL of the BAScloud API instance.
        )pbdoc")
        .def("setToken", &APIContext::setToken, R"pbdoc(
            Set the authentication token.
        )pbdoc", py::arg("token"))
        .def("getToken", &APIContext::getToken, R"pbdoc(
            Get the currently used authentication token for the API requests.
        )pbdoc")
        .def("requestAuthenticationLogin", &APIContext::requestAuthenticationLogin, R"pbdoc(
            Request the authentication of a user per login credentials.
        )pbdoc", py::arg("email"), py::arg("password"))

        .def("requestUser", &APIContext::requestUser, R"pbdoc(
            Request a single User entity.
        )pbdoc", py::arg("userUUID"))
        .def("requestUserCollection", &APIContext::requestUserCollection, R"pbdoc(
            Request a user collection with optional filter.
        )pbdoc", py::arg("email")="")

        .def("requestUserTenantRelationship", &APIContext::requestUserTenantRelationship, R"pbdoc(
            Request the user tenant relationship of a given user.
        )pbdoc", py::arg("userUUID"))
        .def("requestUserAssociatedTenant", &APIContext::requestUserAssociatedTenant, R"pbdoc(
            Request the associated tenant of a given user.
        )pbdoc", py::arg("userUUID"))

        .def("requestUserPasswordReset", &APIContext::requestUserPasswordReset, R"pbdoc(
            Request to password reset for an existing user. 
        )pbdoc", py::arg("email"))
        .def("requestUserPasswordChange", &APIContext::requestUserPasswordChange, R"pbdoc(
            Request to change the password for an existing user. 
        )pbdoc", py::arg("userUUID"), py::arg("resetToken"), py::arg("newPassword"))
        .def("requestUserSignup", &APIContext::requestUserSignup, R"pbdoc(
            Request to signup a new user. 
        )pbdoc", py::arg("email"), py::arg("password"))
        .def("requestUpdateUser", &APIContext::requestUpdateUser, R"pbdoc(
            Request to update information of an existing user. [Admin] 
        )pbdoc", py::arg("userUUID"), py::arg("email"))
        .def("requestDeleteUser", &APIContext::requestDeleteUser, R"pbdoc(
            Request the deletion of an existing user. [Admin] 
        )pbdoc", py::arg("userUUID"))

        .def("requestTenant", &APIContext::requestTenant, R"pbdoc(
            Request a single tenant entity by UUID.
        )pbdoc", py::arg("tenantUUID"))
        .def("requestTenantCollection", &APIContext::requestTenantCollection, R"pbdoc(
            Request a tenant collection.
        )pbdoc")
        .def("requestTenantUsersRelationship", &APIContext::requestTenantUsersRelationship, R"pbdoc(
            Request the tenant users relationships of a given tenant.
        )pbdoc", py::arg("tenantUUID"))
        .def("requestTenantAssociatedUsers", &APIContext::requestTenantAssociatedUsers, R"pbdoc(
            Request the associated users of a given tenant with optional paging.
        )pbdoc", py::arg("tenantUUID"), py::arg("pageSize")=-1, py::arg("pageBefore")="", py::arg("pageAfter")="")
        .def("requestCreateTenant", &APIContext::requestCreateTenant, R"pbdoc(
            Request the creation of a tenant entity. [Admin] 
        )pbdoc", py::arg("name"), py::arg("userUUID"))
        .def("requestDeleteTenant", &APIContext::requestDeleteTenant, R"pbdoc(
            Request the deletion of a tenant entity. [Admin] 
        )pbdoc", py::arg("tenantUUID"))
        .def("requestUpdateTenant", &APIContext::requestUpdateTenant, R"pbdoc(
            Request the information update of a tenant entity. [Admin] 
        )pbdoc", py::arg("tenantUUID"), py::arg("name"))
        .def("requestAssignTenantUsers", &APIContext::requestAssignTenantUsers, R"pbdoc(
            Request to assign a collection of User entities to a tenant. [Admin] 
        )pbdoc", py::arg("tenantUUID"), py::arg("userUUIDs"))
        .def("requestRemoveTenantUsers", &APIContext::requestRemoveTenantUsers, R"pbdoc(
            Request to remove a collection of User entities from a tenant. [Admin] 
        )pbdoc", py::arg("tenantUUID"), py::arg("userUUIDs"))

        .def("requestProperty", &APIContext::requestProperty, R"pbdoc(
            Request a single property entity by UUID.
        )pbdoc", py::arg("tenantUUID"), py::arg("propertyUUID"))
        .def("requestPropertyCollection", &APIContext::requestPropertyCollection, R"pbdoc(
            Request a collection of Property entities grouped under the given Tenant.
        )pbdoc", py::arg("tenantUUID"), py::arg("name")="", py::arg("street")="", 
        py::arg("postalCode")="", py::arg("city")="", py::arg("country")="", py::arg("pageSize")=-1, py::arg("pageBefore")="", py::arg("pageAfter")="")
        .def("requestPropertyConnectorsRelationship", &APIContext::requestPropertyConnectorsRelationship, R"pbdoc(
            Request the connectors relationships of a given property.
        )pbdoc", py::arg("tenantUUID"), py::arg("propertyUUID"))
        .def("requestPropertyAssociatedConnectors", &APIContext::requestPropertyAssociatedConnectors, R"pbdoc(
            Request the connectors relationships of a given property.
        )pbdoc", py::arg("tenantUUID"), py::arg("propertyUUID"), py::arg("pageSize")=-1, py::arg("pageBefore")="", py::arg("pageAfter")="")
        .def("requestCreateProperty", &APIContext::requestCreateProperty, R"pbdoc(
            Requests the creation a new property.
        )pbdoc", py::arg("tenantUUID"), py::arg("name"), py::arg("street"), 
        py::arg("postalCode"), py::arg("city"), py::arg("country"))
        .def("requestDeleteProperty", &APIContext::requestDeleteProperty, R"pbdoc(
            Deletes an existing Property in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("propertyUUID"))
        .def("requestUpdateProperty", &APIContext::requestUpdateProperty, R"pbdoc(
            Update an existing Property in the BAScloud.
        )pbdoc", py::arg("tenantUUID"), py::arg("propertyUUID"), py::arg("name")="", py::arg("street")="", 
        py::arg("postalCode")="", py::arg("city")="", py::arg("country")="")

        .def("requestConnector", &APIContext::requestConnector, R"pbdoc(
            Request a single connector entity by UUID.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"))
        .def("requestConnectorCollection", &APIContext::requestConnectorCollection, R"pbdoc(
            Request a collection of Connector entities grouped under the given Tenant.
        )pbdoc", py::arg("tenantUUID"), py::arg("pageSize")=-1, py::arg("pageBefore")="", py::arg("pageAfter")="")
        .def("requestConnectorPropertyRelationship", &APIContext::requestConnectorPropertyRelationship, R"pbdoc(
            Request the property relationships of a given connector.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"))
        .def("requestConnectorAssociatedProperty", &APIContext::requestConnectorAssociatedProperty, R"pbdoc(
            Request the associated property of a given connector.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"))
        .def("requestConnectorDevicesRelationship", &APIContext::requestConnectorDevicesRelationship, R"pbdoc(
            Request the devices relationships of a given connector.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"))
        .def("requestConnectorAssociatedDevices", &APIContext::requestConnectorAssociatedDevices, R"pbdoc(
            Request the associated devices of a given connector.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"), py::arg("pageSize")=-1, py::arg("pageBefore")="", py::arg("pageAfter")="")
        .def("requestCreateConnector", &APIContext::requestCreateConnector, R"pbdoc(
            Requests the creation a new connector.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"), py::arg("name"))
        .def("requestDeleteConnector", &APIContext::requestDeleteConnector, R"pbdoc(
            Requests the deletion of an existing connector.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"))
        .def("requestUpdateConnector", &APIContext::requestUpdateConnector, R"pbdoc(
            Requests the update of an existing connector.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"), py::arg("name")="")
        .def("requestConnectorToken", &APIContext::requestConnectorToken, R"pbdoc(
            Requests a new API key for a connector entity.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"))

        .def("requestDevice", &APIContext::requestDevice, R"pbdoc(
            Request a single device entity by UUID.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"))
        .def("requestDeviceCollection", &APIContext::requestDeviceCollection, R"pbdoc(
            Request a device collection with optional filters.
        )pbdoc", py::arg("tenantUUID"), py::arg("aksID")="", py::arg("description")="", 
        py::arg("unit")="", py::arg("pageSize")=-1, py::arg("pageBefore")="", py::arg("pageAfter")="")
        .def("requestDeviceConnectorRelationship", &APIContext::requestDeviceConnectorRelationship, R"pbdoc(
            Request the connector relationship of a given device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"))
        .def("requestDeviceAssociatedConnector", &APIContext::requestDeviceAssociatedConnector, R"pbdoc(
            Request the associated connector of a given device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"))
        .def("requestDeviceReadingsRelationship", &APIContext::requestDeviceReadingsRelationship, R"pbdoc(
            Request the readings releationship of a given device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"))
        .def("requestDeviceAssociatedReadings", &APIContext::requestDeviceAssociatedReadings, R"pbdoc(
            Request the associated readings of a given device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"), py::arg("pageSize")=-1, py::arg("pageBefore")="", py::arg("pageAfter")="")
        .def("requestDeviceSetPointsRelationship", &APIContext::requestDeviceSetPointsRelationship, R"pbdoc(
            Request the setpoints releationship of a given device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"))
        .def("requestDeviceAssociatedSetPoints", &APIContext::requestDeviceAssociatedSetPoints, R"pbdoc(
            Request the associated setpoints of a given device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"), py::arg("pageSize")=-1, py::arg("pageBefore")="", py::arg("pageAfter")="")
        .def("requestCreateDevice", &APIContext::requestCreateDevice, R"pbdoc(
            Request the creation of a new device entity given an associated connector.
        )pbdoc", py::arg("tenantUUID"), py::arg("connectorUUID"), py::arg("aksID"), py::arg("description"), 
        py::arg("unit"))
        .def("requestDeleteDevice", &APIContext::requestDeleteDevice, R"pbdoc(
            Request the deletion of an existing device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"))
        .def("requestUpdateDevice", &APIContext::requestUpdateDevice, R"pbdoc(
            Request an update of an existing device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"), py::arg("aksID")="", py::arg("description")="", 
        py::arg("unit")="")
            
        .def("requestReading", &APIContext::requestReading, R"pbdoc(
            Request a single reading entity by UUID.
        )pbdoc", py::arg("tenantUUID"), py::arg("readingUUID"))
        .def("requestReadingCollection", &APIContext::requestReadingCollection, R"pbdoc(
            Request a reading collection with optional filters.
        )pbdoc", py::arg("tenantUUID"), py::arg("from_")=-1, py::arg("until")=-1, 
        py::arg("timestamp")=-1, py::arg("value")=std::numeric_limits<double>::quiet_NaN(), py::arg("deviceUUID")="", py::arg("pageSize")=-1, py::arg("pageBefore")="", py::arg("pageAfter")="")
        .def("requestReadingDeviceRelationship", &APIContext::requestReadingDeviceRelationship, R"pbdoc(
            Request the device releationship of a given reading.
        )pbdoc", py::arg("tenantUUID"), py::arg("readingUUID"))
        .def("requestReadingAssociatedDevice", &APIContext::requestReadingAssociatedDevice, R"pbdoc(
            Request the associated device of a given reading.
        )pbdoc", py::arg("tenantUUID"), py::arg("readingUUID"))
        .def("requestCreateReading", &APIContext::requestCreateReading, R"pbdoc(
            Request the creation of a new reading entity given an associated device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"), py::arg("value"), py::arg("timestamp"))
        .def("requestDeleteReading", &APIContext::requestDeleteReading, R"pbdoc(
            Request the deletion of an existing reading entity.
        )pbdoc", py::arg("tenantUUID"), py::arg("readingUUID"))
            
        .def("requestSetPoint", &APIContext::requestSetPoint, R"pbdoc(
            Request a single setpoint entity by UUID.
        )pbdoc", py::arg("tenantUUID"), py::arg("setpointUUID"))
        .def("requestSetPointCollection", &APIContext::requestSetPointCollection, R"pbdoc(
            Request a setpoint collection with optional filters.
        )pbdoc", py::arg("tenantUUID"), py::arg("from_")=-1, py::arg("until")=-1, 
            py::arg("timestamp")=-1, py::arg("currentTime")=-1, py::arg("deviceUUID")="", py::arg("pageSize")=-1, py::arg("pageBefore")="", py::arg("pageAfter")="")
        .def("requestCreateSetPoint", &APIContext::requestCreateSetPoint, R"pbdoc(
            Request the creation of a new setpoint entity given an associated device.
        )pbdoc", py::arg("tenantUUID"), py::arg("deviceUUID"), py::arg("value"), py::arg("timestamp"));


    py::class_<Util>(m, "Util")
        .def_static("parseDateTimeString", &Util::parseDateTimeString, R"pbdoc(
            Parses DateTime string to UNIX timestamp. 
        )pbdoc", py::arg("dateTime"))
        .def_static("parseURLParameter", &Util::parseURLParameter, R"pbdoc(
            Parses a URL and returns a dict of parameter key-value pairs. 
        )pbdoc", py::arg("url"));

    py::register_exception<ServerError>(m, "ServerError", PyExc_RuntimeError);
    py::register_exception<ConnectionError>(m, "ConnectionError", PyExc_RuntimeError);
    py::register_exception<NotFoundRequest>(m, "NotFoundRequest", PyExc_RuntimeError);
    py::register_exception<BadRequest>(m, "BadRequest", PyExc_RuntimeError);
    py::register_exception<ConflictRequest>(m, "ConflictRequest", PyExc_RuntimeError);
    py::register_exception<UnauthorizedRequest>(m, "UnauthorizedRequest", PyExc_RuntimeError);
    py::register_exception<InvalidResponse>(m, "InvalidResponse", PyExc_RuntimeError);

    // py::class_<ServerError>(m, "ServerError")
    //     .def(py::init<const std::string&>(), R"pbdoc(
    //             ServerError exception.
    //         )pbdoc", py::arg("message"));

    // py::class_<ConnectionError>(m, "ConnectionError")
    //     .def(py::init<const std::string&>(), R"pbdoc(
    //             ConnectionError exception.
    //         )pbdoc", py::arg("message"));

    // py::class_<NotFoundRequest>(m, "NotFoundRequest")
    //     .def(py::init<const std::string&>(), R"pbdoc(
    //             NotFoundRequest exception.
    //         )pbdoc", py::arg("message"));

    // py::class_<BadRequest>(m, "BadRequest")
    //     .def(py::init<const std::string&>(), R"pbdoc(
    //             BadRequest exception.
    //         )pbdoc", py::arg("message"));

    // py::class_<ConflictRequest>(m, "ConflictRequest")
    //     .def(py::init<const std::string&>(), R"pbdoc(
    //             ConflictRequest exception.
    //         )pbdoc", py::arg("message"));

    // py::class_<UnauthorizedRequest>(m, "UnauthorizedRequest")
    //     .def(py::init<const std::string&>(), R"pbdoc(
    //             UnauthorizedRequest exception.
    //         )pbdoc", py::arg("message"));

    // py::class_<InvalidResponse>(m, "InvalidResponse")
    //     .def(py::init<const std::string&>(), R"pbdoc(
    //             InvalidResponse exception.
    //         )pbdoc", py::arg("message"));


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
