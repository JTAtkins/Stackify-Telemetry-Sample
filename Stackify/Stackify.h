#pragma once
#include <string>
#include "Priority.h"
#include "Exception.h"
#include "Message.h"
#include <cpprest/http_client.h> 
#include <ppltasks.h>
#include <cpprest/json.h>
#include <boost/stacktrace.hpp>

//This is just here from a macro/naming conflict
#ifdef FormatMessage
#undef FormatMessage
#endif

#ifdef ERROR
#undef ERROR
#endif

namespace DefenseAgainstTheDarkArts
{
	class Stackify
	{
	public:
		Stackify(std::wstring api_key, std::wstring application_name, std::wstring environment);
		~Stackify();

		void Log(std::wstring message, Priority priority);
		void Log(Message& message);
		void Log(Exception& exception);

		//Environment
		void SetEnvironment(std::wstring env);
		std::wstring GetEnvironment();

		void SetApplicationName(std::wstring app_name);
		std::wstring GetApplicationName();

		void SetServerVariable(std::wstring key, std::wstring value);
		std::wstring GetServerVariable(std::wstring key);

		web::json::value FormatMessage(Message msg);
		web::json::value FormatException(Exception ex);
		pplx::task<void> PostMessage(std::wstring api_key, web::json::value message);

		bool log_server_variables;
	private:
		

		std::wstring GetLocalComputerName();
		std::wstring GetOperatingSystemVersion();
		std::string GetWindowsTimeZoneName();
		std::wstring GetWorkingDirectory();

		std::wstring api_key_;
	    std::wstring environment_;
		std::wstring server_name_;
		std::wstring application_name_;
		std::wstring location_;
		std::wstring logger_;
		std::wstring platform_;
		
		std::map<std::wstring, std::wstring> server_variables_;
		//pplx::task<void> PostMessage(web::json::object message);
		
	};
}


