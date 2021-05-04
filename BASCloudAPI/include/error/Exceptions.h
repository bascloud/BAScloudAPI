#pragma once

#include <string>


namespace BAScloud {

// ML: We extend runtime_error as the base-class std::exception has no std::string message constructor.

/** 
 * 500 server HTTP error.
 * 
 * While requesting the BAScloud API the server returned an server error. 
 * If the problem reoccurs please contact the BAScloud support.
 * 
 */
class ServerError : public std::runtime_error {
 public:

 /**
  * ServerError constructor
  *
  * @param message Message describing the occurred error and potential causes.
  */
   ServerError(const std::string &message) : std::runtime_error(message) {
   }
};

/** 
 * A connection error occured.
 * 
 * While requesting the BAScloud API an connection failure occured. Check
 * for a working network connection and the correct endpoint URL.
 * 
 */
class ConnectionError : public std::runtime_error {
 public:
 /**
  * ConnectionError constructor
  *
  * @param message Message describing the occurred error and potential causes.
  */
   ConnectionError(const std::string &message) : std::runtime_error(message) {
   }
};

/** 
 * 404 Not Found HTTP error.
 * 
 * The requested resource could not be found but may be available in the future. 
 * Subsequent requests by the client are permissible.
 * 
 */
class NotFoundRequest : public std::runtime_error {
 public:
 /**
  * NotFoundRequest constructor
  *
  * @param message Message describing the occurred error and potential causes.
  */
   NotFoundRequest(const std::string &message) : std::runtime_error(message) {
   }
};

/** 
 * 400 Bad Request HTTP error.
 * 
 * The server cannot or will not process the request due to an apparent client error 
 * (e.g., malformed request syntax, size too large, invalid request message framing, 
 * or deceptive request routing)
 * 
 */
class BadRequest : public std::runtime_error {
 public:
 /**
  * BadRequest constructor
  *
  * @param message Message describing the occurred error and potential causes.
  */
   BadRequest(const std::string &message) : std::runtime_error(message) {
   }
};

/** 
 * 400 Conflict HTTP error.
 * 
 * Indicates that the request could not be processed because of conflict in 
 * the current state of the resource, such as an edit conflict between multiple simultaneous updates.
 * 
 */
class ConflictRequest : public std::runtime_error {
 public:
 /**
  * ConflictRequest constructor
  *
  * @param message Message describing the occurred error and potential causes.
  */
   ConflictRequest(const std::string &message) : std::runtime_error(message) {
   }
};

/** 
 * 401 Unauthorized HTTP error.
 * 
 * The current user does not have valid authentication credentials for the target resource.
 * This could mean a invalid authentication token or no authorisation for the resource (e.g. administration endpoints).
 * 
 */
class UnauthorizedRequest : public std::runtime_error {
 public:
 /**
  * UnauthorizedRequest constructor
  *
  * @param message Message describing the occurred error and potential causes.
  */
   UnauthorizedRequest(const std::string &message) : std::runtime_error(message) {
   }
};

/** 
 * Invalid response received from the BAScloud API.
 * 
 * There was an error while processing the API response e.g. missing attribute or invalid formated data. 
 * 
 */
class InvalidResponse : public std::runtime_error {
 public:
 /**
  * InvalidResponse constructor
  *
  * @param message Message describing the occurred error and potential causes.
  */
   InvalidResponse(const std::string &message) : std::runtime_error(message) {
   }
};

/**
  * Converts a HTTP status code to a textual phrase. 
  *
  * A numerical status code is converted to its textual description e.g. 404 to Not Found. 
  * 
  * Supports status codes in the range of 100 to 511. 
  * 
  * @param status_code A numerical HTTP status code.
  * 
  * @return A textual phrase describing the given HTTP status code.
  */
inline std::string HTTPCodeToPhrase(int status_code) {
  // ML: Source: https://github.com/j-ulrich/http-status-codes-cpp, License: CC0/Universal
  switch(status_code) {

    //####### 1xx - Informational #######
    case 100: return "Continue";
    case 101: return "Switching Protocols";
    case 102: return "Processing";
    case 103: return "Early Hints";

    //####### 2xx - Successful #######
    case 200: return "OK";
    case 201: return "Created";
    case 202: return "Accepted";
    case 203: return "Non-Authoritative Information";
    case 204: return "No Content";
    case 205: return "Reset Content";
    case 206: return "Partial Content";
    case 207: return "Multi-Status";
    case 208: return "Already Reported";
    case 226: return "IM Used";

    //####### 3xx - Redirection #######
    case 300: return "Multiple Choices";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 303: return "See Other";
    case 304: return "Not Modified";
    case 305: return "Use Proxy";
    case 307: return "Temporary Redirect";
    case 308: return "Permanent Redirect";

    //####### 4xx - Client Error #######
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 402: return "Payment Required";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 406: return "Not Acceptable";
    case 407: return "Proxy Authentication Required";
    case 408: return "Request Timeout";
    case 409: return "Conflict";
    case 410: return "Gone";
    case 411: return "Length Required";
    case 412: return "Precondition Failed";
    case 413: return "Payload Too Large";
    case 414: return "URI Too Long";
    case 415: return "Unsupported Media Type";
    case 416: return "Range Not Satisfiable";
    case 417: return "Expectation Failed";
    case 418: return "I'm a teapot";
    case 422: return "Unprocessable Entity";
    case 423: return "Locked";
    case 424: return "Failed Dependency";
    case 426: return "Upgrade Required";
    case 428: return "Precondition Required";
    case 429: return "Too Many Requests";
    case 431: return "Request Header Fields Too Large";
    case 451: return "Unavailable For Legal Reasons";

    //####### 5xx - Server Error #######
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    case 502: return "Bad Gateway";
    case 503: return "Service Unavailable";
    case 504: return "Gateway Time-out";
    case 505: return "HTTP Version Not Supported";
    case 506: return "Variant Also Negotiates";
    case 507: return "Insufficient Storage";
    case 508: return "Loop Detected";
    case 510: return "Not Extended";
    case 511: return "Network Authentication Required";

    default: return std::string();
  }
}


}