#pragma once

#include <string>
#include <regex>
#include <map>
#include <chrono>
#include <iomanip>

namespace BAScloud {

    /** 
     * Util class providing various utility functions for handling BAScloud request and responses.
     */
    class Util {

     public:

        /**
         * Parses a URL string and extracts the contained parameters. 
         *
         * Given a URL its parameters are extracted and returned as a Map with parameter names as key. 
         * 
         * Example: The following URL 
         * test.local/devices?page[size]=1&page[before]=Mzk5ZTM1MWYtNTI3OS00YzFhLTk0MmUtYTZiODBmMjFiYzVh
         * returns a Map with keys {"page[size]", "page[before]"} and values {"1", "Mzk5ZTM1MWYtNTI3OS00YzFhLTk0MmUtYTZiODBmMjFiYzVh"}.
         * 
         * @param url A HTTP/S URL.
         * 
         * @return A Map with the extracted parameter names as keys and their values as values.
         */
        static std::map<std::string, std::string> parseURLParameter(std::string url) {

            std::map<std::string, std::string> param_map;

            std::regex param_reg("([^&?]*?)=([^&]*)");   // matches name=value pairs of parameters after ? divided by &

            std::regex_token_iterator<std::string::iterator> rend; // End iterator

            std::regex_token_iterator<std::string::iterator> key(url.begin(), url.end(), param_reg, 1); // Find first match group, the parameter name
            std::regex_token_iterator<std::string::iterator> val(url.begin(), url.end(), param_reg, 2); // Find second match group, the parameter value

            while (key != rend && val != rend) {
                param_map.emplace(*key++, *val++);
            } 

            return param_map;
        }

        /**
         * Parses a ISO 8601 timestamp string and returns a UNIX timestamp. 
         * 
         * Given a ISO 8601 formatted timestamp string ("%Y-%m-%dT%H:%M:%S") the function returns its corresponding
         * UNIX timestamp. Local time is used for conversion.
         * 
         * Example input: "2019-08-22T10:55:23.000Z" 
         * 
         * @param dateTime A timestamp in ISO 8601 format.
         * 
         * @return UNIX timestamp in seconds.
         */
        static std::time_t parseDateTimeString(std::string dateTime) {
            //std::string s{"2019-08-22T10:55:23.000Z"};
            std::tm t{};
            t.tm_isdst = -1; // Needs to be set to determine if daylight saving time is on
            std::istringstream ss(dateTime);
            
            ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
            if (ss.fail()) {
                throw std::runtime_error{"failed to parse time string"};
            }   

            return std::mktime(&t);
        }

    };
    
} 
