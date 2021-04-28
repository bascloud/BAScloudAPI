# -*- coding: utf-8 -*-
import datetime
import sys
import traceback

# import the BAS Cloud API library using the name api
import pyBASCloudAPI as api

# Error handler function which is given as a error callback to request functions
def printErrorHandler(exception, json):
    print("\t\tException occured in request: ", exception, json)


print("Initialising library...")

# For accessing the BASCloud API, a API endpoint URL needs to be defined (Note: no trailing symbols, must end with TLD identifier)
BCAPI = api.EntityContext("server_URL")
print("\tOK." )


# Before accessing any API methods, a user must be authenticated
print("1. - Authentication with user login" )

# /**
# * All methods in the BASCloud API library potential throw different exceptions to signal a failure in the API request.
# * 
# * The following exceptions may be thrown:
# * 
# * ServerError: If the server returns a (unexpected) server error 500.
# * ConnectionError: If the connection to the server could not be establised.
# * BadRequest: If parameters given through the request were invalid e.g. invalid UUID.
# * UnauthorizedRequest: If the currently authenticated user has no authority to access the API endpoint.
# * NotFoundRequest: If the requested resource was not found e.g. no entity with the given UUID exists.
# * InvalidResponse: If the response returned by the server was invalid e.g. invalid JSON was received or missing data.
# * 
# * Wherver to catch all exceptions together or individually is left up to the library user and may be use-case dependend.
# * 
# */
try:
    BCAPI.authenticateWithUserLogin(email="user_email", password="user_password")
    print("\tOK.")
except:
    print("\tRequest failed.\n")
    traceback.print_exc()
    sys.exit(1)

# Check if current authentication is valid, checks expiration date and valid token
print("\tAuthenticated: ", BCAPI.isAuthenticated())


print("2. - Create a new reading" )

# A reading contains a timestamp of the time the reading was read (as a UNIX timestamp in seconds)
currentDateTime = int(datetime.datetime.now().timestamp())

# For creating a reading, its associated tenant UUID is needed
tenantUUID = ""

# For creating a reading, its associated device UUID is needed describing the device for which a new reading is saved.
deviceUUID = ""

# The value reading of the device at the specified timestamp.
value = 1234.56


# /** 
# * If no tenant and device UUIDs are known, the needed information can be requested from the API
# * 
# * To retrieve the current tenant the user UUID is needed, if the UUID is not known, the current user can be requested through a collection request
# * Usually this returns only your own user, only for administators multiple users are returned
# */

# users = BCAPI.getUsersCollection(errorHandler=printErrorHandler)
# print("\t\tOK." )
# currentUser = users[0][0]

# # Request the associated tenant of the user based on the UUID
# associated_tenant = BCAPI.getAssociatedTenant(currentUser.uuid)
# print("\t\tOK." )

# print("\tTenant UUID: ",  associated_tenant.uuid )
# tenantUUID = associated_tenant.uuid

# # Now using the known tenant UUID, a device can be requested through i.e. a device collection request
# # To select a specific device, use the request filters
# aksID_filter = ""
# description_filter = ""
# unit_filter = ""

# devices = BCAPI.getDevicesCollection(tenantUUID=tenantUUID, aksID=aksID_filter, description=description_filter, unit=unit_filter, errorHandler=printErrorHandler)
# print("\t\tOK." )

# deviceUUID = devices[0][0].uuid


try:
    # /** 
    # * Creates a new reading in the BASCloud 
    # * Parameters are a tenant UUID under which the device and reading is grouped, 
    # * a device UUID for which the reading was read, the readings value and current timestamp.
    # */
    new_reading = BCAPI.createReading(tenantUUID=tenantUUID, deviceUUID=deviceUUID, value=value, timestamp=currentDateTime);
    print("\t\tOK." )

    print("\tCreated Reading:")
    print("\tReading UUID: ",  new_reading.uuid)
    print("\tReading Value: ",  new_reading.value)
    print("\tReading timestamp: ",  datetime.datetime.fromtimestamp(new_reading.timestamp))
    print("\tReading created at: ",  datetime.datetime.fromtimestamp(new_reading.createdAt))
    print("\tReading updated at: ",  datetime.datetime.fromtimestamp(new_reading.updatedAt))

except api.ServerError as e:
    print("\tServer Error occured.", e)
    sys.exit(1)
except api.ConnectionError as e:
    print("\tConnection Error occured.", e)
    sys.exit(1)
except api.BadRequest as e:
    print("\tBad Request. Request contained invalid parameters.", e)
    # Maybe try again...
    sys.exit(1)
except api.UnauthorizedRequest as e:
    print("\tCurrent user has no authority to create a reading.", e)
    # Possible the user has no authority to create a reading for this tenant or device.
    sys.exit(1)
except api.NotFoundRequest as e:
    print("\tGiven UUID was not found.", e)
    # Either the tenant UUID or device UUID was not found.
    sys.exit(1)
except api.InvalidResponse as e:
    print("\tServer responds with invalid.", e)
    # Maybe try again...
    sys.exit(1)


print("\tRequesting deletion of created reading again... ")

try:
    # Delete reading again
    BCAPI.deleteReading(tenantUUID=tenantUUID, readingUUID=new_reading.uuid)
    print("\t\tOK.")
except:
    print("\tRequest failed.\n")
    traceback.print_exc()
    sys.exit(1)

