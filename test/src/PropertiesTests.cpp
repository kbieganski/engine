#include <string>
#include <boost/test/unit_test.hpp>
#include <Properties.hpp>


BOOST_AUTO_TEST_SUITE(PropertiesTests)


BOOST_AUTO_TEST_CASE(propertiesReading) {
	string propertiesAsString =
		"key1=value1\n"
		"key2= value2\n"
		"key3 =value3\n"
		"key4 = value4";

	Properties properties;
	properties.read(propertiesAsString);

	BOOST_CHECK_EQUAL(properties.get<string>("key1", ""), "value1");
	BOOST_CHECK_EQUAL(properties.get<string>("key2", ""), "value2");
	BOOST_CHECK_EQUAL(properties.get<string>("key3", ""), "value3");
	BOOST_CHECK_EQUAL(properties.get<string>("key4", ""), "value4");
}


BOOST_AUTO_TEST_CASE(propertiesConversion) {
	Properties properties;
	properties.set("bool", true);
	properties.set("int", static_cast<int64_t>(-1));
	properties.set("uint", static_cast<uint64_t>(1u));
	properties.set("float", 0.5);
	properties.set("string", "value");

	BOOST_CHECK_EQUAL(properties.get<bool>("bool", false), true);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("bool", -1), -1);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("bool", -1), -1);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("bool", -1), -1);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("bool", -1), -1);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("bool", 1), 1);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("bool", 1), 1);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("bool", 1), 1);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("bool", 1), 1);
	BOOST_CHECK_EQUAL(properties.get<double>("bool", 0.5), 0.5);
	BOOST_CHECK_EQUAL(properties.get<double>("bool", 0.5), 0.5);
	BOOST_CHECK_EQUAL(properties.get<string>("bool", ""), "true");

	BOOST_CHECK_EQUAL(properties.get<bool>("int", false), false);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("int", 0), -1);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("int", 0), -1);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("int", 0), -1);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("int", 0), -1);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("int", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("int", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("int", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("int", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<double>("int", 0.0), -1.0);
	BOOST_CHECK_EQUAL(properties.get<double>("int", 0.0), -1.0);
	BOOST_CHECK_EQUAL(properties.get<string>("int", ""), "-1");

	BOOST_CHECK_EQUAL(properties.get<bool>("uint", false), true);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(properties.get<double>("uint", 0), 1.0);
	BOOST_CHECK_EQUAL(properties.get<double>("uint", 0), 1.0);
	BOOST_CHECK_EQUAL(properties.get<string>("uint", ""), "1");

	BOOST_CHECK_EQUAL(properties.get<bool>("float", true), true);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(properties.get<double>("float", 0.5), 0.5);
	BOOST_CHECK_EQUAL(properties.get<double>("float", 0.5), 0.5);
	BOOST_CHECK_EQUAL(properties.get<string>("float", ""), "0.5");

	BOOST_CHECK_EQUAL(properties.get<bool>("undef", true), true);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("string", -3), -3);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("string", -3), -3);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("string", -3), -3);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("string", -3), -3);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("string", 3), 3);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("string", 3), 3);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("string", 3), 3);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("string", 3), 3);
	BOOST_CHECK_EQUAL(properties.get<double>("string", 2.5), 2.5);
	BOOST_CHECK_EQUAL(properties.get<double>("string", 2.5), 2.5);
	BOOST_CHECK_EQUAL(properties.get<string>("string", ""), "value");

	BOOST_CHECK_EQUAL(properties.get<bool>("undef", true), true);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("undef", -2), -2);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("undef", -2), -2);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("undef", -2), -2);
	BOOST_CHECK_EQUAL(properties.get<int64_t>("undef", -2), -2);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("undef", 2), 2);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("undef", 2), 2);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("undef", 2), 2);
	BOOST_CHECK_EQUAL(properties.get<uint64_t>("undef", 2), 2);
	BOOST_CHECK_EQUAL(properties.get<double>("undef", 1.5), 1.5);
	BOOST_CHECK_EQUAL(properties.get<double>("undef", 1.5), 1.5);
	BOOST_CHECK_EQUAL(properties.get<string>("undef", "alt"), "alt");
}


BOOST_AUTO_TEST_CASE(propertiesWriting) {
	Properties properties;
	properties.set("key2", static_cast<int64_t>(-1));
	properties.set("key4", 1.0);
	properties.set("key1", "value");
	properties.set("key3", static_cast<uint64_t>(1));
	properties.set("key5", true);

	string propertiesAsString;
	properties.write(propertiesAsString);

	BOOST_CHECK_EQUAL(propertiesAsString,
					  "key1=value\n"
					  "key2=-1\n"
					  "key3=1\n"
					  "key4=1\n"
					  "key5=true\n");
}


BOOST_AUTO_TEST_SUITE_END()
