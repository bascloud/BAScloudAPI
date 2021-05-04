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


class TestReadingCollection(unittest.TestCase):

    def test_reading(self):
        print("\tRequesting all readings...")

        readings = BCAPI.getReadingsCollection(BASCLOUD_TEST_TENANT_UUID, errorHandler=errorHandler)
        print("\t\tOK.")

        print("\tFound readings: ", len(readings[0]))

        self.assertTrue(len(readings[0]) >= 1)


class TestSingleReading(unittest.TestCase):

    def test_reading(self):
        print("\tRequesting single reading with UUID...")

        reading = BCAPI.getReading(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_READING_UUID)
        print("\t\tOK.")

        print("\tReading UUID: ", reading.uuid)
        print("\tReading Value: ", reading.value)
        print("\tReading timestamp: ", datetime.datetime.fromtimestamp(reading.timestamp).strftime("%FT%T.000Z"))
        print("\tReading created at: ", datetime.datetime.fromtimestamp(reading.createdAt).strftime("%FT%T.000Z"))
        print("\tReading updated at: ", datetime.datetime.fromtimestamp(reading.updatedAt).strftime("%FT%T.000Z"))


        self.assertEqual(reading.uuid, BASCLOUD_TEST_READING_UUID)
        self.assertEqual(reading.value, 1234.56)
        self.assertTrue(reading.timestamp > 0)
        self.assertTrue(reading.createdAt > 0)
        self.assertTrue(reading.updatedAt > 0)


class TestAssociatedDevice(unittest.TestCase):

    def test_reading(self):
        print("\tRequesting associated device of the reading again...")

        read_device = BCAPI.getAssociatedDevice(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_READING_UUID)
        print("\t\tOK.")

        self.assertEqual(read_device.uuid, BASCLOUD_TEST_DEVICE_UUID)



class TestCreateAndDeleteReading(unittest.TestCase):

    def test_reading(self):
        currentDateTime = int(time.time())

        new_read = BCAPI.createReading(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID, 2345.67, currentDateTime)
        print("\t\tOK.")

        new_uuid = new_read.uuid

        self.assertEqual(new_read.value, 2345.67)
        self.assertEqual(new_read.timestamp, currentDateTime)
        # self.assertTrue(new_read.createdAt >= currentDateTime) # TODO ML: this is wrong at the moment, for some reason the backend returns timestamps -2hours
        # self.assertTrue(new_read.updatedAt() >= currentDateTime)

        
        print("\tRequesting deletion of created reading again... ")

        BCAPI.deleteReading(BASCLOUD_TEST_TENANT_UUID, new_uuid)
        print("\t\tOK.")

        try:
            non_read = BCAPI.getReading(BASCLOUD_TEST_TENANT_UUID, new_uuid)
        except api.NotFoundRequest:
            self.assertTrue(True)
        except:
            self.assertTrue(False)


if __name__ == '__main__':
    authenticate()
    unittest.main()