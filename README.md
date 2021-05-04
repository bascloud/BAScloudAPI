# BAScloudAPI

The BAScloud API Library is a C/C++ based library which wraps the BAScloud REST API (v2.1) endpoints into an interface for programmers to easily use e.g. for GLT integrations or client implementations.

The BAScloudAPI library provides a high-level abstraction of the BAScloud functions and entity data through an object-oriented concept.

The library is available in C/C++ and as a Python package for multiple platforms (Windows/Linux/MacOS).



## Usage

The class BAScloud::EntityContext is the basis for all high-level interaction with the BAScloud API. It provides wrapper functions for all API endpoint functions and delivers the response data in entity class objects. BAScloud::EntityContext further handles authentication, endpoint requests, response parsing, and errors.

Entity types of the BAScloud are adopted 1:1 into the classes 
BAScloud::User, BAScloud::Tenant, BAScloud::Property, BAScloud::Connector, BAScloud::Device, BAScloud::Reading, BAScloud::SetPoint. 

For accessing the raw response data of the REST API endpoints, the class BAScloud::APIContext provides a 1:1 implementation of the endpoints, without any additional handling of authentication, response parsing, or errors. 

**It is recommended to use BAScloud::EntityContext for all interactions with the API.**



## Library Documentation

See [here](https://bascloud.github.io/BAScloudAPI/) for the C/C++ library code documentation.



## API Documentation

A complete documentation of the REST API endpoints of the BAScloud is available under the following link.

<a target="_blank" href="https://docs.bascloud.net/">BAScloud API Documentation</a> 



## Examples

Complete demo examples for different use cases are located under [examples](/examples). For building examples set `BASCLOUDAPI_BUILD_EXAMPLES=ON` through cmake. (Default "ON")

Further example code can be found under [tests](/tests). For building tests set `BASCLOUDAPI_BUILD_TESTS=ON` through cmake. (Default "ON")

### Login

#### With User Login
```c++
#include "EntityContext.h"

using namespace BAScloud;

/**
 * Before accessing the BAScloud API, the correct API endpoint URL needs to be 
 * defined (Note: no trailing symbols, must end with TLD identifier)
 */
EntityContext BCAPI("server_URL");

/**
 * All methods in the BAScloud API library potential throw different exceptions to signal a failure in the API request.
 * 
 * The following exceptions may be thrown:
 * 
 * ServerError: If the server returns an (unexpected) server error 500.
 * ConnectionError: If the connection to the server could not be establised.
 * BadRequest: If parameters given through the request were invalid e.g. invalid UUID.
 * UnauthorizedRequest: If the currently authenticated user has no authority to access the API endpoint.
 * NotFoundRequest: If the requested resource was not found e.g. no entity with the given UUID exists.
 * InvalidResponse: If the response returned by the server was invalid e.g. invalid JSON was received or missing data.
 * 
 * Wherver to catch all exceptions together or individually is left up to the library user and may be use-case dependend.
 * 
 */
try {
    BCAPI.authenticateWithUserLogin("user_email", "user_password");
} catch(std::exception& e) {

}

```
#### With existing Connector token
For creating readings or devices from a connector, a connector authentication can be performed by requesting a connector token from an initial user login.
The connector token can be persisted locally on the connector as it never expires (only if the token is renewed).
**Note**: Connector token has limited authority in the BAScloud (compared to the user). 
```c++
#include "EntityContext.h"

using namespace BAScloud;

EntityContext BCAPI("server_URL");

/**
 * Use a connector token saved on disk or requested previously. 
 * 
 */
BCAPI.authenticateWithConnectorToken("connector_token");
```


### Access Readings

```c++
#include "EntityContext.h"

using namespace BAScloud;

// For accessing a device, its associated tenant UUID is needed
std::string tenantUUID = "";

/**
 * For requesting a collection of BAScloud readings, filters on their attributes can be applied.
 * Readings in a corresponding time-span can be searched for, for a specific timestamp, a specific value, 
 * and a corresponding device UUID.
 * 
 * Filter parameter are optional and can be ommitted, or left empty (default values)
 */
std::time_t from_filter = -1;
std::time_t until_filter = -1;
std::time_t timestamp_filter = -1;
double value_filter = std::numeric_limits<double>::quiet_NaN();
std::string deviceUUID_filter = {};

/**
 * For requesting a collection of BAScloud entities, pagination can be used for large collections.
 * 
 * PagingOption consists of:
 * page_size: number of entries per page.
 * direction: for navigating between pages, next, prev and none.
 * page_pointer: for navigating between pages, points to the next/prev page.
 * 
 * Paging are optional and can be ommitted, or left empty ({})
 */
PagingOption paging = PagingOption(1000, PagingOption::Direction::NONE, {});

try {
    /** 
     * Request a collection of devices from the BAScloud.
     * Parameters are a tenant UUID under which the devices are grouped, 
     * and optional paging and filters.
     * 
     * Each library method returning a collection supports an error handler function. This function 
     * is called for each entity in the request response for which an error occured e.g. failed to parse due to missing or 
     * invalid data.
     * 
     * The function is given the exception which was thrown and the json data which caused the error.
     */
    EntityCollection<Reading> readings = BCAPI.getReadingsCollection(tenantUUID, paging, from_filter, until_filter, 
    	timestamp_filter, value_filter, deviceUUID_filter, [](std::exception& e, json& j) {
            // Do something with the error and data, or ignore it (errorHandler can be left empty)
        });
} catch(std::exception& e) {

}
```

**Note**: For accessing single readings, a different approach can be used. See [examples](/examples).

## Compilation

**Requirements:**

- C/C++ compiler (CLANG/GCC/MSVC)

- CMake >= 2.11

- (Linux/macOS) OpenSSL Library (libssl-dev, openssl-devel, openssl)

- (Optional) Doxygen for documentation

- (Optional) gcov/lcov for test code coverage

  

Building the library:

```
mkdir build
cd build
cmake ..
make
```

### CMake parameters

The following build parameter are used with their default values:

```
BASCLOUDAPI_BUILD_TESTS ON
BASCLOUDAPI_BUILD_EXAMPLES ON
BASCLOUDAPI_BUILD_DOC OFF
BASCLOUDAPI_BUILD_PYTHON ON
BASCLOUDAPI_BUILD_CODE_COVERAGE OFF
```

### Tests

For API access tests, global variables for test login, server URL, and test UUIDs need to be defined first.

See [tests/CMakeLists.txt](/tests/CMakeLists.txt).

### Windows

For test code coverage currently only the compiler `clang`and `gcc` and their tools lcov and gcov are supported.

### RaspberryPi

To configure the build architecture to ARM, the ENV variable RASPBERRY_VERSION needs to be set to 1-4 e.g.:

`export RASPBERRY_VERSION=3`

#### Cross-compilation

For compiling RaspberryPi binaries on a different system, `clang` can be used in combination with an available root filesystem of a RaspberryPi . 

Either copy the root files from a RaspberryPi image, or mount an existing filesystem using sshfs to `toolchains/rpi_rootfs`.

The ENV variable RASPBERRY_VERSION needs to be set to 1-4.

See [crossbuild_rpi.sh](crossbuild_rpi.sh)

### ESP

Coming soon.


## Python

The code for the Python binding of the BAScloudAPI library is available under pyBAScloudAPI.

Pre-build packages for different platforms are provided under [releases](https://github.com/bascloud/BAScloudAPI/releases).

### Installation

Using the pre-build packages simply execute:

`python -m pip install pyBAScloudAPI-*.whl`

### Usage

```python
import pyBAScloudAPI as api

BCAPI = api.EntityContext("server url")
BCAPI.authenticateWithUserLogin("user_email", "user_password")
```

For complete examples see [pyBAScloudAPI/examples](pyBAScloudAPI/examples) and 
[pyBAScloudAPI/tests](pyBAScloudAPI/tests).

### Building

Building the pyBAScloudAPI package, the package *build* can be used.

`python -m pip install build`

Building the Python package then only involves executing:

`python -m build`

which will create a Python wheel package in the dist directory that can then be installed through

`python -m pip install pyBAScloudAPI-*.whl`

Alternative:

`python setup.py bdist_wheel` or

`python setup.py install` for direct installation.


**Note**: Building the Python package will build the C/C++ library, thus all requirements for its compilation need to be met.

### Tests

Python tests are located in [pyBAScloudAPI/tests](pyBAScloudAPI/tests).
