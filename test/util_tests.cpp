#include <chrono>
#include <iostream>

#include <fmt/core.h>
#include <fmt/chrono.h>

#include <gtest/gtest.h>

#include "Util.h"
#include "error/Exceptions.h"


TEST(BasicTests, ParseURLParameterTest) {

    std::string url = "test.local/tenants/XXX/connectors/XXX/devices?page[size]=1&page[before]=Mzk5ZTM1MWYtNTI3OS00YzFhLTk0MmUtYTZiODBmMjFiYzVh";

    std::map<std::string, std::string> params = BASCloud::Util::parseURLParameter(url);

    EXPECT_EQ(params.at("page[size]"), "1");
    EXPECT_EQ(params.at("page[before]"), "Mzk5ZTM1MWYtNTI3OS00YzFhLTk0MmUtYTZiODBmMjFiYzVh");
}

TEST(BasicTests, ParseCurrentDateTimeTest) {

    std::time_t currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cerr <<"CURRENT DATETIME: "<<currentDateTime<<std::endl;

    // "2020-03-09T13:09:41.640Z"
    std::string currentDateTimeString = fmt::format("{:%FT%T.000Z}", fmt::localtime(currentDateTime));

    std::cerr <<"CURRENT DATETIME: "<<currentDateTimeString<<std::endl;

    std::time_t parsedDateTime = BASCloud::Util::parseDateTimeString(currentDateTimeString);
    std::string parsedDateTimeString = fmt::format("{:%FT%T.000Z}", fmt::localtime(parsedDateTime));

    std::cerr <<"PARSED DATETIME: "<<parsedDateTime<<std::endl;
    std::cerr <<"PARSED DATETIME: "<<parsedDateTimeString<<std::endl;

    EXPECT_EQ(currentDateTime, parsedDateTime);
    EXPECT_EQ(currentDateTimeString, parsedDateTimeString);
}

TEST(BasicTests, ParseFixedDateTimeTest) {

    std::string fixedDateTimeString = "2019-06-02T12:17:58.819Z";

    std::cerr <<"FIXED DATETIME: "<<fixedDateTimeString<<std::endl;

    std::time_t parsedDateTime = BASCloud::Util::parseDateTimeString(fixedDateTimeString);
    std::string parsedDateTimeString = fmt::format("{:%FT%T.819Z}", fmt::localtime(parsedDateTime));

    std::cerr <<"PARSED DATETIME: "<<parsedDateTime<<std::endl;
    std::cerr <<"PARSED DATETIME: "<<parsedDateTimeString<<std::endl;

    EXPECT_EQ(fixedDateTimeString, parsedDateTimeString);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}