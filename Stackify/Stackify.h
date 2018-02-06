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

		void Trace(std::wstring text);
		void Debug(std::wstring text);
		void Information(std::wstring text);
		void Notice(std::wstring text);
		void Warning(std::wstring text, std::wstring source_method, int source_line);
		void Error(std::wstring text, std::wstring error_message, std::wstring error_type, std::wstring source_method, int source_line);
		void Critical(std::wstring text, std::wstring error_message, std::wstring error_type, std::wstring source_method, int source_line);
		void Fatal(std::wstring text, std::wstring error_message, std::wstring error_type, std::wstring source_method, int source_line);

		//Todo: Add more overrides?
		void Log(Message& message);
		void Log(Exception& exception);

		//Environment
		void SetEnvironment(std::wstring env);
		std::wstring GetEnvironment();

		void SetApplicationName(std::wstring app_name);
		std::wstring GetApplicationName();

		void SetServerVariable(std::wstring key, std::wstring value);
		std::wstring GetServerVariable(std::wstring key);

		bool log_server_variables;
	private:
		web::json::value FormatMessage(Message msg);
		web::json::value FormatException(Exception ex);
		pplx::task<void> PostMessage(std::wstring api_key, web::json::value message);

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


