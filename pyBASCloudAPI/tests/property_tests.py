import unittest
import time 
import datetime

from definitions import *

import pyBAScloudAPI as api

BCAPI = api.EntityContext(BASCLOUD_TEST_URL)


def errorHandler(e, j):
    raise e

def authenticate():
    currentDateTime = int(time.time())
    print("CURRENT DATETIME: ", currentDateTime)

    currentDateTimeString = datetime.datetime.fromtimestamp(currentDateTime).strftime("%FT%T.000Z")
    print("CURRENT DATETIME STR: ", currentDateTimeString)

    BCAPI.authenticateWithUserLogin(BASCLOUD_TEST_EMAIL, BASCLOUD_TEST_PASS)

    print("TOKEN: ", BCAPI.getToken())
    print("EXPIRATION DATE: ", datetime.datetime.fromtimestamp(BCAPI.getTokenExpirationDate()).strftime("%FT%T.000Z"))


class TestPropertyCollection(unittest.TestCase):

    def test_property(self):
        print("\tRequesting all properties...")

        props = BCAPI.getPropertiesCollection(BASCLOUD_TEST_TENANT_UUID, errorHandler=errorHandler)
        print("\t\tOK.")

        print("\tFound properties: ", len(props[0]))

        self.assertTrue(len(props[0]) >= 1)


class TestSingleProperty(unittest.TestCase):

    def test_property(self):
        print("\tRequesting single property with UUID...")

        property = BCAPI.getProperty(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_PROPERTY_UUID)
        print("\t\tOK.")

        print("\tProperty UUID: ", property.uuid)
        print("\tProperty Name: ", property.name)
        print("\tProperty Address: {}, {} {}, {}".format(property.street, property.postalCode, property.city, property.country))
        print("\ttProperty created at: ", datetime.datetime.fromtimestamp(property.createdAt).strftime("%FT%T.000Z"))
        print("\ttProperty updated at: ", datetime.datetime.fromtimestamp(property.updatedAt).strftime("%FT%T.000Z"))

        self.assertEqual(property.uuid, BASCLOUD_TEST_PROPERTY_UUID)
        self.assertEqual(property.name, "Nils Test-Property")
        self.assertEqual(property.street, "Dieselstraße 6")
        self.assertEqual(property.postalCode, "63739")
        self.assertEqual(property.city, "Aschaffenburg")
        self.assertEqual(property.country, "Deutschland")
        self.assertTrue(property.createdAt > 0)
        self.assertTrue(property.updatedAt > 0)


class TestAssociatedConnector(unittest.TestCase):

    def test_property(self):
        print("\tRequesting associated connectors of the property...")

        prop_connectors = BCAPI.getAssociatedConnectors(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_PROPERTY_UUID, errorHandler=errorHandler)
        print("\t\tOK.")

        print("\tFound property connectors: ", len(prop_connectors[0]))

        self.assertTrue(len(prop_connectors[0]) >= 1)


class TestCreateUpdateAndDeleteProperty(unittest.TestCase):

    def test_property(self):
        print("\tCreating new property...")

        new_property = BCAPI.createProperty(BASCLOUD_TEST_TENANT_UUID, "TestProperty", "Street", "12345", "City", "Deutschland")
        print("\t\tOK.")

        new_uuid = new_property.uuid

        self.assertEqual(new_property.name, "TestProperty")
        self.assertEqual(new_property.street, "Street")
        self.assertEqual(new_property.postalCode, "12345")
        self.assertEqual(new_property.city, "City")
        self.assertEqual(new_property.country, "Deutschland")
        self.assertTrue(new_property.createdAt > 0)
        self.assertTrue(new_property.updatedAt > 0)

        print("\tUpdating newly created property...")

        up_re_property = BCAPI.updateProperty(BASCLOUD_TEST_TENANT_UUID, new_uuid, "TestProperty 2")
        print("\t\tOK.")

        self.assertEqual(up_re_property.name, "TestProperty 2")

        print("\tDeleting newly created property...")
        BCAPI.deleteProperty(BASCLOUD_TEST_TENANT_UUID, new_uuid)
        print("\t\tOK.")

        try:
            non_property = BCAPI.getProperty(BASCLOUD_TEST_TENANT_UUID, new_uuid)
        except api.NotFoundRequest:
            self.assertTrue(True)
        except:
            self.assertTrue(False)



if __name__ == '__main__':
    authenticate()
    unittest.main()