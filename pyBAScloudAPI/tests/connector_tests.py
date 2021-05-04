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


class TestConnectorCollection(unittest.TestCase):

    def test_connector(self):
        print("\tRequesting all connectors...")

        connectors = BCAPI.getConnectorsCollection(BASCLOUD_TEST_TENANT_UUID)
        print("\t\tOK.")

        print("\tFound properties: ", len(connectors[0]))

        self.assertTrue(len(connectors[0]) >= 1)


class TestSingleConnector(unittest.TestCase):

    def test_connector(self):
        print("\tRequesting single connector with UUID...")

        connector = BCAPI.getConnector(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_CONNECTOR_UUID)
        print("\t\tOK.")

        print("\tConnector UUID: ", connector.uuid)
        print("\tConnector Name: ", connector.name)
        print("\tConnector created at: ", datetime.datetime.fromtimestamp(connector.createdAt).strftime("%FT%T.000Z"))
        print("\tConnector updated at: ", datetime.datetime.fromtimestamp(connector.updatedAt).strftime("%FT%T.000Z"))

        self.assertEqual(connector.uuid, BASCLOUD_TEST_CONNECTOR_UUID)
        self.assertEqual(connector.name, "Stefan01")
        self.assertTrue(connector.createdAt > 0)
        self.assertTrue(connector.updatedAt > 0)


class TestAssociatedProperty(unittest.TestCase):

    def test_connector(self):
        print("\tRequesting associated property of the connector again...")

        conn_prop = BCAPI.getAssociatedProperty(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_CONNECTOR_UUID)
        print("\t\tOK.")

        print("\tConnector's property UUID: ", conn_prop.uuid)

        self.assertEqual(conn_prop.uuid, BASCLOUD_TEST_PROPERTY_UUID)


class TestAssociatedDevices(unittest.TestCase):

    def test_connector(self):
        print("\tRequest connector's associated devices...")

        conn_devices = BCAPI.getAssociatedDevices(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_CONNECTOR_UUID)

        print("\t\tFound connector with ", len(conn_devices[0]), " devices")

        self.assertTrue(len(conn_devices[0]) >= 1)


class TestCreateUpdateAndDeleteConnector(unittest.TestCase):

    def test_connector(self):
        print("\tCreating new connector...")

        new_connector = BCAPI.createConnector(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_PROPERTY_UUID, "TestConnector")
        print("\t\tOK.")

        new_uuid = new_connector.uuid

        self.assertEqual(new_connector.name, "TestConnector")
        self.assertTrue(new_connector.createdAt > 0)
        self.assertTrue(new_connector.updatedAt > 0)

        print("\tUpdating newly created connector...")

        up_re_connector = BCAPI.updateConnector(BASCLOUD_TEST_TENANT_UUID, new_uuid, "TestConnector2")
        print("\t\tOK.")

        self.assertEqual(up_re_connector.name, "TestConnector2")


        print("\tDeleting newly created connector...")
        BCAPI.deleteConnector(BASCLOUD_TEST_TENANT_UUID, new_uuid)
        print("\t\tOK.")

        try:
            non_connector = BCAPI.getConnector(BASCLOUD_TEST_TENANT_UUID, new_uuid)
        except api.NotFoundRequest:
            self.assertTrue(True)
        except:
            self.assertTrue(False)


if __name__ == '__main__':
    authenticate()
    unittest.main()