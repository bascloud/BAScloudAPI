import unittest
import time 
import datetime

import pyBAScloudAPI as api


class TestParseURLParameter(unittest.TestCase):

    def test_parsing(self):
        url = "test.local/tenants/XXX/connectors/XXX/devices?page[size]=1&page[before]=Mzk5ZTM1MWYtNTI3OS00YzFhLTk0MmUtYTZiODBmMjFiYzVh";

        params = api.Util.parseURLParameter(url)

        self.assertEqual(int(params["page[size]"]), 1, "Page[size] should be 1")
        self.assertEqual(params["page[before]"], "Mzk5ZTM1MWYtNTI3OS00YzFhLTk0MmUtYTZiODBmMjFiYzVh", "page[before] should be Mzk5ZTM1MWYtNTI3OS00YzFhLTk0MmUtYTZiODBmMjFiYzVh")


class TestParseCurrentDateTime(unittest.TestCase):

    def test_parsing(self):
            
        currentDateTime = int(time.time())

        print("CURRENT DATETIME: ", currentDateTime)

        currentDateTimeString = datetime.datetime.fromtimestamp(currentDateTime).strftime("%FT%T.000Z")

        print("CURRENT DATETIME STR: ", currentDateTimeString)

        parsedDateTime = api.Util.parseDateTimeString(currentDateTimeString)
        parsedDateTimeString = datetime.datetime.fromtimestamp(parsedDateTime).strftime("%FT%T.000Z")

        print("PARSED DATETIME: ", parsedDateTime)
        print("PARSED DATETIME STR: ", parsedDateTimeString)

        self.assertEqual(currentDateTime, parsedDateTime, "Timestamps should be equal")
        self.assertEqual(currentDateTimeString, parsedDateTimeString, "Dates should be equal")


class TestParseFixedDateTime(unittest.TestCase):

    def test_parsing(self):
            
        fixedDateTimeString = "2019-06-02T12:17:58.819Z"

        print("FIXED DATETIME STR: ", fixedDateTimeString)

        parsedDateTime = api.Util.parseDateTimeString(fixedDateTimeString)
        parsedDateTimeString = datetime.datetime.fromtimestamp(parsedDateTime).strftime("%FT%T.819Z")

        print("PARSED DATETIME: ", parsedDateTime)
        print("PARSED DATETIME STR: ", parsedDateTimeString)

        self.assertEqual(fixedDateTimeString, parsedDateTimeString, "Dates should be equal")

if __name__ == '__main__':
    unittest.main()