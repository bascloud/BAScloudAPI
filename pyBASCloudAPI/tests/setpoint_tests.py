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



class TestSetPointCollection(unittest.TestCase):

    def test_setpoint(self):
        print("\tRequesting all setpoints...")

        setpoints = BCAPI.getSetPointsCollection(BASCLOUD_TEST_TENANT_UUID, errorHandler=errorHandler)
        print("\t\tOK.")

        print("\tFound setpoints: ", len(setpoints[0]))

        self.assertTrue(len(setpoints[0]) >= 0) # may no set points available


class TestSingleSetPoint(unittest.TestCase):

    def test_setpoint(self):
        print("\tRequesting single setpoint with UUID...")

        setpoint = BCAPI.getSetPoint(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_STEPOINT_UUID)
        print("\t\tOK.")

        print("\tSetpoint UUID: ", setpoint.uuid)
        print("\tSetpoint Value: ", setpoint.value)
        print("\tSetpoint timestamp: ", datetime.datetime.fromtimestamp(setpoint.timestamp).strftime("%FT%T.000Z"))
        print("\tSetpoint created at: ", datetime.datetime.fromtimestamp(setpoint.createdAt).strftime("%FT%T.000Z"))
        print("\tSetpoint updated at: ", datetime.datetime.fromtimestamp(setpoint.updatedAt).strftime("%FT%T.000Z"))

        self.assertEqual(setpoint.uuid, BASCLOUD_TEST_STEPOINT_UUID)
        self.assertEqual(setpoint.value, 2345.67)
        self.assertTrue(setpoint.timestamp > 0)
        self.assertTrue(setpoint.createdAt > 0)
        self.assertTrue(setpoint.updatedAt > 0)


class TestCreateAndDeleteSetPoint(unittest.TestCase):

    def test_setpoint(self):
        currentDateTime = int(time.time())

        new_set = BCAPI.createSetPoint(BASCLOUD_TEST_TENANT_UUID, BASCLOUD_TEST_DEVICE_UUID, 2345.67, currentDateTime)
        print("\t\tOK.")

        new_uuid = new_set.uuid
        print("\t\tUUID: ", new_uuid)

        self.assertEqual(new_set.value, 2345.67)
        self.assertEqual(new_set.timestamp, currentDateTime)
        # self.assertTrue(new_set.createdAt-(2*60*60) >= currentDateTime) // TODO ML: this is wrong at the moment, for some reason the backend returns timestamps -2hours
        # self.assertTrue(new_set.updatedAt-(2*60*60) >= currentDateTime)

        print("\tAgain Requesting all setpoints...") # Thus deactivating/inactivating them

        print("\tFrom: ", datetime.datetime.fromtimestamp(currentDateTime-(60*60*24)).strftime("%FT%T.000Z"))
        print("\tUntil: ", datetime.datetime.fromtimestamp(currentDateTime+(60*60*24)).strftime("%FT%T.000Z"))

        setpoints = BCAPI.getSetPointsCollection(BASCLOUD_TEST_TENANT_UUID, from_=currentDateTime-(60*60*24), until=currentDateTime+(60*60*24), errorHandler=errorHandler)
        print("\t\tOK.")

        print("\tFound setpoints: ", len(setpoints[0]))

        self.assertTrue(len(setpoints[0]) >= 1) # may no set points available


if __name__ == '__main__':
    authenticate()
    unittest.main()