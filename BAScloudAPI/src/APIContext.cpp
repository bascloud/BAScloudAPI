#include <limits>
#include <iostream>

#include <fmt/core.h>
#include <fmt/chrono.h>

#include "APIContext.h"


namespace BAScloud {

APIContext::APIContext(std::string API_server_URL) : 
    API_server_URL(API_server_URL) {
    
}

void APIContext::setAPIURL(std::string API_server_URL) {
    API_server_URL = API_server_URL;
}

std::string APIContext::getAPIURL() {
    return API_server_URL;
}

void APIContext::setToken(std::string new_API_token) {
    API_token = new_API_token;
}

std::string APIContext::getToken() {
    return API_token;
}

// API endpoint request implementations
cpr::Response APIContext::requestAuthenticationLogin(std::string API_email, std::string API_password) {
    
    std::string request_body = json({
        {"data", {
            {"type", "credentials"}, 
            {"attributes", {
                    {"email", API_email}, 
                    {"password", API_password}
                }
            }
        }
        }}).dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + API_AUTHENTICATION_PATH),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body));

    return r;
}

// User API endpoints
cpr::Response APIContext::requestUserSignup(std::string email, std::string password) {
    
    std::string request_body = json({
        {"data", {
            {"type", "users"}, 
            {"attributes", {
                    {"email", email}, 
                    {"password", password}
                }
            }
        }
        }}).dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_USER_SIGNUP_PATH)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body));

    return r;
}

cpr::Response APIContext::requestUserPasswordReset(std::string email) {
    
    std::string request_body = json({
        {"data", {
            {"type", "resetPassword"}, 
            {"attributes", {
                    {"email", email} 
                }
            }
        }
        }}).dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_USER_RESET_PASSWORD_PATH)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body));

    return r;
}

cpr::Response APIContext::requestUserPasswordChange(std::string API_user_UUID, std::string API_reset_token, std::string new_password) {
    
    std::string request_body = json({
        {"data", {
            {"type", "changePassword"}, 
            {"attributes", {
                    {"token", API_reset_token},
                    {"userId", API_user_UUID},
                    {"password", new_password}
                }
            }
        }
        }}).dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_USER_CHANGE_PASSWORD_PATH)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body));

    return r;
}

cpr::Response APIContext::requestUpdateUser(std::string API_user_UUID, std::string email) {

    json request_json = json({
        {"data", {
            {"type", "users"}, 
            {"id", API_user_UUID},
            {"attributes", {"email", email}
            }
        }
        }});


    std::string request_body = request_json.dump();

    cpr::Response r = cpr::Patch(cpr::Url(API_server_URL + fmt::format(API_PROPERTY_UPDATE_DELETE_PATH, API_user_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestDeleteUser(std::string API_user_UUID) {
    cpr::Response r = cpr::Delete(cpr::Url(API_server_URL + fmt::format(API_USER_DELETE_PATH, API_user_UUID)),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestUser(std::string API_user_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_USER_SINGLE_PATH, API_user_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestUserCollection(std::string email/*={}*/) {
    
    cpr::Parameters params = cpr::Parameters{};
    if(!email.empty()) {
        params.Add({"email", email});
    }

    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + API_USER_COLLECTION_PATH),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestUserTenantRelationship(std::string API_user_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_USER_TENANT_RELATIONSHIP_PATH, API_user_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestUserAssociatedTenant(std::string API_user_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_USER_ASSOCIATED_TENANT_PATH, API_user_UUID)),
                            cpr::Header{{"Content-Type", "application/vnd.api+json"}},
                            cpr::Bearer{API_token});

    return r;
}

// Tenants API endpoints
cpr::Response APIContext::requestTenant(std::string API_tenant_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_TENANT_SINGLE_PATH, API_tenant_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestTenantCollection() {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + API_TENANT_COLLECTION_PATH),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestTenantUsersRelationship(std::string API_tenant_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_TENANT_USERS_RELATIONSHIP_PATH, API_tenant_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestTenantAssociatedUsers(std::string API_tenant_UUID, int page_size/*=-1*/, std::string page_before/*={}*/, std::string page_after/*={}*/) {

    cpr::Parameters params = cpr::Parameters{};

    if(page_size>0) {
        params.Add({"page[size]", fmt::format("{}", page_size)});
    }
    if(!page_before.empty()) {
        params.Add({"page[before]", page_before});
    }
    if(!page_after.empty()) {
        params.Add({"page[after]", page_after});
    }

    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_TENANT_ASSOCIATED_USERS_PATH, API_tenant_UUID)),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestDeleteTenant(std::string API_tenant_UUID) {
    cpr::Response r = cpr::Delete(cpr::Url(API_server_URL + fmt::format(API_TENANT_UPDATE_DELETE_PATH, API_tenant_UUID)),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestCreateTenant(std::string name, std::string API_user_UUID) {

    std::string request_body = json::object({
        {"data", {
            {"type", "tenants"},{"attributes", {{"name", name}}},
            {"relationships", {{"user",{{"data", {{"type", "users"},{"id", API_user_UUID}}}}}}}
        }
        }}).dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_TENANT_CREATE_PATH)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestUpdateTenant(std::string API_tenant_UUID, std::string name) {

    std::string request_body = json({
        {"data", {
            {"type", "tenants"}, 
            {"id", API_tenant_UUID},
            {"attributes", {
                    {"name", name}
                }
            }
        }
        }}).dump();

    cpr::Response r = cpr::Patch(cpr::Url(API_server_URL + fmt::format(API_TENANT_UPDATE_DELETE_PATH, API_tenant_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}


cpr::Response APIContext::requestAssignTenantUsers(std::string API_tenant_UUID, std::vector<std::string> API_user_UUIDs) {

    json request_json = json({
        {"data", {
        }
        }}).dump();

    for(std::string uuid: API_user_UUIDs) {
        request_json["data"].push_back(json::object({
                        {"type", "users"},
                        {"id", uuid}
                        }));
    }

    std::string request_body = request_json.dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_TENANT_ASSIGN_REMOVE_USERS_PATH, API_tenant_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestRemoveTenantUsers(std::string API_tenant_UUID, std::vector<std::string> API_user_UUIDs) {

    json request_json = json({
        {"data", {
        }
        }}).dump();

    for(std::string uuid: API_user_UUIDs) {
        request_json["data"].push_back(json::object({
                        {"type", "users"},
                        {"id", uuid}
                        }));
    }

    std::string request_body = request_json.dump();

    cpr::Response r = cpr::Delete(cpr::Url(API_server_URL + fmt::format(API_TENANT_ASSIGN_REMOVE_USERS_PATH, API_tenant_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

// Properties API endpoints
cpr::Response APIContext::requestProperty(std::string API_tenant_UUID, std::string API_property_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_PROPERTY_SINGLE_PATH, API_tenant_UUID, API_property_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestPropertyCollection(std::string API_tenant_UUID, std::string name/*={}*/, std::string street/*={}*/, std::string postalCode/*={}*/, std::string city/*={}*/, std::string country/*={}*/, int page_size/*=-1*/, std::string page_before/*={}*/, std::string page_after/*={}*/) {

    cpr::Parameters params = cpr::Parameters{};
    if(!name.empty()) {
        params.Add({"name", name});
    }
    if(!street.empty()) {
        params.Add({"street", street});
    }
    if(!postalCode.empty()) {
        params.Add({"postalCode", postalCode});
    }
    if(!city.empty()) {
        params.Add({"city", city});
    }
    if(!country.empty()) {
        params.Add({"country", country});
    }
    if(page_size>0) {
        params.Add({"page[size]", fmt::format("{}", page_size)});
    }
    if(!page_before.empty()) {
        params.Add({"page[before]", page_before});
    }
    if(!page_after.empty()) {
        params.Add({"page[after]", page_after});
    }

    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_PROPERTY_COLLECTION_PATH, API_tenant_UUID)),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestPropertyConnectorsRelationship(std::string API_tenant_UUID, std::string API_property_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_PROPERTY_CONNECTORS_RELATIONSHIP_PATH, API_tenant_UUID, API_property_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestPropertyAssociatedConnectors(std::string API_tenant_UUID, std::string API_property_UUID, int page_size/*=-1*/, std::string page_before/*={}*/, std::string page_after/*={}*/) {
    
    cpr::Parameters params = cpr::Parameters{};

    if(page_size>0) {
        params.Add({"page[size]", fmt::format("{}", page_size)});
    }
    if(!page_before.empty()) {
        params.Add({"page[before]", page_before});
    }
    if(!page_after.empty()) {
        params.Add({"page[after]", page_after});
    }

    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_PROPERTY_ASSOCIATED_CONNECTORS_PATH, API_tenant_UUID, API_property_UUID)),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestCreateProperty(std::string API_tenant_UUID, std::string name, std::string street, std::string postalCode, std::string city, std::string country) {
    
    std::string request_body = json({
        {"data", {
            {"type", "properties"}, 
            {"attributes", {
                    {"name", name}, 
                    {"street", street}, 
                    {"postalCode", postalCode}, 
                    {"city", city}, 
                    {"country", country}, 
                }
            }
        }
        }}).dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_PROPERTY_CREATE_PATH, API_tenant_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestUpdateProperty(std::string API_tenant_UUID, std::string API_property_UUID, std::string name/*={}*/, std::string street/*={}*/, std::string postalCode/*={}*/, std::string city/*={}*/, std::string country/*={}*/) {
    
    json request_json = json({
        {"data", {
            {"type", "properties"}, 
            {"id", API_property_UUID},  
            {"attributes", {
                }
            }
        }
        }});

    if(!name.empty()) {
        request_json["data"]["attributes"].push_back(json::object_t::value_type("name", name));
    }
    if(!street.empty()) {
        request_json["data"]["attributes"].push_back(json::object_t::value_type("street", street));
    }
    if(!postalCode.empty()) {
        request_json["data"]["attributes"].push_back(json::object_t::value_type("postalCode", postalCode));
    }
    if(!city.empty()) {
        request_json["data"]["attributes"].push_back(json::object_t::value_type("city", city));
    }
    if(!country.empty()) {
        request_json["data"]["attributes"].push_back(json::object_t::value_type("country", country));
    }

    std::string request_body = request_json.dump();

    cpr::Response r = cpr::Patch(cpr::Url(API_server_URL + fmt::format(API_PROPERTY_UPDATE_DELETE_PATH, API_tenant_UUID, API_property_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestDeleteProperty(std::string API_tenant_UUID, std::string API_property_UUID) {

    cpr::Response r = cpr::Delete(cpr::Url(API_server_URL + fmt::format(API_PROPERTY_UPDATE_DELETE_PATH, API_tenant_UUID, API_property_UUID)),
                                cpr::Bearer{API_token});

    return r;
}

// Connectors API endpoints
cpr::Response APIContext::requestConnector(std::string API_tenant_UUID, std::string API_connector_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_CONNECTOR_SINGLE_PATH, API_tenant_UUID, API_connector_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestConnectorCollection(std::string API_tenant_UUID, int page_size/*=-1*/, std::string page_before/*={}*/, std::string page_after/*={}*/) {
        
    cpr::Parameters params = cpr::Parameters{};

    if(page_size>0) {
        params.Add({"page[size]", fmt::format("{}", page_size)});
    }
    if(!page_before.empty()) {
        params.Add({"page[before]", page_before});
    }
    if(!page_after.empty()) {
        params.Add({"page[after]", page_after});
    }

    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_CONNECTOR_COLLECTION_PATH, API_tenant_UUID)),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestConnectorPropertyRelationship(std::string API_tenant_UUID, std::string API_connector_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_CONNECTOR_PROPERTY_RELATIONSHIP_PATH, API_tenant_UUID, API_connector_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestConnectorAssociatedProperty(std::string API_tenant_UUID, std::string API_connector_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_CONNECTOR_ASSOCIATED_PROPERTY_PATH, API_tenant_UUID, API_connector_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestConnectorDevicesRelationship(std::string API_tenant_UUID, std::string API_connector_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_CONNECTOR_DEVICES_RELATIONSHIP_PATH, API_tenant_UUID, API_connector_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestConnectorAssociatedDevices(std::string API_tenant_UUID, std::string API_connector_UUID, int page_size/*=-1*/, std::string page_before/*={}*/, std::string page_after/*={}*/) {
            
    cpr::Parameters params = cpr::Parameters{};

    if(page_size>0) {
        params.Add({"page[size]", fmt::format("{}", page_size)});
    }
    if(!page_before.empty()) {
        params.Add({"page[before]", page_before});
    }
    if(!page_after.empty()) {
        params.Add({"page[after]", page_after});
    }

    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_CONNECTOR_ASSOCIATED_DEVICES_PATH, API_tenant_UUID, API_connector_UUID)),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestCreateConnector(std::string API_tenant_UUID, std::string API_property_UUID, std::string name) {
    
    std::string request_body = json::object({
        {"data", {
            {"type", "connectors"},{"attributes", {{"name", name}}},
            {"relationships", {{"property",{{"data", {{"type", "properties"},{"id", API_property_UUID}}}}}}}
        }
        }}).dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_CONNECTOR_CREATE_PATH, API_tenant_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestUpdateConnector(std::string API_tenant_UUID, std::string API_connector_UUID, std::string name/*={}*/) {
    
    json request_json = json({
        {"data", {
            {"id", API_connector_UUID}, 
            {"type", "connectors"}, 
            {"attributes", {
            }
            }
        }
        }});

    if(!name.empty()) {
        request_json["data"]["attributes"].push_back(json::object_t::value_type("name", name));
    }

    std::string request_body = request_json.dump();

    cpr::Response r = cpr::Patch(cpr::Url(API_server_URL + fmt::format(API_CONNECTOR_UPDATE_DELETE_PATH, API_tenant_UUID, API_connector_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestDeleteConnector(std::string API_tenant_UUID, std::string API_connector_UUID) {
    cpr::Response r = cpr::Delete(cpr::Url(API_server_URL + fmt::format(API_CONNECTOR_UPDATE_DELETE_PATH, API_tenant_UUID, API_connector_UUID)),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestConnectorToken(std::string API_tenant_UUID, std::string API_connector_UUID) {
    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_CONNECTOR_GENERATE_TOKEN_PATH, API_tenant_UUID, API_connector_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"}},
                                cpr::Bearer{API_token});

    return r;
}

// Devices API endpoints
cpr::Response APIContext::requestDevice(std::string API_tenant_UUID, std::string API_device_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_DEVICE_SINGLE_PATH, API_tenant_UUID, API_device_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestDeviceCollection(std::string API_tenant_UUID, std::string aksID/*={}*/, std::string description/*={}*/, std::string unit/*={}*/, int page_size/*=-1*/, std::string page_before/*={}*/, std::string page_after/*={}*/) {

    cpr::Parameters params = cpr::Parameters{};
    if(!aksID.empty()) {
        params.Add({"aksID", aksID});
    }
    if(!description.empty()) {
        params.Add({"description", description});
    }
    if(!unit.empty()) {
        params.Add({"unit", unit});
    }
    if(page_size>0) {
        params.Add({"page[size]", fmt::format("{}", page_size)});
    }
    if(!page_before.empty()) {
        params.Add({"page[before]", page_before});
    }
    if(!page_after.empty()) {
        params.Add({"page[after]", page_after});
    }

    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_DEVICE_COLLECTION_PATH, API_tenant_UUID)),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestDeviceConnectorRelationship(std::string API_tenant_UUID, std::string API_device_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_DEVICE_CONNECTOR_RELATIONSHIP_PATH, API_tenant_UUID, API_device_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestDeviceAssociatedConnector(std::string API_tenant_UUID, std::string API_device_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_DEVICE_ASSOCIATED_CONNECTOR_PATH, API_tenant_UUID, API_device_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestDeviceReadingsRelationship(std::string API_tenant_UUID, std::string API_device_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_DEVICE_READINGS_RELATIONSHIP_PATH, API_tenant_UUID, API_device_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestDeviceAssociatedReadings(std::string API_tenant_UUID, std::string API_device_UUID, int page_size/*=-1*/, std::string page_before/*={}*/, std::string page_after/*={}*/) {
    
    cpr::Parameters params = cpr::Parameters{};

    if(page_size>0) {
        params.Add({"page[size]", fmt::format("{}", page_size)});
    }
    if(!page_before.empty()) {
        params.Add({"page[before]", page_before});
    }
    if(!page_after.empty()) {
        params.Add({"page[after]", page_after});
    }
    
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_DEVICE_ASSOCIATED_READINGS_PATH, API_tenant_UUID, API_device_UUID)),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestDeviceSetPointsRelationship(std::string API_tenant_UUID, std::string API_device_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_DEVICE_SETPOINTS_RELATIONSHIP_PATH, API_tenant_UUID, API_device_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestDeviceAssociatedSetPoints(std::string API_tenant_UUID, std::string API_device_UUID, int page_size/*=-1*/, std::string page_before/*={}*/, std::string page_after/*={}*/) {
        
    cpr::Parameters params = cpr::Parameters{};

    if(page_size>0) {
        params.Add({"page[size]", fmt::format("{}", page_size)});
    }
    if(!page_before.empty()) {
        params.Add({"page[before]", page_before});
    }
    if(!page_after.empty()) {
        params.Add({"page[after]", page_after});
    }

    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_DEVICE_ASSOCIATED_SETPOINTS_PATH, API_tenant_UUID, API_device_UUID)),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestCreateDevice(std::string API_tenant_UUID, std::string API_connector_UUID, std::string aksID, std::string description, std::string unit) {
    
    std::string request_body = json::object({
        {"data", {
            {"type", "devices"},{"attributes", {{"aksId", aksID},{"description", description},{"unit", unit}}},
            {"relationships", {{"connector",{{"data", {{"type", "connectors"},{"id", API_connector_UUID}}}}}}}
        }
        }}).dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_DEVICE_CREATE_PATH, API_tenant_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestUpdateDevice(std::string API_tenant_UUID, std::string API_device_UUID, std::string aksID/*={}*/, std::string description/*={}*/, std::string unit/*={}*/) {
    
    json request_json = json({
        {"data", {
            {"id", API_device_UUID},
            {"type", "devices"}, 
            {"attributes", {
            }
            }
        }
        }});

    if(!aksID.empty()) {
        request_json["data"]["attributes"].push_back(json::object_t::value_type("aksId", aksID));
    }
    if(!description.empty()) {
        request_json["data"]["attributes"].push_back(json::object_t::value_type("description", description));
    }
    if(!unit.empty()) {
        request_json["data"]["attributes"].push_back(json::object_t::value_type("unit", unit));
    }

    std::string request_body = request_json.dump();

    cpr::Response r = cpr::Patch(cpr::Url(API_server_URL + fmt::format(API_DEVICE_UPDATE_DELETE_PATH, API_tenant_UUID, API_device_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestDeleteDevice(std::string API_tenant_UUID, std::string API_device_UUID) {
    cpr::Response r = cpr::Delete(cpr::Url(API_server_URL + fmt::format(API_DEVICE_UPDATE_DELETE_PATH, API_tenant_UUID, API_device_UUID)),
                            cpr::Bearer{API_token});

    return r;
}


// Readings API endpoints
cpr::Response APIContext::requestReading(std::string API_tenant_UUID, std::string API_reading_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_READING_SINGLE_PATH, API_tenant_UUID, API_reading_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestReadingCollection(std::string API_tenant_UUID, std::time_t from/*=-1*/, std::time_t until/*=-1*/, std::time_t timestamp/*=-1*/, double value/*=std::numeric_limits<double>::quiet_NaN()*/, std::string API_device_UUID/*={}*/, int page_size/*=-1*/, std::string page_before/*={}*/, std::string page_after/*={}*/) {
    
    cpr::Parameters params = cpr::Parameters{};
    if(from >= 0) {
        params.Add({"from", fmt::format("{:%FT%T.000Z}", fmt::localtime(from))}); // TODO date format
    }
    if(until >= 0) {
        params.Add({"until", fmt::format("{:%FT%T.000Z}", fmt::localtime(until))});
    }
    if(timestamp >= 0) {
        params.Add({"timestamp", fmt::format("{:%FT%T.000Z}", fmt::localtime(timestamp))});
    }
    if(!std::isnan(value)) {
        params.Add({"value", fmt::format("{}", value)});
    }
    if(!API_device_UUID.empty()) {
        params.Add({"deviceId", API_device_UUID});
    }
    if(page_size>0) {
        params.Add({"page[size]", fmt::format("{}", page_size)});
    }
    if(!page_before.empty()) {
        params.Add({"page[before]", page_before});
    }
    if(!page_after.empty()) {
        params.Add({"page[after]", page_after});
    }

    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_READING_COLLECTION_PATH, API_tenant_UUID)),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestReadingDeviceRelationship(std::string API_tenant_UUID, std::string API_reading_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_READING_DEVICE_RELATIONSHIP_PATH, API_tenant_UUID, API_reading_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestReadingAssociatedDevice(std::string API_tenant_UUID, std::string API_reading_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_READING_ASSOCIATED_DEVICE_PATH, API_tenant_UUID, API_reading_UUID)),
                            cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestCreateReading(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp) {

    std::string request_body = json::object({
        {"data", {
            {"type", "readings"},{"attributes", {{"value", value}, {"timestamp", fmt::format("{:%FT%T.000Z}", fmt::localtime(timestamp))}}},
            {"relationships", {{"device",{{"data", {{"type", "devices"},{"id", API_device_UUID}}}}}}}
        }
        }}).dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_READING_CREATE_PATH, API_tenant_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}

cpr::Response APIContext::requestDeleteReading(std::string API_tenant_UUID, std::string API_reading_UUID) {
    cpr::Response r = cpr::Delete(cpr::Url(API_server_URL + fmt::format(API_READING_DELETE_PATH, API_tenant_UUID, API_reading_UUID)),
                            cpr::Bearer{API_token});

    return r;
}


// SetPoints API endpoints
cpr::Response APIContext::requestSetPoint(std::string API_tenant_UUID, std::string API_setpoint_UUID) {
    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_SETPOINT_SINGLE_PATH, API_tenant_UUID, API_setpoint_UUID)),
                            cpr::Bearer{API_token});

    return r;    
}

cpr::Response APIContext::requestSetPointCollection(std::string API_tenant_UUID, std::time_t from/*=-1*/, std::time_t until/*=-1*/, std::time_t timestamp/*=-1*/, std::time_t currentTime/*=-1*/, std::string API_device_UUID/*={}*/, int page_size/*=-1*/, std::string page_before/*={}*/, std::string page_after/*={}*/) {
    
    cpr::Parameters params = cpr::Parameters{};
    if(from >= 0) {
        params.Add({"from", fmt::format("{:%FT%T.000Z}", fmt::localtime(from))}); // TODO date format
    }
    if(until >= 0) {
        params.Add({"until", fmt::format("{:%FT%T.000Z}", fmt::localtime(until))});
    }
    if(timestamp >= 0) {
        params.Add({"timestamp", fmt::format("{:%FT%T.000Z}", fmt::localtime(timestamp))});
    }
    if(currentTime >= 0) {
        params.Add({"currentTime", fmt::format("{:%FT%T.000Z}", fmt::localtime(currentTime))});
    }
    if(!API_device_UUID.empty()) {
        params.Add({"deviceId", API_device_UUID});
    }
    if(page_size>0) {
        params.Add({"page[size]", fmt::format("{}", page_size)});
    }
    if(!page_before.empty()) {
        params.Add({"page[before]", page_before});
    }
    if(!page_after.empty()) {
        params.Add({"page[after]", page_after});
    }

    cpr::Response r = cpr::Get(cpr::Url(API_server_URL + fmt::format(API_SETPOINT_COLLECTION_PATH, API_tenant_UUID)),
                            cpr::Bearer{API_token},
                            params);

    return r;
}

cpr::Response APIContext::requestCreateSetPoint(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp) {
    
    std::string request_body = json::object({
        {"data", {
            {"type", "setpoints"},{"attributes", {{"value", value}, {"timestamp", fmt::format("{:%FT%T.000Z}", fmt::localtime(timestamp))}}},
            {"relationships", {{"device",{{"data", {{"type", "devices"},{"id", API_device_UUID}}}}}}}
        }
        }}).dump();

    cpr::Response r = cpr::Post(cpr::Url(API_server_URL + fmt::format(API_SETPOINT_CREATE_PATH, API_tenant_UUID)),
                                cpr::Header{{"Content-Type", "application/vnd.api+json"},
                                            {"Content-Length", std::to_string(request_body.length())}},
                                cpr::Body(request_body),
                                cpr::Bearer{API_token});

    return r;
}



}