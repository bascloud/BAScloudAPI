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



class TestDeviceCollection(unittest.TestCase):

    def test_device(self):
        print("\tRequesting all devices...")

        devices = BCAPI.getDevicesCollection(BASCLOUD_TEST_TENANT_UUID, errorHandler=errorHandler)
        print("\t\tOK.")

        print("\tFound devices: ", len(devices[0]))

        self.assertTrue(len(devices[0]) >= 1)


class TestSingleDevice(unittest.TestCase):

    def test_device(self):
        print("\tRequesting single device with UUID...")

        device = BCAPI.getDevice(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID)
        print("\t\tOK.")

        print("\tDevice UUID: ", device.uuid)
        print("\tDevice AKS ID: ", device.aksID)
        print("\tDevice Description: ", device.description)
        print("\tDevice Unit: ", device.unit)
        print("\tDevice created at: ", datetime.datetime.fromtimestamp(device.createdAt).strftime("%FT%T.000Z"))
        print("\tDevice updated at: ", datetime.datetime.fromtimestamp(device.updatedAt).strftime("%FT%T.000Z"))

        self.assertEqual(device.uuid, BASCLOUD_TEST_DEVICE_UUID)
        self.assertEqual(device.aksID, "nils-sdfdsfsdfsdfd")
        self.assertEqual(device.description, "some description")
        self.assertEqual(device.unit, "Celsius")
        self.assertTrue(device.createdAt > 0)
        self.assertTrue(device.updatedAt > 0)


class TestAssociatedConnector(unittest.TestCase):

    def test_device(self):
        print("\tRequesting associated connector of the device again...")

        device_conn = BCAPI.getAssociatedConnector(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID)
        print("\t\tOK.")

        self.assertEqual(device_conn.uuid, BASCLOUD_TEST_CONNECTOR_UUID)


class TestAssociatedReadings(unittest.TestCase):

    def test_device(self):
        device_readings = BCAPI.getAssociatedReadings(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID, errorHandler=errorHandler)

        print("\t\tFound device with ", len(device_readings[0]), " readings")

        self.assertTrue(len(device_readings[0]) >= 1)


class TestAssociatedSetPoints(unittest.TestCase):

    def test_device(self):
        device_setpoint = BCAPI.getAssociatedSetPoints(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID, errorHandler=errorHandler)

        print("\t\tFound device with ", len(device_setpoint[0]), " setpoints")

        self.assertTrue(len(device_setpoint[0]) >= 0) # May not any setpoints for it


# // Delete devices not working atm
# // TEST(BasicTests, CreateUpdateAndDeleteDeviceTest) {
# //     print("\tCreating new device...")

# //     Device new_device = BCAPI.createDevice(BASCLOUD_TEST_TENANT_UUID, DBASCLOUD_TEST_CONNECTOR_UUID, "AKS007", "TestDevice", "m3")
# //     print("\t\tOK.")

# //     std::string new_uuid = new_device.uuid

# //     self.assertEqual(new_device.aksID, "AKS007")
# //     self.assertEqual(new_device.description, "TestDevice")
# //     self.assertEqual(new_device.unit, "m3")
# //     self.assertTrue(new_device.createdAt > 0)
# //     self.assertTrue(new_device.updatedAt > 0)

# //     print("\tUpdating newly created device...")

# //     Device up_re_device = BCAPI.updateDevice(BASCLOUD_TEST_TENANT_UUID, new_uuid, "AKS008")
# //     print("\t\tOK.")

# //     print(new_uuid, " - \n", up_re_device.uuid)

# //     self.assertEqual(up_re_device.aksID, "AKS008")

# //     print("\tDeleting newly created device...")

# //     BCAPI.deleteDevice(BASCLOUD_TEST_TENANT_UUID, new_uuid)
# //     print("\t\tOK.")

# //     try {
# //         Device non_dev = BCAPI.getDevice(BASCLOUD_TEST_TENANT_UUID, new_uuid)
# //     } catch(NotFoundRequest& e) {
# //         self.assertTrue(true)
# //     } catch(...) {
# //         self.assertTrue(false)
# //     }

# // }

if __name__ == '__main__':
    authenticate()
    unittest.main()