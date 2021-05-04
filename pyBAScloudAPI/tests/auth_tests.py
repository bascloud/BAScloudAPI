import unittest
import time 
import datetime

from definitions import *

import pyBAScloudAPI as api


class TestSuccessfullAuthentication(unittest.TestCase):

    def test_auth(self):

        currentDateTime = int(time.time())
        print("CURRENT DATETIME: ", currentDateTime)

        currentDateTimeString = datetime.datetime.fromtimestamp(currentDateTime).strftime("%FT%T.000Z")
        print("CURRENT DATETIME STR: ", currentDateTimeString)

        BCAPI = api.EntityContext(BASCLOUD_TEST_URL)

        BCAPI.authenticateWithUserLogin(BASCLOUD_TEST_EMAIL, BASCLOUD_TEST_PASS)

        print("TOKEN: ", BCAPI.getToken())
        print("EXPIRATION DATE: ", datetime.datetime.fromtimestamp(BCAPI.getTokenExpirationDate()).strftime("%FT%T.000Z"))

        self.assertEqual(BCAPI.isAuthenticated(), True, "APi should be authenticated.")
        self.assertTrue(BCAPI.getTokenExpirationDate() > currentDateTime, "Expiration date should be in the future.")
        self.assertTrue(len(BCAPI.getToken()) > 0, "Token should be not empty.")


class TestFailedAuthentication(unittest.TestCase):

    def test_auth(self):
        # Invalid authentication should return unauthorized by the api and throw exception accordingly
        BCAPI = api.EntityContext(BASCLOUD_TEST_URL)

        try:
            BCAPI.authenticateWithUserLogin("test@test.de", "test")
        except api.UnauthorizedRequest:
            self.assertEqual(BCAPI.isAuthenticated(), False, "API should not be authenticated.")
            self.assertTrue(BCAPI.getTokenExpirationDate() == -1, "Expiration date should be invalid.")
            self.assertTrue(len(BCAPI.getToken()) == 0, "Token should be empty.")
        except:
            raise


class TestFailedConnection(unittest.TestCase):

    def test_auth(self):
        # Invalid authentication should return unauthorized by the api and throw exception accordingly
        BCAPI = api.EntityContext("https://test.local")

        try:
            BCAPI.authenticateWithUserLogin("test@test.de", "test")
        except api.ConnectionError:
            self.assertEqual(BCAPI.isAuthenticated(), False, "API should not be authenticated.")
            self.assertTrue(BCAPI.getTokenExpirationDate() == -1, "Expiration date should be invalid.")
            self.assertTrue(len(BCAPI.getToken()) == 0, "Token should be empty.")
        except:
            raise


if __name__ == '__main__':
    unittest.main()