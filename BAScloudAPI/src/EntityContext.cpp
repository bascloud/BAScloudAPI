#include <chrono>
#include <algorithm>
#include <utility>
#include <string>
#include <iostream>

#include <fmt/core.h>

#include "EntityContext.h"
#include "error/Exceptions.h"
#include "Util.h"


namespace BAScloud {

EntityContext::EntityContext(std::string API_server_URL) : 
    api_context(APIContext(API_server_URL)), API_server_URL(API_server_URL), API_token(""), API_token_valid_until(-1) {
    
}

json EntityContext::parseResponse(cpr::Response response) {
    
    if(response.error.code != cpr::ErrorCode::OK) {
        // Throw connection exception when request through cpr failed
        throw ConnectionError(response.error.message);
    }

    json json_response;

    if(!response.text.empty()) {
        try {
            json_response = json::parse(response.text);
        } catch (...) {
            if(response.text.find("Server Error") != std::string::npos) {
                // May be possible the requests returns a server error, which is HTML, catch this here
                throw ServerError("Server Error response received from the BAScloud API. Contact support if the error continues to occur.");
            } else {
                // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
                std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response contained invalid JSON."));
            }

        }
    } else {
        json_response = {};
    }

    if(response.error.code == cpr::ErrorCode::OK && (cpr::status::is_client_error(response.status_code) || cpr::status::is_server_error(response.status_code))) {
        // Throw predefined exceptions for error codes returned by the api
        switch(response.status_code) {
            case 400: 
                throw BadRequest(fmt::format("400 Bad Request error response received from BAScloud API. {} {}", json_response["errors"][0]["title"], json_response["errors"][0]["detail"]));
            case 401: 
                throw UnauthorizedRequest(fmt::format("401 Unauthorized error response received from BAScloud API. {} {}", json_response["errors"][0]["title"], json_response["errors"][0]["detail"]));
            case 403: 
                throw ForbiddenRequest(fmt::format("409 Forbidden error response received from BAScloud API. {} {}", json_response["errors"][0]["title"], json_response["errors"][0]["detail"]));
            case 404: 
                throw NotFoundRequest(fmt::format("404 Not Found error response received from BAScloud API. {} {}", json_response["errors"][0]["title"], json_response["errors"][0]["detail"]));
            case 409: 
                throw ConflictRequest(fmt::format("409 Conflict error response received from BAScloud API. {} {}", json_response["errors"][0]["title"], json_response["errors"][0]["detail"]));
            case 500: 
                throw ServerError(fmt::format("500 Internal Server Error response received from BAScloud API. {} {}", json_response["errors"][0]["title"], json_response["errors"][0]["detail"]));
            default: throw std::runtime_error(fmt::format("Unexpected HTTP error response received from the BAScloud API. Error: {} {}", response.status_code, HTTPCodeToPhrase(response.status_code)));
        }
    } else {
        return json_response;
    }
}

PagingResult EntityContext::parsePaging(json response) {

    PagingResult pres;

    try {
        pres.currentPage = response["meta"]["page"]["page"];
        pres.pageSize = response["meta"]["page"]["pageSize"];
        pres.totalPages = response["meta"]["page"]["totalPages"];
        pres.count = response["meta"]["page"]["count"];

        if(response.contains("links")) {
            std::string next_link = response["links"].at("next").is_null() ? "" : response["links"].value("next", "");
            std::string prev_link = response["links"].at("prev").is_null() ? "" : response["links"].value("prev", "");

            pres.nextPagePointer = next_link.empty() ? "" : BAScloud::Util::parseURLParameter(next_link).at("page[after]"); 
            pres.previousPagePointer =  prev_link.empty() ? "" : BAScloud::Util::parseURLParameter(prev_link).at("page[before]"); 
        } else {
            pres.nextPagePointer = "";
            pres.previousPagePointer = "";        
        }
    } catch (...) {
        // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
        std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected paging data."));
    }

    return pres;
}

void EntityContext::validateUUID(std::string UUID) {
    // TODO ML: Too narrow? always valid for our UUIDs?
    if(UUID.length()!=36) {
        throw std::invalid_argument(fmt::format("Invalid argument. UUID incorrect format or length. Provided UUID: \"{}\"", UUID));
    }
}

void EntityContext::validateRole(std::string role) {
    if(role != "admin" && role != "user" && role != "connector" && role != "superadmin") {
        throw std::invalid_argument(fmt::format("Invalid argument. Role not supported. Provided Role: \"{}\"", role));
    }
}

void EntityContext::checkAndRenewAuthentication() {
    
    if(isAuthenticated()) {
        return;
    } else {
        if(!API_login_email.empty() && !API_login_password.empty()) {
            authenticateWithUserLogin(API_login_email, API_login_password);
        } else {
            throw UnauthorizedRequest("No authentication login data available for the context. Call authenticateWithUserLogin() providing valid user login data.");
        }
    }

}

std::string EntityContext::getToken() {
    return API_token;
}

std::time_t EntityContext::getTokenExpirationDate() {
    return API_token_valid_until;
}

bool EntityContext::isAuthenticated() {
    std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    return !API_token.empty() && API_token_valid_until > currentDateTime;
}

void EntityContext::authenticateWithUserLogin(std::string API_email, std::string API_password) {

    cpr::Response r = api_context.requestAuthenticationLogin(API_email, API_password);

    json respond = parseResponse(r);

    if(respond["data"]["type"] == "accesstoken") {
        try {
            API_token = respond["data"]["attributes"].value("token", "");
            uint64_t timestamp = respond["data"]["attributes"]["expires"];
            // ML: expiration timestamp is returned in milliseconds, to have same unit in whole library, convert to seconds
            API_token_valid_until = timestamp / 1000;
            API_login_email = API_email;
            API_login_password = API_password;

            api_context.setToken(API_token);
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no accesstoken is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain an accesstoken.");
    }
}

void EntityContext::authenticateWithConnectorToken(std::string API_connector_token) {
    
    if(API_connector_token.empty()) {
        throw std::invalid_argument("Invalid Connector token: Token is empty.");
    }

    API_login_email = "";
    API_login_password = "";
    
    API_token = API_connector_token;
    api_context.setToken(API_token);
    
    struct tm never = {};
    never.tm_year=10000;
    API_token_valid_until = mktime(&never);
}


User EntityContext::createNewUser(std::string email, std::string password) {
 
    cpr::Response r = api_context.requestUserSignup(email, password);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "users") {
        try {
            time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            User user(respond["data"]["id"], respond["data"]["attributes"]["email"], currentDateTime, currentDateTime, this);

            return user;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no user is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain users data.");
    }
}

void EntityContext::requestUserPasswordReset(std::string email) {

    cpr::Response r = api_context.requestUserPasswordReset(email);
    
    json respond = parseResponse(r);

    // There is no relevant information in the response, if no HTTP errors occured, request was successfull
    // Wherever the request was valid i.e. correct email can not be distinguished by the response for security reasons
}

void EntityContext::updateUserPassword(std::string API_user_UUID, std::string reset_token, std::string new_password) {

    validateUUID(API_user_UUID);

    cpr::Response r = api_context.requestUserPasswordChange(API_user_UUID, reset_token, new_password);
    
    json respond = parseResponse(r);

    // There is no relevant information in the response, if no HTTP errors occured, request was successfull (201 no content)
    // Wherever the request was valid i.e. correct email can not be distinguished by the response for security reasons
}

User EntityContext::updateUser(std::string API_user_UUID, std::string email/*={}*/, std::string API_tenant_UUID/*={}*/, std::string role/*={}*/) {

    validateUUID(API_user_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestUpdateUser(API_user_UUID, email, API_tenant_UUID, role);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "users") {
        try {
            User user(respond["data"]["id"], respond["data"]["attributes"]["email"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return user;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no user is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain users data.");
    }
}

void EntityContext::deleteUser(std::string API_user_UUID) {

    validateUUID(API_user_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestDeleteUser(API_user_UUID);
    
    json respond = parseResponse(r);

    // OK No Content
}


User EntityContext::getUser(std::string API_user_UUID) {

    validateUUID(API_user_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestUser(API_user_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "users") {
        try {
            User user(respond["data"]["id"], respond["data"]["attributes"]["email"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return user;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no user is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

EntityCollection<User> EntityContext::getUsersCollection(PagingOption paging/*={}*/, std::string email/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/, 
    std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {

    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestUserCollection(email, createdFrom, createdUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestUserCollection(email, createdFrom, createdUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestUserCollection(email, createdFrom, createdUntil, paging.page_size);
        break;
        default:
            r = api_context.requestUserCollection(email, createdFrom, createdUntil);
    }

    json respond = parseResponse(r);

    std::vector<User> users;
    for(json json_user: respond["data"]) {
        if(json_user["type"] == "users") {
            try {
                User user(json_user["id"], json_user["attributes"]["email"], Util::parseDateTimeString(json_user["meta"]["createdAt"]), Util::parseDateTimeString(json_user["meta"]["updatedAt"]), this);

                users.push_back(user);
            } catch (std::exception& e) {
                errorHandler(e, json_user);
            }
        }
    }

    return std::make_pair(users, PagingResult{});
}

Tenant EntityContext::getAssociatedTenant(std::string API_user_UUID) {
    
    validateUUID(API_user_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestUserAssociatedTenant(API_user_UUID);

    json respond = parseResponse(r);

    if(respond["data"]["type"] == "tenants") {
        try {
            Tenant tenant(respond["data"]["id"], respond["data"]["attributes"]["name"], respond["data"]["attributes"]["urlName"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return tenant;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no tenant is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain tenant data.");
    }
}

PermissionData EntityContext::getUserPermissions(std::string API_user_UUID) {

    validateUUID(API_user_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestUserPermissions(API_user_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "permissions") {
        try {
            PermissionData perm;
            perm.role = respond["data"]["attributes"]["role"];

            for(auto it: respond["data"]["attributes"]["resources"].items()) {
                std::cout << it.key() << " | " << it.value() << "\n";
                std::vector<std::string> resources = {};
                for (auto it : it.value()) {
                    resources.push_back(it);
                }
                perm.resources[it.key()] = resources;
            }

            return perm;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no tenant is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain permissions data.");
    }
}

Tenant EntityContext::getTenant(std::string API_tenant_UUID) {

    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestTenant(API_tenant_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "tenants") {
        try {
            Tenant tenant(respond["data"]["id"], respond["data"]["attributes"]["name"], respond["data"]["attributes"]["urlName"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return tenant;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no tenant is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

EntityCollection<Tenant> EntityContext::getTenantsCollection(PagingOption paging/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/, std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {

    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestTenantCollection(createdFrom, createdUntil);

    json respond = parseResponse(r);

    std::vector<Tenant> tenants;
    for(json json_tenant: respond["data"]) {
        if(json_tenant["type"] == "tenants") {
            try {
                Tenant tenant(json_tenant["id"], json_tenant["attributes"]["name"], json_tenant["attributes"]["urlName"], Util::parseDateTimeString(json_tenant["meta"]["createdAt"]), Util::parseDateTimeString(json_tenant["meta"]["updatedAt"]), this);

                tenants.push_back(tenant);
            } catch (std::exception& e) {
                errorHandler(e, json_tenant);
            }
        } 
    }

    return std::make_pair(tenants, PagingResult{});
}

EntityCollection<User> EntityContext::getAssociatedUsers(std::string API_tenant_UUID, PagingOption paging/*={}*/, std::string email/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/, std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {

    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestTenantAssociatedUsers(API_tenant_UUID, email, createdFrom, createdUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestTenantAssociatedUsers(API_tenant_UUID, email, createdFrom, createdUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestTenantAssociatedUsers(API_tenant_UUID, email, createdFrom, createdUntil, paging.page_size);
        break;
        default:
            r = api_context.requestTenantAssociatedUsers(API_tenant_UUID);
    }

    json respond = parseResponse(r);

    std::vector<User> users;
    for(json json_user: respond["data"]) {
        if(json_user["type"] == "users") {
            try {
                User user(json_user["id"], json_user["attributes"]["email"], Util::parseDateTimeString(json_user["meta"]["createdAt"]), Util::parseDateTimeString(json_user["meta"]["updatedAt"]), this);

                users.push_back(user);
            } catch (std::exception& e) {
                errorHandler(e, json_user);
            }
        } 
    }

    return std::make_pair(users, parsePaging(respond));
}

Tenant EntityContext::createTenant(std::string name, std::string API_user_UUID) {
    
    validateUUID(API_user_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestCreateTenant(name, API_user_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "tenants") {
        try {
            Tenant tenant(respond["data"]["id"], respond["data"]["attributes"]["name"], respond["data"]["attributes"]["urlName"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return tenant;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no tenant is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

void EntityContext::deleteTenant(std::string API_tenant_UUID) {
    
    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestDeleteTenant(API_tenant_UUID);
    
    json respond = parseResponse(r);

    // OK response is empty
}

Tenant EntityContext::updateTenant(std::string API_tenant_UUID, std::string name/*={}*/) {
    
    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestUpdateTenant(API_tenant_UUID, name);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "tenants") {
        try {
            Tenant tenant(respond["data"]["id"], respond["data"]["attributes"]["name"], respond["data"]["attributes"]["urlName"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return tenant;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no tenant is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}


void EntityContext::assignTenantUsers(std::string API_tenant_UUID, std::vector<std::string> API_user_UUIDs, std::vector<std::string> API_user_ROLES) {
    
    if(API_user_UUIDs.size() != API_user_ROLES.size()) {
        throw std::invalid_argument("Invalid argument. User UUIDs and Roles lists must be the same length.");
    }

    validateUUID(API_tenant_UUID);
    for(std::string uuid: API_user_UUIDs) {
        validateUUID(uuid);
    }
    for(std::string role: API_user_ROLES) {
        validateRole(role);
    }
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestAssignTenantUsers(API_tenant_UUID, API_user_UUIDs, API_user_ROLES);
    
    json respond = parseResponse(r);

    // OK response is empty
}

void EntityContext::removeTenantUsers(std::string API_tenant_UUID, std::vector<std::string> API_user_UUIDs) {
    
    validateUUID(API_tenant_UUID);
    for(std::string uuid: API_user_UUIDs) {
        validateUUID(uuid);
    }
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestRemoveTenantUsers(API_tenant_UUID, API_user_UUIDs);
    
    json respond = parseResponse(r);

    // OK response is empty
}


Property EntityContext::getProperty(std::string API_tenant_UUID, std::string API_property_UUID) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_property_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestProperty(API_tenant_UUID, API_property_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "properties") {
        try {
            // aksId and identifier may be null in response
            if(respond["data"]["attributes"]["aksId"].is_null())
                respond["data"]["attributes"]["aksId"] = "";
            
            if(respond["data"]["attributes"]["identifier"].is_null())
                respond["data"]["attributes"]["identifier"] = "";

            Property property(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["name"], respond["data"]["attributes"].value("aksId", ""), respond["data"]["attributes"].value("identifier", ""), respond["data"]["attributes"]["street"], respond["data"]["attributes"]["postalCode"], respond["data"]["attributes"]["city"], respond["data"]["attributes"]["country"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return property;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no accesstoken is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

EntityCollection<Property> EntityContext::getPropertiesCollection(std::string API_tenant_UUID, PagingOption paging/*={}*/, std::string name/*={}*/, std::string aksID/*={}*/, std::string identifier/*={}*/, std::string street/*={}*/, std::string postalCode/*={}*/, std::string city/*={}*/, std::string country/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/,
     std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {

    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestPropertyCollection(API_tenant_UUID, name, aksID, identifier, street, postalCode, city, country, createdFrom, createdUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestPropertyCollection(API_tenant_UUID, name, aksID, identifier, street, postalCode, city, country, createdFrom, createdUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestPropertyCollection(API_tenant_UUID, name, aksID, identifier, street, postalCode, city, country, createdFrom, createdUntil, paging.page_size);
        break;
        default:
            r = api_context.requestPropertyCollection(API_tenant_UUID, name, aksID, identifier, street, postalCode, city, country, createdFrom, createdUntil);
    }

    json respond = parseResponse(r);

    std::vector<Property> properties;
    for(json json_prop: respond["data"]) {
        if(json_prop["type"] == "properties") {
            try {
                // aksId and identifier may be null in response
                std::string aksId = "";
                if(!json_prop["data"]["attributes"]["aksId"].is_null())
                    aksId = json_prop["data"]["attributes"]["aksId"];
                
                std::string identifier = "";
                if(!json_prop["data"]["attributes"]["identifier"].is_null())
                    identifier = json_prop["data"]["attributes"]["identifier"];

                Property property(json_prop["id"], API_tenant_UUID, json_prop["attributes"]["name"], aksId, identifier, json_prop["attributes"]["street"], json_prop["attributes"]["postalCode"], json_prop["attributes"]["city"], json_prop["attributes"]["country"], Util::parseDateTimeString(json_prop["meta"]["createdAt"]), Util::parseDateTimeString(json_prop["meta"]["updatedAt"]), this);

                properties.push_back(property);
            } catch (std::exception& e) {
                errorHandler(e, json_prop);
            }
        } 
    }

    return std::make_pair(properties, parsePaging(respond));
}

EntityCollection<Connector> EntityContext::getAssociatedConnectors(std::string API_tenant_UUID, std::string API_property_UUID, PagingOption paging/*={}*/, 
    std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_property_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestPropertyAssociatedConnectors(API_tenant_UUID, API_property_UUID, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestPropertyAssociatedConnectors(API_tenant_UUID, API_property_UUID, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestPropertyAssociatedConnectors(API_tenant_UUID, API_property_UUID, paging.page_size);
        break;
        default:
            r = api_context.requestPropertyAssociatedConnectors(API_tenant_UUID, API_property_UUID);
    }

    json respond = parseResponse(r);

    std::vector<Connector> connectors;
    for(json json_conn: respond["data"]) {
        if(json_conn["type"] == "connectors") {
            try {
                Connector connector(json_conn["id"], API_tenant_UUID, json_conn["attributes"]["name"], json_conn["attributes"].value("apiKey", ""), Util::parseDateTimeString(json_conn["meta"]["createdAt"]), Util::parseDateTimeString(json_conn["meta"]["updatedAt"]), this);

                connectors.push_back(connector);
            } catch (std::exception& e) {
                errorHandler(e, json_conn);
            }
        } 
    }

    return std::make_pair(connectors, parsePaging(respond));
}

EntityCollection<Device> EntityContext::getAssociatedPropertyDevices(std::string API_tenant_UUID, std::string API_property_UUID, PagingOption paging/*={}*/, std::string aksID/*={}*/, std::string localAksID/*={}*/, std::string API_connector_UUID/*={}*/, std::string description/*={}*/, std::string unit/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/, std::time_t deletedUntil/*=-1*/,
    std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {
    
    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestDeviceCollection(API_tenant_UUID, aksID, localAksID, API_connector_UUID, API_property_UUID, description, unit, createdFrom, createdUntil, deletedUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestDeviceCollection(API_tenant_UUID, aksID, localAksID, API_connector_UUID, API_property_UUID, description, unit, createdFrom, createdUntil, deletedUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestDeviceCollection(API_tenant_UUID, aksID, localAksID, API_connector_UUID, API_property_UUID, description, unit, createdFrom, createdUntil, deletedUntil, paging.page_size);
        break;
        default:
            r = api_context.requestDeviceCollection(API_tenant_UUID, aksID, localAksID, API_connector_UUID, API_property_UUID, description, unit, createdFrom, createdUntil, deletedUntil);
    }

    json respond = parseResponse(r);

    std::vector<Device> devices;
    for(json json_dev: respond["data"]) {
        if(json_dev["type"] == "devices") {
            try {
                Device device(json_dev["id"], API_tenant_UUID, json_dev["attributes"]["aksId"], json_dev["attributes"]["localAksId"], json_dev["attributes"]["description"], json_dev["attributes"]["unit"], Util::parseDateTimeString(json_dev["meta"]["createdAt"]), Util::parseDateTimeString(json_dev["meta"]["updatedAt"]), this);

                devices.push_back(device);
            } catch (std::exception& e) {
                errorHandler(e, json_dev);
            }
        } 
    }

    return std::make_pair(devices, parsePaging(respond));
}

Property EntityContext::createProperty(std::string API_tenant_UUID, std::string name, std::string aksID/*={}*/, std::string identifier/*={}*/, std::string street/*={}*/, std::string postalCode/*={}*/, std::string city/*={}*/, std::string country/*={}*/) {

    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestCreateProperty(API_tenant_UUID, name, aksID, identifier, street, postalCode, city, country);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "properties") {
        try {
            // aksId and identifier may be null in response
            if(respond["data"]["attributes"]["aksId"].is_null())
                respond["data"]["attributes"]["aksId"] = "";
            
            if(respond["data"]["attributes"]["identifier"].is_null())
                respond["data"]["attributes"]["identifier"] = "";

            Property prop(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["name"], respond["data"]["attributes"]["aksId"], respond["data"]["attributes"]["identifier"], respond["data"]["attributes"]["street"], respond["data"]["attributes"]["postalCode"], respond["data"]["attributes"]["city"], respond["data"]["attributes"]["country"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return prop;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no accesstoken is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

void EntityContext::deleteProperty(std::string API_tenant_UUID, std::string API_property_UUID) {

    cpr::Response r = api_context.requestDeleteProperty(API_tenant_UUID, API_property_UUID);
    
    json respond = parseResponse(r);

    // Successfull delete returns 204 No Content
}

Property EntityContext::updateProperty(std::string API_tenant_UUID, std::string API_property_UUID, std::string name/*={}*/, std::string aksID/*={}*/, std::string identifier/*={}*/, std::string street/*={}*/, std::string postalCode/*={}*/, std::string city/*={}*/, std::string country/*={}*/) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_property_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestUpdateProperty(API_tenant_UUID, API_property_UUID, name, street, postalCode, city, country);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "properties") {
        try {
            // aksId and identifier may be null in response
            if(respond["data"]["attributes"]["aksId"].is_null())
                respond["data"]["attributes"]["aksId"] = "";
            
            if(respond["data"]["attributes"]["identifier"].is_null())
                respond["data"]["attributes"]["identifier"] = "";
                
            Property prop(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["name"], respond["data"]["attributes"]["aksId"], respond["data"]["attributes"]["identifier"], respond["data"]["attributes"]["street"], respond["data"]["attributes"]["postalCode"], respond["data"]["attributes"]["city"], respond["data"]["attributes"]["country"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return prop;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no accesstoken is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}


Connector EntityContext::getConnector(std::string API_tenant_UUID, std::string API_connector_UUID) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_connector_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestConnector(API_tenant_UUID, API_connector_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "connectors") {
        try {
            Connector conn(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["name"], "", Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return conn;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no accesstoken is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

EntityCollection<Connector> EntityContext::getConnectorsCollection(std::string API_tenant_UUID, PagingOption paging/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/, 
    std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {

    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestConnectorCollection(API_tenant_UUID, createdFrom, createdUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestConnectorCollection(API_tenant_UUID, createdFrom, createdUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestConnectorCollection(API_tenant_UUID, createdFrom, createdUntil, paging.page_size);
        break;
        default:
            r = api_context.requestConnectorCollection(API_tenant_UUID, createdFrom, createdUntil);
    }

    json respond = parseResponse(r);

    std::vector<Connector> connectors;
    for(json json_conn: respond["data"]) {
        if(json_conn["type"] == "connectors") {
            try {
                Connector conn(json_conn["id"], API_tenant_UUID, json_conn["attributes"]["name"], "", Util::parseDateTimeString(json_conn["meta"]["createdAt"]), Util::parseDateTimeString(json_conn["meta"]["updatedAt"]), this);

                connectors.push_back(conn);
            } catch (std::exception& e) {
                errorHandler(e, json_conn);
            }
        } 
    }

    return std::make_pair(connectors, parsePaging(respond));
}

Property EntityContext::getAssociatedProperty(std::string API_tenant_UUID, std::string API_connector_UUID) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_connector_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestConnectorAssociatedProperty(API_tenant_UUID, API_connector_UUID);

    json respond = parseResponse(r);

    if(respond["data"]["type"] == "properties") {
        try {
            Property prop(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["name"], respond["data"]["attributes"]["aksId"], respond["data"]["attributes"]["identifier"], respond["data"]["attributes"]["street"], respond["data"]["attributes"]["postalCode"], respond["data"]["attributes"]["city"], respond["data"]["attributes"]["country"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return prop;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no accesstoken is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

EntityCollection<Device> EntityContext::getAssociatedConnectorDevices(std::string API_tenant_UUID, std::string API_connector_UUID, PagingOption paging/*={}*/, std::string aksID/*={}*/, std::string localAksID/*={}*/, std::string description/*={}*/, std::string unit/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/, std::time_t deletedUntil/*=-1*/,
    std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_connector_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestConnectorAssociatedDevices(API_tenant_UUID, API_connector_UUID, aksID, localAksID, description, unit, createdFrom, createdUntil, deletedUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestConnectorAssociatedDevices(API_tenant_UUID, API_connector_UUID, aksID, localAksID, description, unit, createdFrom, createdUntil, deletedUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestConnectorAssociatedDevices(API_tenant_UUID, API_connector_UUID, aksID, localAksID, description, unit, createdFrom, createdUntil, deletedUntil, paging.page_size);
        break;
        default:
            r = api_context.requestConnectorAssociatedDevices(API_tenant_UUID, API_connector_UUID, aksID, localAksID, description, unit, createdFrom, createdUntil, deletedUntil);
    }

    json respond = parseResponse(r);

    std::vector<Device> devices;
    for(json json_dev: respond["data"]) {
        if(json_dev["type"] == "devices") {
            try {
                Device device(json_dev["id"], API_tenant_UUID, json_dev["attributes"]["aksId"], json_dev["attributes"]["localAksId"], json_dev["attributes"]["description"], json_dev["attributes"]["unit"], Util::parseDateTimeString(json_dev["meta"]["createdAt"]), Util::parseDateTimeString(json_dev["meta"]["updatedAt"]), this);

                devices.push_back(device);
            } catch (std::exception& e) {
                errorHandler(e, json_dev);
            }
        } 
    }

    return std::make_pair(devices, parsePaging(respond));
}

Connector EntityContext::createConnector(std::string API_tenant_UUID, std::string name) {

    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestCreateConnector(API_tenant_UUID, name);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "connectors") {
        try {
            Connector conn(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["name"], respond["data"]["attributes"]["token"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return conn;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no accesstoken is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

void EntityContext::deleteConnector(std::string API_tenant_UUID, std::string API_connector_UUID) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_connector_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestDeleteConnector(API_tenant_UUID, API_connector_UUID);
    
    json respond = parseResponse(r);

    // OK response with 204 No Content
}

Connector EntityContext::updateConnector(std::string API_tenant_UUID, std::string API_connector_UUID, std::string name/*={}*/) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_connector_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestUpdateConnector(API_tenant_UUID, API_connector_UUID, name);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "connectors") {
        try {
            // token left empty
            Connector conn(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["name"], "", Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return conn;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no accesstoken is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }

}

PermissionData EntityContext::getConnectorPermissions(std::string API_tenant_UUID, std::string API_connector_UUID) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_connector_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestConnectorPermissions(API_tenant_UUID, API_connector_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "permissions") {
        try {
            PermissionData perm;
            perm.role = respond["data"]["attributes"]["role"];

            for(auto it: respond["data"]["attributes"]["resources"].items()) {
                std::cout << it.key() << " | " << it.value() << "\n";
                std::vector<std::string> resources = {};
                for (auto it : it.value()) {
                    resources.push_back(it);
                }
                perm.resources[it.key()] = resources;
            }

            return perm;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no tenant is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain permissions data.");
    }
}

std::string EntityContext::getNewConnectorAuthToken(std::string API_tenant_UUID, std::string API_connector_UUID) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_connector_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestConnectorToken(API_tenant_UUID, API_connector_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "accesstoken") {
        try {
            return respond["data"]["attributes"]["token"];
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no accesstoken is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

Device EntityContext::getDevice(std::string API_tenant_UUID, std::string API_device_UUID) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_device_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestDevice(API_tenant_UUID, API_device_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "devices") {
        try {
            Device dev(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["aksId"], respond["data"]["attributes"]["localAksId"], respond["data"]["attributes"]["description"], respond["data"]["attributes"]["unit"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return dev;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no accesstoken is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

EntityCollection<Device> EntityContext::getDevicesCollection(std::string API_tenant_UUID, PagingOption paging/*={}*/, std::string aksID/*={}*/, std::string localAksID/*={}*/, std::string API_connector_UUID/*={}*/, std::string API_property_UUID/*={}*/, std::string description/*={}*/, std::string unit/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/, std::time_t deletedUntil/*=-1*/,
    std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {
    
    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestDeviceCollection(API_tenant_UUID, aksID, localAksID, API_connector_UUID, API_property_UUID, description, unit, createdFrom, createdUntil, deletedUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestDeviceCollection(API_tenant_UUID, aksID, localAksID, API_connector_UUID, API_property_UUID, description, unit, createdFrom, createdUntil, deletedUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestDeviceCollection(API_tenant_UUID, aksID, localAksID, API_connector_UUID, API_property_UUID, description, unit, createdFrom, createdUntil, deletedUntil, paging.page_size);
        break;
        default:
            r = api_context.requestDeviceCollection(API_tenant_UUID, aksID, localAksID, API_connector_UUID, API_property_UUID, description, unit, createdFrom, createdUntil, deletedUntil);
    }

    json respond = parseResponse(r);

    std::vector<Device> devices;
    for(json json_dev: respond["data"]) {
        if(json_dev["type"] == "devices") {
            try {
                Device device(json_dev["id"], API_tenant_UUID, json_dev["attributes"]["aksId"], json_dev["attributes"]["localAksId"], json_dev["attributes"]["description"], json_dev["attributes"]["unit"], Util::parseDateTimeString(json_dev["meta"]["createdAt"]), Util::parseDateTimeString(json_dev["meta"]["updatedAt"]), this);

                devices.push_back(device);
            } catch (std::exception& e) {
                errorHandler(e, json_dev);
            }
        } 
    }

    return std::make_pair(devices, parsePaging(respond));
}

Connector EntityContext::getAssociatedConnector(std::string API_tenant_UUID, std::string API_device_UUID) {
    
    validateUUID(API_tenant_UUID);
    validateUUID(API_device_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestDeviceAssociatedConnector(API_tenant_UUID, API_device_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "connectors") {
        try {
            // ML: token left empty, can be requested by updateConnectorToken
            Connector conn(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["name"], "", Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return conn;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no connector is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    } 
}

EntityCollection<Reading> EntityContext::getAssociatedReadings(std::string API_tenant_UUID, std::string API_device_UUID, PagingOption paging/*={}*/, std::time_t from/*=-1*/, std::time_t until/*=-1*/, std::time_t timestamp/*=-1*/, double value/*=std::numeric_limits<double>::quiet_NaN()*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/, 
    std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {
    
    validateUUID(API_tenant_UUID);
    validateUUID(API_device_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestDeviceAssociatedReadings(API_tenant_UUID, API_device_UUID, from, until, timestamp, value, createdFrom, createdUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestDeviceAssociatedReadings(API_tenant_UUID, API_device_UUID, from, until, timestamp, value, createdFrom, createdUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestDeviceAssociatedReadings(API_tenant_UUID, API_device_UUID, from, until, timestamp, value, createdFrom, createdUntil, paging.page_size);
        break;
        default:
            r = api_context.requestDeviceAssociatedReadings(API_tenant_UUID, API_device_UUID, from, until, timestamp, value, createdFrom, createdUntil);
    }
    
    json respond = parseResponse(r);

    std::vector<Reading> readings;
    for(json json_read: respond["data"]) {
        if(json_read["type"] == "readings") {
            try {
                Reading reading(json_read["id"], API_tenant_UUID, json_read["attributes"]["value"].get<double>(), Util::parseDateTimeString(json_read["attributes"]["timestamp"]), Util::parseDateTimeString(json_read["meta"]["createdAt"]), Util::parseDateTimeString(json_read["meta"]["updatedAt"]), this);

                readings.push_back(reading);
            } catch (std::exception& e) {
                errorHandler(e, json_read);
            }
        } 
    }

    return std::make_pair(readings, parsePaging(respond));
}

EntityCollection<SetPoint> EntityContext::getAssociatedSetPoints(std::string API_tenant_UUID, std::string API_device_UUID, PagingOption paging/*={}*/, std::time_t from/*=-1*/, std::time_t until/*=-1*/, std::time_t timestamp/*=-1*/, std::time_t currentTime/*=-1*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/,
        std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {
    
    validateUUID(API_tenant_UUID);
    validateUUID(API_device_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestDeviceAssociatedSetPoints(API_tenant_UUID, API_device_UUID, from, until, timestamp, currentTime, createdFrom, createdUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestDeviceAssociatedSetPoints(API_tenant_UUID, API_device_UUID, from, until, timestamp, currentTime, createdFrom, createdUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestDeviceAssociatedSetPoints(API_tenant_UUID, API_device_UUID, from, until, timestamp, currentTime, createdFrom, createdUntil, paging.page_size);
        break;
        default:
            r = api_context.requestDeviceAssociatedSetPoints(API_tenant_UUID, API_device_UUID, from, until, timestamp, currentTime, createdFrom, createdUntil);
    }
    
    json respond = parseResponse(r);

    std::vector<SetPoint> setpoints;
    for(json json_read: respond["data"]) {
        if(json_read["type"] == "setpoints") {
            try {
                SetPoint setpoint(json_read["id"], API_tenant_UUID, json_read["attributes"]["value"].get<double>(), Util::parseDateTimeString(json_read["attributes"]["timestamp"]), Util::parseDateTimeString(json_read["meta"]["createdAt"]), Util::parseDateTimeString(json_read["meta"]["updatedAt"]), this);

                setpoints.push_back(setpoint);
            } catch (std::exception& e) {
                errorHandler(e, json_read);
            }
        } 
    }

    return std::make_pair(setpoints, parsePaging(respond));
}

Device EntityContext::createDevice(std::string API_tenant_UUID, std::string API_connector_UUID, std::string API_property_UUID, std::string aksID, std::string description, std::string unit, std::string localAksID/*={}*/) {
        
    validateUUID(API_tenant_UUID);
    validateUUID(API_connector_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestCreateDevice(API_tenant_UUID, API_connector_UUID, API_property_UUID, aksID, description, unit, localAksID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "devices") {
        try {
            Device device(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["aksId"], respond["data"]["attributes"]["localAksId"], respond["data"]["attributes"]["description"], respond["data"]["attributes"]["unit"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return device;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no device is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

Device EntityContext::updateDevice(std::string API_tenant_UUID, std::string API_device_UUID, std::string aksID/*={}*/, std::string description/*={}*/, std::string unit/*={}*/) {
            
    validateUUID(API_tenant_UUID);
    validateUUID(API_device_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestUpdateDevice(API_tenant_UUID, API_device_UUID, aksID, description, unit);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "devices") {
        try {
            Device device(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["aksId"], respond["data"]["attributes"]["localAksId"], respond["data"]["attributes"]["description"], respond["data"]["attributes"]["unit"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return device;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no device is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

void EntityContext::deleteDevice(std::string API_tenant_UUID, std::string API_device_UUID) {
    cpr::Response r = api_context.requestDeleteDevice(API_tenant_UUID, API_device_UUID);
    
    json respond = parseResponse(r);

    // OK is empty
}


Reading EntityContext::getReading(std::string API_tenant_UUID, std::string API_reading_UUID) {
            
    validateUUID(API_tenant_UUID);
    validateUUID(API_reading_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestReading(API_tenant_UUID, API_reading_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "readings") {
        try {
            Reading reading(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["value"].get<double>(), Util::parseDateTimeString(respond["data"]["attributes"]["timestamp"]), Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return reading;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no reading is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

EntityCollection<Reading> EntityContext::getReadingsCollection(std::string API_tenant_UUID, PagingOption paging/*={}*/, std::time_t from/*=-1*/, std::time_t until/*=-1*/, 
    std::time_t timestamp/*=-1*/, double value/*=std::numeric_limits<double>::quiet_NaN()*/, std::string API_device_UUID/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/,
    std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {
                
    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestReadingCollection(API_tenant_UUID, from, until, timestamp, value, API_device_UUID, createdFrom, createdUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestReadingCollection(API_tenant_UUID, from, until, timestamp, value, API_device_UUID, createdFrom, createdUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestReadingCollection(API_tenant_UUID, from, until, timestamp, value, API_device_UUID, createdFrom, createdUntil, paging.page_size);
        break;
        default:
            r = api_context.requestReadingCollection(API_tenant_UUID, from, until, timestamp, value, API_device_UUID, createdFrom, createdUntil);
    }

    json respond = parseResponse(r);

    std::vector<Reading> readings;
    for(json json_read: respond["data"]) {
        if(json_read["type"] == "readings") {
            try {
                Reading read(json_read["id"], API_tenant_UUID, json_read["attributes"]["value"].get<double>(), Util::parseDateTimeString(json_read["attributes"]["timestamp"]), Util::parseDateTimeString(json_read["meta"]["createdAt"]), Util::parseDateTimeString(json_read["meta"]["updatedAt"]), this);

                readings.push_back(read);
            } catch (std::exception& e) {
                errorHandler(e, json_read);
            }
        } 
    }

    return std::make_pair(readings, parsePaging(respond));
}

Device EntityContext::getAssociatedReadingsDevice(std::string API_tenant_UUID, std::string API_reading_UUID) {
                
    validateUUID(API_tenant_UUID);
    validateUUID(API_reading_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestReadingAssociatedDevice(API_tenant_UUID, API_reading_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "devices") {
        try {
            Device device(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["aksId"], respond["data"]["attributes"]["localAksId"], respond["data"]["attributes"]["description"], respond["data"]["attributes"]["unit"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return device;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no device is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

Device EntityContext::getAssociatedSetpointsDevice(std::string API_tenant_UUID, std::string API_setpoint_UUID) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_setpoint_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestSetPointAssociatedDevice(API_tenant_UUID, API_setpoint_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "devices") {
        try {
            Device device(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["aksId"], respond["data"]["attributes"]["localAksId"], respond["data"]["attributes"]["description"], respond["data"]["attributes"]["unit"], Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return device;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no device is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

Reading EntityContext::createReading(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp) {
                    
    validateUUID(API_tenant_UUID);
    validateUUID(API_device_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestCreateReading(API_tenant_UUID, API_device_UUID, value, timestamp);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "readings") {
        try {
            Reading reading(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["value"].get<double>(), Util::parseDateTimeString(respond["data"]["attributes"]["timestamp"]), Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return reading;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no reading is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

std::vector<Reading> EntityContext::createReadings(std::string API_tenant_UUID, std::vector<ReadingSetData> readings,
    std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {

    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestCreateReadingsSet(API_tenant_UUID, readings);
    
    json respond = parseResponse(r);

    std::vector<Reading> createdReadings;
    for(json json_read: respond["data"]) {
        if(json_read["type"] == "readings") {
            try {
                Reading read(json_read["id"], API_tenant_UUID, json_read["attributes"]["value"].get<double>(), Util::parseDateTimeString(json_read["attributes"]["timestamp"]), Util::parseDateTimeString(json_read["meta"]["createdAt"]), Util::parseDateTimeString(json_read["meta"]["updatedAt"]), this);

                createdReadings.push_back(read);
            } catch (std::exception& e) {
                errorHandler(e, json_read);
            }
        } 
    }

    return createdReadings;
}

Reading EntityContext::updateReading(std::string API_tenant_UUID, std::string API_reading_UUID, double value/*=std::numeric_limits<double>::quiet_NaN()*/, std::time_t timestamp/*=-1*/, std::string API_device_UUID/*={}*/) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_reading_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestUpdateReading(API_tenant_UUID, API_reading_UUID, value, timestamp, API_device_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "readings") {
        try {
            Reading reading(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["value"].get<double>(), Util::parseDateTimeString(respond["data"]["attributes"]["timestamp"]), Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return reading;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no reading is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

void EntityContext::deleteReading(std::string API_tenant_UUID, std::string API_reading_UUID) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_reading_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestDeleteReading(API_tenant_UUID, API_reading_UUID);
    
    json respond = parseResponse(r);

    // OK response is empty
}

SetPoint EntityContext::getSetPoint(std::string API_tenant_UUID, std::string API_setpoint_UUID) {
                    
    validateUUID(API_tenant_UUID);
    validateUUID(API_setpoint_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestSetPoint(API_tenant_UUID, API_setpoint_UUID);

    json respond = parseResponse(r);

    if(respond["data"]["type"] == "setpoints") {
        try {
            SetPoint setpoint(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["value"].get<double>(), Util::parseDateTimeString(respond["data"]["attributes"]["timestamp"]), Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return setpoint;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no setpoints is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

EntityCollection<SetPoint> EntityContext::getSetPointsCollection(std::string API_tenant_UUID, PagingOption paging/*={}*/, std::time_t from/*=-1*/, std::time_t until/*=-1*/, 
    std::time_t timestamp/*=-1*/, std::time_t currentTime/*=-1*/, std::string API_device_UUID/*={}*/, std::time_t createdFrom/*=-1*/, std::time_t createdUntil/*=-1*/,
    std::function<void (std::exception&, json&)> errorHandler/*=[](std::exception& e, json& j){}*/) {
                    
    validateUUID(API_tenant_UUID);
    checkAndRenewAuthentication();

    cpr::Response r;

    switch(paging.direction) {
        case PagingOption::Direction::PREVIOUS:

            r = api_context.requestSetPointCollection(API_tenant_UUID, from, until, timestamp, currentTime, API_device_UUID, createdFrom, createdUntil, paging.page_size, paging.page_pointer, {});
        break;
        case PagingOption::Direction::NEXT:

            r = api_context.requestSetPointCollection(API_tenant_UUID, from, until, timestamp, currentTime, API_device_UUID, createdFrom, createdUntil, paging.page_size, {}, paging.page_pointer);
        break;
        case PagingOption::Direction::NONE:

            r = api_context.requestSetPointCollection(API_tenant_UUID, from, until, timestamp, currentTime, API_device_UUID, createdFrom, createdUntil, paging.page_size);
        break;
        default:
            r = api_context.requestSetPointCollection(API_tenant_UUID, from, until, timestamp, currentTime, API_device_UUID, createdFrom, createdUntil);
    }
    
    json respond = parseResponse(r);

    std::vector<SetPoint> setpoints;
    for(json json_set: respond["data"]) {
        if(json_set["type"] == "setpoints") {
            try {
                SetPoint setpoint(json_set["id"], API_tenant_UUID, json_set["attributes"]["value"].get<double>(), Util::parseDateTimeString(json_set["attributes"]["timestamp"]), Util::parseDateTimeString(json_set["meta"]["createdAt"]), Util::parseDateTimeString(json_set["meta"]["updatedAt"]), this);

                setpoints.push_back(setpoint);
            } catch (std::exception& e) {
                errorHandler(e, json_set);
            }
        } 
    }

    return std::make_pair(setpoints, parsePaging(respond));
}

SetPoint EntityContext::createSetPoint(std::string API_tenant_UUID, std::string API_device_UUID, double value, std::time_t timestamp) {
                    
    validateUUID(API_tenant_UUID);
    validateUUID(API_device_UUID);
    checkAndRenewAuthentication();
    
    cpr::Response r = api_context.requestCreateSetPoint(API_tenant_UUID, API_device_UUID, value, timestamp);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "setpoints") {
        try {
            SetPoint setpoint(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["value"].get<double>(), Util::parseDateTimeString(respond["data"]["attributes"]["timestamp"]), Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return setpoint;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no setpoint is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

SetPoint EntityContext::updateSetPoint(std::string API_tenant_UUID, std::string API_setpoint_UUID, double value/*=std::numeric_limits<double>::quiet_NaN()*/, std::time_t timestamp/*=-1*/, std::string API_device_UUID/*={}*/) {

    validateUUID(API_tenant_UUID);
    validateUUID(API_setpoint_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestUpdateSetPoint(API_tenant_UUID, API_setpoint_UUID, value, timestamp, API_device_UUID);
    
    json respond = parseResponse(r);

    if(respond["data"]["type"] == "setpoints") {
        try {
            SetPoint setpoint(respond["data"]["id"], API_tenant_UUID, respond["data"]["attributes"]["value"].get<double>(), Util::parseDateTimeString(respond["data"]["attributes"]["timestamp"]), Util::parseDateTimeString(respond["data"]["meta"]["createdAt"]), Util::parseDateTimeString(respond["data"]["meta"]["updatedAt"]), this);

            return setpoint;
        } catch (...) {
            // Invalid JSON returned by the api, throw InvalidResponse and the nested original exception
            std::throw_with_nested(InvalidResponse("Invalid response received from the BAScloud API. Response did not contain expected data."));
        }
    } else {
        // If no setpoint is contained in the response data return invalid response
        throw InvalidResponse("Invalid response received from the BAScloud API. Response did not contain user data.");
    }
}

void EntityContext::deleteSetPoint(std::string API_tenant_UUID, std::string API_setpoint_UUID) {
    
    validateUUID(API_tenant_UUID);
    validateUUID(API_setpoint_UUID);
    checkAndRenewAuthentication();

    cpr::Response r = api_context.requestDeleteSetPoint(API_tenant_UUID, API_setpoint_UUID);
    
    json respond = parseResponse(r);

    // OK response is empty
}

}