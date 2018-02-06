#pragma once

#include "Priority.h"
#include "Exception.h"
#include "Message.h"
#include <cpprest/http_client.h> 
#include <pplx/pplxtasks.h>
#include <cpprest/json.h>

namespace DefenseAgainstTheDarkArts
{
	class Stackify_Android
	{
	public:
		Stackify_Android(std::string api_key);
		~Stackify_Android();

		std::string environment;
		std::string server_name;
		std::string app_name;
		std::string location;
		std::string platform;


		void log(std::string message, Priority priority);
		void log(Message& message);
		//void log(Exception& exception);

		//You can set an a number of other properties by key/value pair that will be logged as 
		//Server Variables on your message.  Put whatever you want here.
		void setProperty(std::string key, std::string value);
		void getProperty(std::string key);

		
		static std::string GetUserName();
		static web::json::object FormatMessage();
		
	private:
		std::string api_key_;
		std::map<std::string, std::string> property_map_;

		static std::string GetHostName();
		static std::string GetTimezone();
		static std::string GetOperatingSystemVersion();
		
		pplx::task<int> PostMessage(web::json::object message);
	};
}
