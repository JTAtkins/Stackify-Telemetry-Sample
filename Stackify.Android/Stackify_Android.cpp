#include "Stackify_Android.h"
#include <sys/system_properties.h>


namespace DefenseAgainstTheDarkArts
{
	Stackify_Android::Stackify_Android(std::string api_key)
	{
	}

	Stackify_Android::~Stackify_Android()
	{
	}

	std::string Stackify_Android::GetHostName()
	{

		char host_name_property[PROP_VALUE_MAX + 1];
		//__system_property_get("ro.product.model", model_string);
		__system_property_get("net.hostname", host_name_property);

		std::stringstream host_name;
		host_name << host_name_property;

		return host_name.str();
	}

	std::string Stackify_Android::GetTimezone()
	{
		time_t t = time(nullptr);
		struct tm lt = { 0 };

		localtime_r(&t, &lt);

		return lt.tm_zone;
	}

	std::string Stackify_Android::GetOperatingSystemVersion()
	{
		char os_version_property[PROP_VALUE_MAX + 1];
		__system_property_get("ro.product.model", os_version_property);

		std::stringstream os_version;
		os_version << os_version_property;

		return os_version.str();
	}

	web::json::object Stackify_Android::FormatMessage()
	{
		web::json::value root;
		root[L"Env"] = web::json::value::string(U("Mobile Developer"));
		root[L"ServerName"] = web::json::value::string(U("Mobile"));
		root[L"AppLoc"] = web::json::value::string(web::http::header_names::location);
		root[L"Logger"] = web::json::value::string(U("Mobile feature articles"));
		root[L"Platform"] = web::json::value::string(U("mobile"));

		// Create a JSON message
		web::json::value message;
		message[L"Msg"] = web::json::value::string(U("Developing Android Apps with Scala and Scaloid: Part 2"));
		message[L"Th"] = web::json::value::string(U("Developing Android Apps with Scala and Scaloid: Part 2"));
		message[L"EpochMs"] = web::json::value::string(U("Starting with templates, Android features can be added quickly with a single line of DSL code."));
		message[L"SrcMethod"] = web::json::value::string(U("mobile/developing-android-apps-with-scala-and-s/240162204"));
		message[L"SrcLine"] = web::json::value::string(U("240162204"));

		// Create the messages array
		web::json::value messages;
		messages[0] = message;

		// Assign the items array as the value for the Messages key
		root[L"Msgs"] = messages;

		return root;
	}
}
