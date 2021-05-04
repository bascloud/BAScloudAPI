# -*- coding: utf-8 -*-

import datetime

import pyBAScloudAPI as api


def printErrorHandler(exception, json):
    print("\t\tException occured in request: ", exception, json)




print("Testing util functions.")
print("\t2021-04-26T10:56:58.000Z = ", api.Util.parseDateTimeString(dateTime="2021-04-26T10:56:58.000Z"))
assert api.Util.parseDateTimeString(dateTime="2021-04-26T10:56:58.000Z") == 1619427418

print("\tParameters:", api.Util.parseURLParameter(url="test.local/tenants/XXX/connectors/XXX/devices?page[size]=1&page[before]=Mzk5ZTM1MWYtNTI3OS00YzFhLTk0MmUtYTZiODBmMjFiYzVh"))



print("Demo of library methods for BAScloud API endpoints.")

print("Initialising library...")

BCAPI = api.EntityContext("https://basc-prd-apm-euw.azure-api.net/v2")

print("\tOK.")

print("1. - Authentication with user login")

BCAPI.authenticateWithUserLogin(email="erhardt@profm-gmbh.de", password="Dont4get$1")
print("\tOK.")
print("\tAuthenticated: ", BCAPI.isAuthenticated())
print("\tToken valid until: ", datetime.datetime.fromtimestamp(BCAPI.getTokenExpirationDate()))
print("\tToken: ", BCAPI.getToken())


print("2. - Access user information")

print("\tRequesting all users...")

users = BCAPI.getUsersCollection(errorHandler=printErrorHandler)
print("\t\tOK.")

print("\tFound users: ", len(users[0]))

print("\tRequesting single users with UUID...")

currentUser = BCAPI.getUser(users[0][0].uuid)
print("\t\tOK.")

print("\tUser UUID: ", currentUser.uuid)
print("\tUser Email: ", currentUser.email)
print("\tUser created at: ", datetime.datetime.fromtimestamp(currentUser.createdAt))
print("\tUser updated at: ", datetime.datetime.fromtimestamp(currentUser.updatedAt))


print("\tRequesting associated tenant...")

assoc_tenant = BCAPI.getAssociatedTenant(currentUser.uuid)
print("\t\tOK.")

print("\tTenant UUID: ", assoc_tenant.uuid)



print("3. - Access tenant information")

print("\tRequesting all tenants...")

tenants = BCAPI.getTenantsCollection()
print("\t\tOK.")

print("\tFound tenants: ", len(tenants[0]))

print("\tRequesting single tenant with UUID...")

tenant = BCAPI.getTenant(tenants[0][0].uuid)
print("\t\tOK.")

print("\tTenant UUID: ",  tenant.uuid)
print("\tTenant Name: ", tenant.name)
print("\tTenant URL Name: ", tenant.urlName)
print("\tTenant created at: ", datetime.datetime.fromtimestamp(tenant.createdAt))
print("\tTenant updated at: ", datetime.datetime.fromtimestamp(tenant.updatedAt))


print("\tRequesting associated users of tenant...")

tenant_user = BCAPI.getAssociatedUsers(tenant.uuid)
print("\t\tOK.")

print("\tFound tenant users: ", len(tenant_user[0]))


# print("3.5 - Create tenant and update")

# print("\tCreating new tenant...")

# new_tenant = BCAPI.createTenant("MoritzTestTenant", currentUser.uuid);
# print("\t\tOK.")

# print("\tNew tenant UUID: ", new_tenant.uuid)

# print("\tRequesting newly created tenant with UUID...")

# new_re_tenant = BCAPI.getTenant(new_tenant.uuid);
# print("\t\tOK.")

# print("\tTenant UUID: ", new_re_tenant.uuid)
# print("\tTenant Name: ", new_re_tenant.name)
# print("\tTenant URL Name: ", new_re_tenant.urlName)
# print("\tTenant created at: ", datetime.datetime.fromtimestamp(new_re_tenant.createdAt))
# print("\tTenant updated at: ", datetime.datetime.fromtimestamp(new_re_tenant.createdAt))


# print("\tUpdating newly created tenant...")

# up_re_tenant = BCAPI.updateTenant(new_re_tenant.uuid, "MoritzTestTenant2")
# print("\t\tOK.")

# print("\tTenant Name: ", up_re_tenant.getName())

# print("\tRequesting associated users of tenant...")

# re_tenant_user = BCAPI.getAssociatedUsers(new_re_tenant.uuid)
# print("\t\tOK.")

# print("\tFound tenant users: ", len(re_tenant_user[0]))

# print("\tDeleting newly created tenant...")

# BCAPI.deleteTenant(new_re_tenant.uuid)
# print("\t\tOK.")


print("4. - Access property information")

print("\tRequesting all properties...")

props = BCAPI.getPropertiesCollection(tenant.uuid)
print("\t\tOK.")

print("\tFound properties: ", len(props[0]))

print("\tRequesting single property with UUID...")
print("\tProperty UUID: ", props[0][0].uuid)

property = BCAPI.getProperty(tenant.uuid, props[0][0].uuid)
print("\t\tOK.")

print("\tProperty UUID: ", property.uuid)
print("\tProperty Name: ", property.name)
print("\tProperty Address: {}, {} {}, {}".format(property.street, property.postalCode, property.city, property.country))
print("\tProperty created at: ", datetime.datetime.fromtimestamp(property.createdAt))
print("\tProperty updated at: ", datetime.datetime.fromtimestamp(property.updatedAt))

print("\tRequesting associated connectors of the property...")

prop_connectors = BCAPI.getAssociatedConnectors(tenant.uuid, property.uuid)
print("\t\tOK.")

print("\tFound property connectors: ", len(prop_connectors[0]))


print("4.5 - Create property and update")

print("\tCreating new property...")

new_property = BCAPI.createProperty(tenant.uuid, "MoritzTestProperty", "Street", "12345", "City", "Country")
print("\t\tOK.")

print("\tNew property UUID: ", new_property.uuid)

print("\tRequesting newly created property with UUID...")

new_re_property = BCAPI.getProperty(tenant.uuid,new_property.uuid)
print("\t\tOK.")

print("\tProperty UUID: ",  new_re_property.uuid)
print("\tProperty Name: ", new_re_property.name)
print("\tProperty Address: {}, {} {}, {}".format(new_re_property.street, new_re_property.postalCode, new_re_property.city, new_re_property.country))
print("\tProperty created at: ", datetime.datetime.fromtimestamp(new_re_property.createdAt))
print("\tProperty updated at: ", datetime.datetime.fromtimestamp(new_re_property.updatedAt))


print("\tUpdating newly created property...")

up_re_property = BCAPI.updateProperty(tenant.uuid, new_re_property.uuid, "MoritzTestProperty2");
print("\t\tOK.")

print("\tProperty Name: ", up_re_property.name)


# Should be always be empty see next section create new connector for it
print("\tRequesting associated connectors of the property...")

new_prop_connectors = BCAPI.getAssociatedConnectors(tenant.uuid, up_re_property.uuid)
print("\t\tOK.")

print("\tFound property connectors: ", len(new_prop_connectors[0]))


# print("\tDeleting newly created property...")

# BCAPI.deleteProperty(tenant.uuid, up_re_property.uuid)
# print("\t\tOK.")



print("5. - Access connector information")

print("\tRequesting all connectors...")

connectors = BCAPI.getConnectorsCollection(tenant.uuid)
print("\t\tOK.")

print("\tFound connectors: ", len(connectors[0]))

print("\tRequesting single connector with UUID...")
print("\tConnector UUID: ", connectors[0][0].uuid)

connector = BCAPI.getConnector(tenant.uuid, connectors[0][0].uuid)
print("\t\tOK.")

print("\tConnector UUID: ", connector.uuid)
print("\tConnector Name: ", connector.name)
print("\tConnector created at: ", datetime.datetime.fromtimestamp(connector.createdAt))
print("\tConnector updated at: ", datetime.datetime.fromtimestamp(connector.updatedAt))

print("\tRequesting associated property of the connector again...")

conn_prop = BCAPI.getAssociatedProperty(tenant.uuid, connector.uuid)
print("\t\tOK.")

print("\tConnector's property UUID: ", conn_prop.uuid)


print("\tRequest connector's associated devices...")

max_devices = 0
for conn in connectors[0]:
    conn_devices = BCAPI.getAssociatedDevices(tenant.uuid, conn.uuid)
    if len(conn_devices[0]) > max_devices:
        max_devices = len(conn_devices[0])
        connector = conn
    print("\t\tFound connector with ", len(conn_devices[0]), " devices")



print("5.5 - Create connector and update")

print("\tCreating new connector...")

new_connector = BCAPI.createConnector(tenant.uuid, new_property.uuid, "MoritzTestConnector")
print("\t\tOK.")

print("\tNew connector UUID: ", new_connector.uuid)

print("\tRequesting new API key for created connector...")

connectorToken = BCAPI.getNewConnectorAuthToken(tenant.uuid, new_connector.uuid)
print("\t\tOK.")

print("\tConnector Auth. Token: ", connectorToken)

print("\tRequesting newly created connector with UUID...")

new_re_connector = BCAPI.getConnector(tenant.uuid, new_connector.uuid)
print("\t\tOK.")

print("\tConnector UUID: ", new_re_connector.uuid)
print("\tConnector Name: ", new_re_connector.name)
print("\tConnector created at: ", datetime.datetime.fromtimestamp(new_re_connector.createdAt))
print("\tConnector updated at: ", datetime.datetime.fromtimestamp(new_re_connector.createdAt))


print("\tUpdating newly created connector...")

up_re_connector = BCAPI.updateConnector(tenant.uuid, new_re_connector.uuid, "MoritzTestConnector2");
print("\t\tOK.")

print("\tConnector Name: ", up_re_connector.name)



print("\tRequesting associated connectors of the new property...")

new_prop_connectors = BCAPI.getAssociatedConnectors(tenant.uuid, new_property.uuid);
print("\t\tOK.")

print("\tFound property connectors: ", len(new_prop_connectors[0]))


# print("\tDeleting newly created connector...")

# BCAPI.deleteConnector(tenant.uuid, new_connector.uuid);
# print("\t\tOK.")

# print("\tDeleting newly created property...")

# BCAPI.deleteProperty(tenant.uuid, new_property.uuid);
# print("\t\tOK.")



print("6. - Access device information")

print("\tRequesting all devices...")

devices = BCAPI.getDevicesCollection(tenant.uuid)
print("\t\tOK.")

print("\tFound devices: ", len(devices[0]))

print("\tRequesting single device with UUID...")
print("\tDevice UUID: ", devices[0][0].uuid)

device = BCAPI.getDevice(tenant.uuid, devices[0][0].uuid)
print("\t\tOK.")

print("\tDevice UUID: ", device.uuid)
print("\tDevice AKS ID: ", device.aksID)
print("\tDevice Description: ", device.description)
print("\tDevice Unit: ", device.unit)
print("\tDevice created at: ", datetime.datetime.fromtimestamp(device.createdAt))
print("\tDevice updated at: ", datetime.datetime.fromtimestamp(device.updatedAt))

print("\tRequesting associated connector of the device again...")

device_conn = BCAPI.getAssociatedConnector(tenant.uuid, device.uuid)
print("\t\tOK.")

print("\tDevice's Connector UUID: ", device_conn.uuid)


print("\tRequesting device associated readings...")

max_readings = 0
for d in devices[0]:
    device_readings = BCAPI.getAssociatedReadings(tenant.uuid, d.uuid)
    if len(device_readings[0]) > max_readings:
        max_readings = len(device_readings[0])
        device = d
    print("\t\tFound device with ", len(device_readings[0]), " readings")

print("\tRequesting paginated device associated readings...")

paging = api.PagingOption(10, api.PagingOption.Direction.NONE, "")

device_readings = BCAPI.getAssociatedReadings(tenant.uuid, device.uuid, paging)
print("\t\tOK.")

print("\tFound readings: ", len(device_readings[0]))

for r in device_readings[0]:
    print("\t\tReading: ", datetime.datetime.fromtimestamp(r.timestamp), " - ", r.value)



print("\tRequesting device associated setpoints...")

max_setpoints = 0
for d in devices[0]:
    device_setpoint = BCAPI.getAssociatedSetPoints(tenant.uuid, d.uuid, paging)
    if len(device_setpoint[0]) > max_setpoints:
        max_setpoints = len(device_setpoint[0])
        device = d
    
    print("\t\tFound device with ", len(device_setpoint[0]), " setpoints")


print("\tRequesting paginated device associated setpoints...")

device_setpoint = BCAPI.getAssociatedSetPoints(tenant.uuid, device.uuid, paging)
print("\t\tOK.")

print("\tFound setpoints: ", len(device_setpoint[0]))

for sp in device_setpoint[0]:
    print("\t\Setpoint: ", datetime.datetime.fromtimestamp(sp.timestamp), " - ", sp.value)



print("6.5 - Create device and update")

print("\tCreating new device...")

new_device = BCAPI.createDevice(tenant.uuid, new_connector.uuid, "MoritzTestAKS1000", "TestDevice", "m3")
print("\t\tOK.")

print("\tNew device UUID: ", new_device.uuid)

print("\tRequesting newly created device with UUID...")

new_re_device = BCAPI.getDevice(tenant.uuid, new_device.uuid)
print("\t\tOK.")

print("\tDevice UUID: ", new_re_device.uuid)
print("\tDevice AKS ID: ", new_re_device.aksID)
print("\tDevice Description: ", new_re_device.description)
print("\tDevice Unit: ", new_re_device.unit)
print("\tDevice created at: ", datetime.datetime.fromtimestamp(new_re_device.createdAt))
print("\tDevice updated at: ", datetime.datetime.fromtimestamp(new_re_device.updatedAt))


print("\tUpdating newly created device...")

up_re_device = BCAPI.updateDevice(tenant.uuid, new_re_device.uuid, "MoritzTestAKS1001");
print("\t\tOK.")

print("\tDevice AKS ID: ", up_re_device.aksID)



print("\tRequesting associated connector of the new device...")

new_dev_connector = BCAPI.getAssociatedConnector(tenant.uuid, up_re_device.uuid)
print("\t\tOK.")

print("\tFound device connector: ", new_dev_connector.uuid)


# print("\tDeleting newly created device...")

# BCAPI.deleteDevice(tenant.uuid, new_device.uuid)
# print("\t\tOK.")


print("7. - Access reading information")

print("\tRequesting all readings...")

readings = BCAPI.getReadingsCollection(tenant.uuid)
print("\t\tOK.")

print("\tFound readings: ", len(readings[0]))

if len(readings[0]) > 0:

    print("\tRequesting single reading with UUID...")
    print("\tReading UUID: ", readings[0][0].uuid)

    reading = BCAPI.getReading(tenant.uuid, readings[0][0].uuid)
    print("\t\tOK.")

    print("\tReading UUID: ", reading.uuid)
    print("\tReading Value: ", reading.value)
    print("\tReading timestamp: ", datetime.datetime.fromtimestamp(reading.timestamp))
    print("\tReading created at: " , datetime.datetime.fromtimestamp(reading.createdAt))
    print("\tReading updated at: ", datetime.datetime.fromtimestamp(reading.updatedAt))

    print("\tRequesting associated device of the reading again...")

    read_device = BCAPI.getAssociatedDevice(tenant.uuid, reading.uuid)
    print("\t\tOK.")

    print("\tReadings's Device UUID: ", read_device.uuid)



print("8. - Create new reading")

currentDateTime = int(datetime.datetime.now().timestamp())

new_read = BCAPI.createReading(tenant.uuid, new_device.uuid, 1234.56, currentDateTime);
print("\t\tOK.")

print("\tRequesting created reading information again... ")

new_re_reading = BCAPI.getReading(tenant.uuid, new_read.uuid)
print("\t\tOK.")

print("\tReading UUID: ", new_re_reading.uuid)
print("\tReading Value: ", new_re_reading.value)
print("\tReading timestamp: ", datetime.datetime.fromtimestamp(new_re_reading.timestamp))
print("\tReading created at: ", datetime.datetime.fromtimestamp(new_re_reading.createdAt))
print("\tReading updated at: ", datetime.datetime.fromtimestamp(new_re_reading.updatedAt))


print("\tRequesting paginated device associated readings...")

new_device_readings = BCAPI.getAssociatedReadings(tenant.uuid, new_device.uuid, paging)
print("\t\tOK.")

print("\tFound readings: ", len(new_device_readings[0]))

for sp in new_device_readings[0]:
    print("\t\tReading: ",datetime.datetime.fromtimestamp(sp.timestamp), " - ", sp.value)



print("\tRequesting deletion of created reading again... ")

BCAPI.deleteReading(tenant.uuid, new_re_reading.uuid)
print("\t\tOK.")


print("9. - Access setpoint information")

print("\tRequesting all setpoints...")

setpoints = BCAPI.getSetPointsCollection(tenant.uuid)
print("\t\tOK.")

print("\tFound setpoints: ", len(setpoints[0]))

if len(setpoints[0]) > 0:

    print("\tRequesting single setpoint with UUID...")
    print("\tSetPoint UUID: ", setpoints[0][0].uuid)

    setpoint = BCAPI.getSetPoint(tenant.uuid, setpoints[0][0].uuid)
    print("\t\tOK.")

    print("\tSetPoint UUID: ", setpoint.uuid)
    print("\tSetPoint Value: ", setpoint.value)
    print("\tSetPoint timestamp: ", datetime.datetime.fromtimestamp(setpoint.timestamp))
    print("\tSetPoint created at: ", datetime.datetime.fromtimestamp(setpoint.createdAt))
    print("\tSetPoint updated at: ", datetime.datetime.fromtimestamp(setpoint.updatedAt))



print("10. - Create new setpoint")

currentDateTime = int(datetime.datetime.now().timestamp())

new_setPoint = BCAPI.createSetPoint(tenant.uuid, new_device.uuid, 2345.67, currentDateTime)
print("\t\tOK.")

print("\tRequesting created setpoint information again... ")

new_re_setpoint = BCAPI.getSetPoint(tenant.uuid, new_setPoint.uuid)
print("\t\tOK.")

print("\tSetPoint UUID: ", new_re_setpoint.uuid)
print("\tSetPoint Value: ", new_re_setpoint.value)
print("\tSetPoint timestamp: ", datetime.datetime.fromtimestamp(new_re_setpoint.timestamp))
print("\tSetPoint created at: ", datetime.datetime.fromtimestamp(new_re_setpoint.createdAt))
print("\tSetPoint updated at: ", datetime.datetime.fromtimestamp(new_re_setpoint.updatedAt))

print("\tRequesting paginated device associated setpoints...")

new_device_setpoint = BCAPI.getAssociatedSetPoints(tenant.uuid, new_device.uuid, paging)
print("\t\tOK.")

print("\tFound setpoints: ", len(new_device_setpoint[0]))

for sp in new_device_setpoint[0]:
    print("\t\tSetPoint: ", datetime.datetime.fromtimestamp(sp.timestamp), " - ", sp.value)


print("12. - Deleting created entities")

# // print("\tDeleting created device...")

# // BCAPI.deleteDevice(tenant.uuid, new_device.uuid);
# // print("\t\tOK.")


print("\tDeleting created connector...")

BCAPI.deleteConnector(tenant.uuid, new_connector.uuid)
print("\t\tOK.")


print("\tDeleting created property...")

BCAPI.deleteProperty(tenant.uuid, new_property.uuid)
print("\t\tOK.")