#include <string>
#include <boost/test/unit_test.hpp>
#include <Settings.hpp>


BOOST_AUTO_TEST_SUITE(SettingsTests)


BOOST_AUTO_TEST_CASE(settingsReading) {
	string settingsAsString =
		"key1=value1\n"
		"key2= value2\n"
		"key3 =value3\n"
		"key4 = value4";

	Settings settings;
	settings.read(settingsAsString);

	BOOST_CHECK_EQUAL(settings.get<string>("key1", ""), "value1");
	BOOST_CHECK_EQUAL(settings.get<string>("key2", ""), "value2");
	BOOST_CHECK_EQUAL(settings.get<string>("key3", ""), "value3");
	BOOST_CHECK_EQUAL(settings.get<string>("key4", ""), "value4");
}


BOOST_AUTO_TEST_CASE(settingsConversion) {
	Settings settings;
	settings.set("bool", true);
	settings.set("int", static_cast<int64_t>(-1));
	settings.set("uint", static_cast<uint64_t>(1u));
	settings.set("float", 0.5);
	settings.set("string", "value");

	BOOST_CHECK_EQUAL(settings.get<bool>("bool", false), true);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("bool", -1), -1);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("bool", -1), -1);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("bool", -1), -1);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("bool", -1), -1);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("bool", 1), 1);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("bool", 1), 1);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("bool", 1), 1);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("bool", 1), 1);
	BOOST_CHECK_EQUAL(settings.get<double>("bool", 0.5), 0.5);
	BOOST_CHECK_EQUAL(settings.get<double>("bool", 0.5), 0.5);
	BOOST_CHECK_EQUAL(settings.get<string>("bool", ""), "true");

	BOOST_CHECK_EQUAL(settings.get<bool>("int", false), false);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("int", 0), -1);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("int", 0), -1);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("int", 0), -1);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("int", 0), -1);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("int", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("int", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("int", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("int", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<double>("int", 0.0), -1.0);
	BOOST_CHECK_EQUAL(settings.get<double>("int", 0.0), -1.0);
	BOOST_CHECK_EQUAL(settings.get<string>("int", ""), "-1");

	BOOST_CHECK_EQUAL(settings.get<bool>("uint", false), true);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("uint", 0), 1);
	BOOST_CHECK_EQUAL(settings.get<double>("uint", 0), 1.0);
	BOOST_CHECK_EQUAL(settings.get<double>("uint", 0), 1.0);
	BOOST_CHECK_EQUAL(settings.get<string>("uint", ""), "1");

	BOOST_CHECK_EQUAL(settings.get<bool>("float", true), true);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("float", 0), 0);
	BOOST_CHECK_EQUAL(settings.get<double>("float", 0.5), 0.5);
	BOOST_CHECK_EQUAL(settings.get<double>("float", 0.5), 0.5);
	BOOST_CHECK_EQUAL(settings.get<string>("float", ""), "0.5");

	BOOST_CHECK_EQUAL(settings.get<bool>("undef", true), true);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("string", -3), -3);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("string", -3), -3);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("string", -3), -3);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("string", -3), -3);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("string", 3), 3);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("string", 3), 3);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("string", 3), 3);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("string", 3), 3);
	BOOST_CHECK_EQUAL(settings.get<double>("string", 2.5), 2.5);
	BOOST_CHECK_EQUAL(settings.get<double>("string", 2.5), 2.5);
	BOOST_CHECK_EQUAL(settings.get<string>("string", ""), "value");

	BOOST_CHECK_EQUAL(settings.get<bool>("undef", true), true);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("undef", -2), -2);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("undef", -2), -2);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("undef", -2), -2);
	BOOST_CHECK_EQUAL(settings.get<int64_t>("undef", -2), -2);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("undef", 2), 2);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("undef", 2), 2);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("undef", 2), 2);
	BOOST_CHECK_EQUAL(settings.get<uint64_t>("undef", 2), 2);
	BOOST_CHECK_EQUAL(settings.get<double>("undef", 1.5), 1.5);
	BOOST_CHECK_EQUAL(settings.get<double>("undef", 1.5), 1.5);
	BOOST_CHECK_EQUAL(settings.get<string>("undef", "alt"), "alt");
}


BOOST_AUTO_TEST_CASE(settingsWriting) {
	Settings settings;
	settings.set("key2", static_cast<int64_t>(-1));
	settings.set("key4", 1.0);
	settings.set("key1", "value");
	settings.set("key3", static_cast<uint64_t>(1));
	settings.set("key5", true);

	string settingsAsString;
	settings.write(settingsAsString);

	BOOST_CHECK_EQUAL(settingsAsString,
					  "key1=value\n"
					  "key2=-1\n"
					  "key3=1\n"
					  "key4=1\n"
					  "key5=true\n");
}


BOOST_AUTO_TEST_SUITE_END()
