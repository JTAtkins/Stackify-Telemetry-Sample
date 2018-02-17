#pragma once
#include <string>
#include <map>
#include "Priority.h"
#include "Exception.h"
#include "Message.h"
#include <cpprest/http_client.h> 
#include <pplx/pplxtasks.h>
#include <cpprest/json.h>
//#include <boost/stacktrace.hpp>

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
		Stackify(std::string api_key, std::string application_name, std::string environment);
		~Stackify();

		void Trace(std::string text);
		void Debug(std::string text);
		void Information(std::string text);
		void Notice(std::string text);
		void Warning(std::string text, std::string source_method, int source_line);
		void Error(std::string text, std::string error_message, std::string error_type, std::string source_method, int source_line);
		void Critical(std::string text, std::string error_message, std::string error_type, std::string source_method, int source_line);
		void Fatal(std::string text, std::string error_message, std::string error_type, std::string source_method, int source_line);

		//Todo: Add more overrides?
		void Log(Message& message);
		void Log(Exception& exception);

		//Environment
		void SetEnvironment(std::string env);
		std::wstring GetEnvironment();

		void SetApplicationName(std::string app_name);
		std::wstring GetApplicationName();

		void SetServerVariable(std::string key, std::string value);
		std::wstring GetServerVariable(std::string key);

		bool log_server_variables;
	private:
		web::json::value FormatMessage(Message msg);
		web::json::value FormatException(Exception ex);
		pplx::task<void> PostMessage(std::string api_key, web::json::value message);

		std::string GetLocalComputerName();
		std::string GetOperatingSystemVersion();
		std::string GetWindowsTimeZoneName();
		std::string GetWorkingDirectory();

		std::string api_key_;
		std::string environment_;
		std::string server_name_;
		std::string application_name_;
		std::string location_;
		std::string logger_;
		std::string platform_;

		map<std::string, std::string> server_variables_;
		//pplx::task<void> PostMessage(web::json::object message);

	};
}

