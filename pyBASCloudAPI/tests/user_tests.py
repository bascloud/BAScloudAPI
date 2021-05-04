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



class TestUserCollection(unittest.TestCase):

    def test_user(self):
        print("\tRequesting all users...")
        users = BCAPI.getUsersCollection(errorHandler=errorHandler)
        self.assertTrue(len(users[0]) >= 1)


class TestSingleUser(unittest.TestCase):

    def test_user(self):
        print("\tRequesting single users with UUID...")

        currentUser = BCAPI.getUser(BASCLOUD_TEST_USER_UUID)
        print("\t\tOK.")

        print("\tUser UUID: ", currentUser.uuid)
        print("\tUser Email: ", currentUser.email)
        print("\tUser created at: ", datetime.datetime.fromtimestamp(currentUser.createdAt).strftime("%FT%T.000Z"))
        print("\tUser updated at: ", datetime.datetime.fromtimestamp(currentUser.updatedAt).strftime("%FT%T.000Z"))

        self.assertEqual(currentUser.uuid, BASCLOUD_TEST_USER_UUID)
        self.assertEqual(currentUser.email, BASCLOUD_TEST_EMAIL)
        self.assertTrue(currentUser.createdAt > 0)
        self.assertTrue(currentUser.updatedAt > 0)


# // Need the authority to delete it again
# // TEST(BasicTests, CreateUserTest) {

# //     print("\tCreating a new user...")

# //     User new_user = BCAPI.createNewUser("lode@accessec.com", "12346")
# //     print("\t\tOK.")

# //     self.assertEqual(new_user.uuid.length(), 36)
# //     self.assertEqual(new_user.email, "lode@accessec.com")
# //     self.assertTrue(new_user.createdAt > 0)
# //     self.assertTrue(new_user.updatedAt > 0)
# // }

class TestAssociatedTenantTest(unittest.TestCase):

    def test_user(self):
        print("\tRequesting associated tenant...")

        assoc_tenant = BCAPI.getAssociatedTenant(BASCLOUD_TEST_USER_UUID)
        print("\t\tOK.")

        self.assertEqual(assoc_tenant.uuid, BASCLOUD_TEST_TENANT_UUID)
        self.assertEqual(assoc_tenant.name, "testing")
        self.assertEqual(assoc_tenant.urlName, "testing")
        self.assertTrue(assoc_tenant.createdAt > 0)
        self.assertTrue(assoc_tenant.updatedAt > 0)


if __name__ == '__main__':
    authenticate()
    unittest.main()