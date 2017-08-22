#include <string>
#include <boost/test/unit_test.hpp>
#include <StringUtility.hpp>


BOOST_AUTO_TEST_SUITE(StringUtilityTests)


BOOST_AUTO_TEST_CASE(stringTrimming) {
	string trimmedString = "trimmed\t \nstring";
	string testString = " \t\n" + trimmedString + " \n\t";
	BOOST_CHECK_EQUAL(StringUtility::trim(testString), trimmedString);
}


BOOST_AUTO_TEST_CASE(stringConversion) {
	double value1 = 0.0;
	double value2 = 0.1;
	double value3 = 1.0;
	double value4 = 1.1;
	double value5 = -1.0;
	BOOST_CHECK_EQUAL(StringUtility::convertToString(value1), "0");
	BOOST_CHECK_EQUAL(StringUtility::convertToString(value2), "0.1");
	BOOST_CHECK_EQUAL(StringUtility::convertToString(value3), "1");
	BOOST_CHECK_EQUAL(StringUtility::convertToString(value4), "1.1");
	BOOST_CHECK_EQUAL(StringUtility::convertToString(value5), "-1");

}


BOOST_AUTO_TEST_SUITE_END()
