#include "Stackify.h"
#include <string>
#include <iostream>
#include <chrono>

using namespace std::chrono;

namespace DefenseAgainstTheDarkArts
{
	Stackify::Stackify(std::string api_key, std::string application_name, std::string environment)
	{
		api_key_ = api_key;
		environment_ = environment;
		application_name_ = application_name;
		platform_ = "cpp";
		logger_ = "defense-against-the-dark-arts-stackify-logger-pococpp-1.0.0.0";
		server_name_ = GetLocalComputerName();
		location_ = GetWorkingDirectory();

		//Sets whether server variables will be logged as data on every message.  Otherwise Server variables are only logged on exceptions
		//Server variables are kvp of misc data that you can set
		log_server_variables = false;
	}

	Stackify::~Stackify()
	{
	}

	void Stackify::Trace(std::string text)
	{
		Message msg;
		msg.text = text;
		msg.priority = TRACE;

		web::json::value json = FormatMessage(msg);
		PostMessage(api_key_, json);
	}

	void Stackify::Debug(std::string text)
	{
		Message msg;
		msg.text = text;
		msg.priority = DEBUG;

		web::json::value json = FormatMessage(msg);
		PostMessage(api_key_, json);
	}

	void Stackify::Information(std::string text)
	{
		Message msg;
		msg.text = text;
		msg.priority = INFORMATION;

		web::json::value json = FormatMessage(msg);
		PostMessage(api_key_, json);
	}

	void Stackify::Notice(std::string text)
	{
		Message msg;
		msg.text = text;
		msg.priority = NOTICE;

		web::json::value json = FormatMessage(msg);
		PostMessage(api_key_, json);
	}

	void Stackify::Warning(std::string text, std::string source_method, int source_line)
	{
		Message msg;
		msg.text = text;
		msg.priority = WARNING;
		msg.source_method = source_method;
		msg.source_line = source_line;

		web::json::value json = FormatMessage(msg);
		PostMessage(api_key_, json);
	}

	void Stackify::Error(std::string text, std::string error_message, std::string error_type, std::string source_method, int source_line)
	{
		Exception msg;
		msg.text = text;
		msg.priority = ERROR;
		msg.source_method = source_method;
		msg.source_line = source_line;
		msg.error_message = error_message;
		msg.error_type = error_type;

		web::json::value json = FormatException(msg);
		PostMessage(api_key_, json);
	}

	void Stackify::Critical(std::string text, std::string error_message, std::string error_type, std::string source_method, int source_line)
	{
		Exception msg;
		msg.text = text;
		msg.priority = CRITICAL;
		msg.source_method = source_method;
		msg.source_line = source_line;
		msg.error_message = error_message;
		msg.error_type = error_type;

		web::json::value json = FormatException(msg);
		PostMessage(api_key_, json);
	}

	void Stackify::Fatal(std::string text, std::string error_message, std::string error_type, std::string source_method, int source_line)
	{
		Exception msg;
		msg.text = text;
		msg.priority = FATAL;
		msg.source_method = source_method;
		msg.source_line = source_line;
		msg.error_message = error_message;
		msg.error_type = error_type;

		web::json::value json = FormatException(msg);
		PostMessage(api_key_, json);
	}

	void Stackify::Log(Message& message)
	{
		web::json::value json = FormatMessage(message);
		PostMessage(api_key_, json);
	}

	void Stackify::Log(Exception& exception)
	{
		web::json::value json = FormatException(exception);
		PostMessage(api_key_, json);
	}

	web::json::value Stackify::FormatMessage(Message msg)
	{
		json::value root;
		root["Env"] = json::value::string(environment_);
		root["ServerName"] = json::value::string(server_name_);
		root["AppName"] = json::value::string(application_name_);
		root["AppLoc"] = json::value::string(location_);
		root["Logger"] = json::value::string(logger_);
		root["Platform"] = json::value::string(platform_);

		// Create a JSON message
		json::value message;
		message["Msg"] = json::value::string(msg.text);
		message["Th"] = json::value::string(msg.thread_name);

		switch (msg.priority) {
		case TRACE:
			message["Level"] = json::value::string("Trace");
			break;
		case DEBUG:
			message["Level"] = json::value::string("Debug");
			break;
		case INFORMATION:
			message["Level"] = json::value::string("Information");
			break;
		case NOTICE:
			message["Level"] = json::value::string("Notice");
			break;
		case WARNING:
			message["Level"] = json::value::string("Warning");
			break;
		case ERROR:
			message["Level"] = json::value::string("Error");
			break;
		case CRITICAL:
			message["Level"] = json::value::string("Critical");
			break;
		case FATAL:
			message["Level"] = json::value::string("Fatal");
			break;
		}

		//Get Epoch Milliseconds
		milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		message[L"EpochMs"] = json::value::number(ms.count());

		message[L"SrcMethod"] = json::value::string(msg.source_method);
		message[L"SrcLine"] = json::value::number(msg.source_line);

		//Populate Data Values
		std::wstringstream data;
		for (auto it = msg.data_map.begin(); it != msg.data_map.end(); ++it)
		{
			//Example: Long string containing "Key1":"Value1", "Key2" : "Value2"
			//Todo: Escape special characters
			data << it->first;
			data << ":";
			data << it->second;

			//Check  if it is the last element otherwise add a comma
			if (++it != msg.data_map.end())
			{
				data << ",";
			}
			else
			{
				//If it is the last element check to see if server variables should be appended to the message as data
				if (log_server_variables)
				{
					if (!server_variables_.size() > 0)
					{
						//Has values so add a comma
						data << ",";
					}
				}
			}
		}

		//Append server variables to data
		if (log_server_variables)
		{
			for (auto it = server_variables_.begin(); it != server_variables_.end(); ++it)
			{
				//Nothing in data so append as normal
				data << it->first;
				data << ":";
				data << it->second;

				//Check  if it is the last element
				if (++it != server_variables_.end())
				{
					data << ",";
				}
			}
		}

		if (!data.str().empty())
		{
			message[L"Data"] = json::value::string(data.str());
		}

		// Create the messages array
		json::value messages;
		messages[0] = message;

		// Assign the items array as the value for the Messages key
		root[L"Msgs"] = messages;

		return root;
	}

	web::json::value Stackify::FormatException(Exception ex)
	{
		json::value root;
		root[L"Env"] = json::value::string(environment_);
		root[L"ServerName"] = json::value::string(server_name_);
		root[L"AppName"] = json::value::string(application_name_);
		root[L"AppLoc"] = json::value::string(location_);
		root[L"Logger"] = json::value::string(U("defense-against-the-dark-arts-stackify-logger-pococpp-1.0.0.0"));
		root[L"Platform"] = json::value::string(U("cpp"));

		// Create a JSON message
		json::value message;
		message[L"Msg"] = json::value::string(ex.text);
		message[L"Th"] = json::value::string(ex.thread_name);

		switch (ex.priority) {
		case TRACE:
			message[L"Level"] = json::value::string(L"Trace");
			break;
		case DEBUG:
			message[L"Level"] = json::value::string(L"Debug");
			break;
		case INFORMATION:
			message[L"Level"] = json::value::string(L"Information");
			break;
		case NOTICE:
			message[L"Level"] = json::value::string(L"Notice");
			break;
		case WARNING:
			message[L"Level"] = json::value::string(L"Warning");
			break;
		case ERROR:
			message[L"Level"] = json::value::string(L"Error");
			break;
		case CRITICAL:
			message[L"Level"] = json::value::string(L"Critical");
			break;
		case FATAL:
			message[L"Level"] = json::value::string(L"Fatal");
			break;
		}

		//Get Epoch Milliseconds
		milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		message[L"EpochMs"] = json::value::number(ms.count());

		message[L"SrcMethod"] = json::value::string(ex.source_method);
		message[L"SrcLine"] = json::value::number(ex.source_line);

		//Environment Detail
		json::value environment_detail;
		environment_detail[L"DeviceName"] = json::value::string(server_name_);
		environment_detail[L"AppLocation"] = json::value::string(location_);
		environment_detail[L"ConfiguredAppName"] = json::value::string(application_name_);
		environment_detail[L"ConfiguredEnvironmentName"] = json::value::string(environment_);

		//Server Variables
		if (server_variables_.size() > 0)
		{
			json::value server_variables;
			for (auto it = server_variables_.begin(); it != server_variables_.end(); ++it)
			{
				server_variables[it->first] = json::value::string(it->second);
			}

			message[L"ServerVariables"] = server_variables;
		}

		//Error object
		json::value error;
		error[L"Message"] = json::value::string(ex.error_message);
		error[L"ErrorType"] = json::value::string(ex.error_type);
		error[L"ErrorTypeCode"] = json::value::string(ex.error_type_code);
		error[L"SourceMethod"] = json::value::string(ex.source_method);
		error[L"CustomerName"] = json::value::string(ex.customer_name);
		error[L"UserName"] = json::value::string(ex.user_name);

		//Error Data Object
		if (ex.error_data_map.size() > 0)
		{
			json::value error_data;
			for (auto it = ex.error_data_map.begin(); it != ex.error_data_map.end(); ++it)
			{
				error_data[it->first] = json::value::string(it->second);
			}

			error[L"Data"] = error_data;
		}

		//StackTrace
		//Todo: Review - Should caller pass in stack trace or just generate?  Leaning toward generate for simplicity
		json::value stack_frames;
		int frame_count = 0;

		boost::stacktrace::basic_stacktrace<> stack_trace = boost::stacktrace::stacktrace();
		for (auto it = stack_trace.begin(); it != stack_trace.end(); ++it)
		{
			//Todo: Remove the first 4 sections of stack trace.  Shows this method
			//Convert to source_file to wstring
			std::string source_file = it->source_file();
			std::wstring ws_source_file;
			ws_source_file.assign(source_file.begin(), source_file.end());

			//Convert source line to wstring
			std::wstringstream source_line;
			source_line << it->source_line();

			//Convert method to wstring
			std::string method = it->name();
			std::wstring ws_method;
			ws_method.assign(method.begin(), method.end());

			json::value stack_frame;
			stack_frame[L"CodeFileName"] = json::value::string(ws_source_file);
			stack_frame[L"LineNum"] = json::value::string(source_line.str());
			stack_frame[L"Method"] = json::value::string(ws_method);

			stack_frames[frame_count] = stack_frame;
			frame_count++;
		}

		error[L"StackTrace"] = stack_frames;

		//Add Environment Detail to Exception object
		json::value stackify_exception;
		stackify_exception[L"EnvironmentDetail"] = environment_detail;
		stackify_exception[L"OccurredEpochMillis"] = json::value::number(ms.count());

		stackify_exception[L"Error"] = error;

		//Add Exception detail to message object
		message[L"Ex"] = stackify_exception;

		//Todo:: Add web request details

		// Create the messages array
		json::value messages;
		messages[0] = message;

		// Assign the items array as the value for the Messages key
		root[L"Msgs"] = messages;

		return root;
	}

	std::wstring Stackify::GetLocalComputerName()
	{
		TCHAR computer_name[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD size = sizeof(computer_name);
		GetComputerNameW(computer_name, &size);

		return computer_name;
	}

	//I dont like this over GetWindowsVersionEx, but that is deprecated
	//Todo: Look for more detailed version information api (Registry?)
	std::wstring Stackify::GetOperatingSystemVersion()
	{
		if (IsWindows10OrGreater())
		{
			return L"Windows 10";
		}

		if (IsWindows8Point1OrGreater())
		{
			return L"Windows 8.1";
		}

		if (IsWindows8OrGreater())
		{
			return L"Windows 8";
		}

		if (IsWindows7SP1OrGreater())
		{
			return L"Windows 7 with Service Pack 1";
		}

		if (IsWindows7OrGreater())
		{
			return L"Windows 7";
		}

		if (IsWindowsVistaSP2OrGreater())
		{
			return L"Windows Vista with Service Pack 2";
		}

		if (IsWindowsVistaSP1OrGreater())
		{
			return L"Windows Vista with Service Pack 1";
		}

		if (IsWindowsVistaOrGreater())
		{
			return L"Windows Vista";
		}

		if (IsWindowsXPSP3OrGreater())
		{
			return L"Windows XP with Service Pack 3";
		}

		if (IsWindowsXPSP2OrGreater())
		{
			return L"Windows XP with Service Pack 2";
		}

		if (IsWindowsXPSP1OrGreater())
		{
			return L"Windows XP with Service Pack 1";
		}

		if (IsWindowsXPOrGreater())
		{
			return L"Windows XP";
		}

		if (IsWindowsServer())
		{
			return L"Windows Server";
		}

		return L"Unknown";
	}

	std::string Stackify::GetWindowsTimeZoneName()
	{
		const char *subkey = "SYSTEM\\CurrentControlSet\\Control\\TimeZoneInformation";
		constexpr size_t keysize{ 128 };
		HKEY key;
		char key_name[keysize]{};
		unsigned long tz_keysize = keysize;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subkey, 0,
			KEY_QUERY_VALUE, &key) == ERROR_SUCCESS)
		{
			if (RegQueryValueExA(key, "TimeZoneKeyName", nullptr, nullptr,
				reinterpret_cast<LPBYTE>(key_name), &tz_keysize) != ERROR_SUCCESS)
			{
				memset(key_name, 0, tz_keysize);
			}
			RegCloseKey(key);
		}

		return std::string(key_name);
	}

	std::wstring Stackify::GetWorkingDirectory()
	{
		TCHAR result[MAX_PATH];
		return std::wstring(result, GetModuleFileNameW(nullptr, result, MAX_PATH));
	}

	pplx::task<void> Stackify::PostMessage(std::wstring api_key, json::value message)
	{
		// Manually build up an HTTP request with header and request URI.
		http_request request(methods::POST);

		request.headers().add(L"Content-Type", L"application/json");
		request.headers().add(L"Accept", L"application/json");
		request.headers().add(L"X-Stackify-PV", L"V1");
		request.headers().add(L"X-Stackify-Key", api_key.c_str());

		request.set_request_uri(L"log/save");
		request.set_body(message);

		http_client client(L"https://api.stackify.com");

		return client.request(request).then([](web::http::http_response response)
		{
			//This is really just fire and forget.  Uncomment for debugging
			/*if (response.status_code() == status_codes::OK)
			{
			auto body = response.extract_string();
			std::wcout << L"Updated: " << body.get().c_str() << std::endl;
			}*/
		});
	}

	void Stackify::SetEnvironment(std::wstring env)
	{
		environment_ = env;
	}

	std::wstring Stackify::GetEnvironment()
	{
		return environment_;
	}

	void Stackify::SetApplicationName(std::wstring app_name)
	{
		application_name_ = app_name;
	}

	std::wstring Stackify::GetApplicationName()
	{
		return application_name_;
	}

	void Stackify::SetServerVariable(std::wstring key, std::wstring value)
	{
		server_variables_.insert_or_assign(key, value);
	}

	std::wstring Stackify::GetServerVariable(std::wstring key)
	{
		const std::map<std::wstring, std::wstring>::iterator it = server_variables_.find(key);
		if (it != server_variables_.end())
			return it->second;

		return L"Key Not Found";
	}
}