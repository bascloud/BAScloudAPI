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


class TestTenantCollection(unittest.TestCase):

    def test_tenant(self):

        print("\tRequesting all tenants...")

        tenants = BCAPI.getTenantsCollection(errorHandler=errorHandler)
        print("\t\tOK.")

        print("\tFound tenants: ", len(tenants[0]))

        self.assertTrue(len(tenants[0]) >= 1)


class TestSingleTenant(unittest.TestCase):

    def test_tenant(self):
        print("\tRequesting single tenant with UUID...")

        tenant = BCAPI.getTenant(BASCLOUD_TEST_TENANT_UUID)
        print("\t\tOK.")

        print("\tTenant UUID: ", tenant.uuid)
        print("\tTenant Name: ", tenant.name)
        print("\tTenant URL Name: ", tenant.urlName)
        print("\tTenant created at: ", datetime.datetime.fromtimestamp(tenant.createdAt).strftime("%FT%T.000Z"))
        print("\tTenant updated at: ", datetime.datetime.fromtimestamp(tenant.updatedAt).strftime("%FT%T.000Z"))

        self.assertEqual(tenant.uuid, BASCLOUD_TEST_TENANT_UUID)
        self.assertEqual(tenant.name, "testing")
        self.assertEqual(tenant.urlName, "tenant_8eda44f3a00d426fbe17d3260aaec0ed")
        self.assertTrue(tenant.createdAt > 0)
        self.assertTrue(tenant.updatedAt > 0)



class TestAssociatedUser(unittest.TestCase):

    def test_tenant(self):
        print("\tRequesting associated users of tenant...")

        tenant_user = BCAPI.getAssociatedUsers(BASCLOUD_TEST_TENANT_UUID, errorHandler=errorHandler)
        print("\t\tOK.")

        self.assertTrue(len(tenant_user[0]) >= 1)



if __name__ == '__main__':
    authenticate()
    unittest.main()